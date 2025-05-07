// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.Win.PlatformApplicationWin.hpp"

#include "Futurewalker.Application.Win.PlatformEventLoopContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformEventLoopWin.hpp"

#include "Futurewalker.Application.PlatformApplicationEvent.hpp"

#include "Futurewalker.Async.AsyncFunction.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
PlatformApplicationWin::PlatformApplicationWin(Delegate delegate, Shared<PlatformApplicationContextWin> context, Shared<PlatformEventLoop> eventLoop, Shared<PlatformMainThread> mainThread)
  : PlatformApplication(delegate)
  , _context {context}
  , _eventLoop {eventLoop}
  , _mainThread {mainThread}
{
}

///
/// @brief
///
auto PlatformApplicationWin::Run() -> Async<void>
{
    if (!_mainThread->IsMainThread())
    {
        throw Exception(ErrorCode::InvalidOperation, "Application must run on main thread");
    }

    co_await _eventLoop->Run(
      {.startTask = [&]() -> Async<void> {
           auto e = Event(PlatformApplicationEvent::Started());
           co_await SendApplicationEvent(e);
       },
       .exitTask = [&]() -> Async<void> {
           auto e = Event(PlatformApplicationEvent::Exited());
           co_await SendApplicationEvent(e);
       }});
}

///
/// @brief
///
auto PlatformApplicationWin::Exit() -> void
{
    _eventLoop->Exit();
}

///
/// @brief
///
auto PlatformApplicationWin::IsRunning() -> Bool
{
    if (auto const eventLoop = _eventLoop.Maybe<PlatformEventLoopWin>())
    {
        return eventLoop->IsRunning();
    }
    return false;
}

///
/// @brief Returns whether the application is active or not.
///
/// Application will receive `ApplicationEvent::ActiveChanged` when this property changed.
///
auto PlatformApplicationWin::IsActive() -> Bool
{
    return _active;
}

///
/// @brief Returns whether the application is foreground or not.
///
/// Application will receive `ApplicationEvent::ForegroundChanged` when this property changed.
///
auto PlatformApplicationWin::IsForeground() -> Bool
{
    return true;
}

///
/// @brief
///
/// @param active
///
auto PlatformApplicationWin::SetActive(Bool const active) -> void
{
    if (_active != active)
    {
        _active = active;

        auto parameter = PlatformApplicationEvent::ActiveChanged();
        parameter.SetActive(active);
        auto event = Event(parameter);
        SyncSendApplicationEvent(event);
    }
}

///
/// @brief
///
auto PlatformApplicationWin::SyncSendApplicationEvent(Event const& event) -> void
{
    if (auto eventLoopContext = Locator::GetInstance<PlatformEventLoopContext>())
    {
        if (auto eventLoop = eventLoopContext->GetEventLoopForThread().As<PlatformEventLoopWin>())
        {
            eventLoop->Spawn(
              AsyncFunction::Fn([&]() -> Lazy<void> {
                  auto e = event;
                  co_await SendApplicationEvent(e);
              }),
              false);
        }
    }
}
}

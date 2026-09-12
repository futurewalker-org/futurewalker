// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformApplicationWin.hpp"

#include "Futurewalker.Application.PlatformApplicationEvent.hpp"

#include "Futurewalker.Async.AsyncFunction.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Core.Win.PlatformWindowMessageWin.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Create PlatformApplicationWin object.
///
auto PlatformApplicationWin::Make(Delegate delegate, Shared<PlatformApplicationContextWin> context) -> Shared<PlatformApplicationWin>
{
    return PlatformApplication::MakeDerived<PlatformApplicationWin>(delegate, context);
}

///
/// @brief
///
PlatformApplicationWin::PlatformApplicationWin(PassKey<PlatformApplication>, Delegate delegate, Shared<PlatformApplicationContextWin> context)
  : PlatformApplication(delegate)
  , _context {context}
{
    if (!_context)
    {
        throw Exception(ErrorCode::Failure);
    }
}

///
/// @brief
///
auto PlatformApplicationWin::RequestQuit() -> void
{
    if (_context->IsMainThread() && _context->IsRunning())
    {
        auto cancelled = False;
        auto event = Event<>(Event<PlatformApplicationEvent::QuitRequested>());
        if (SendApplicationEvent(event))
        {
            if (event.Is<PlatformApplicationEvent::QuitRequested>())
            {
                cancelled = event.As<PlatformApplicationEvent::QuitRequested>()->IsCancelled();
            }
        }

        if (!cancelled)
        {
            _context->PostQuitMessage();
        }
    }
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
/// @brief Set main menu.
///
auto PlatformApplicationWin::SetMainMenu(Menu const& menu) -> void
{
    (void)menu;
}

///
/// @brief Mark the application as started.
///
auto PlatformApplicationWin::SetStarted() -> void
{
    if (!_started)
    {
        _started = true;
        auto e = Event<>(Event<PlatformApplicationEvent::Started>());
        SendApplicationEvent(e);
    }
}

///
/// @brief Mark the application as quitting.
///
auto PlatformApplicationWin::SetQuitting() -> void
{
    if (!_quitting)
    {
        _quitting = true;
        auto e = Event<>(Event<PlatformApplicationEvent::Quitting>());
        SendApplicationEvent(e);
    }
}

///
/// @brief Set active state.
///
auto PlatformApplicationWin::SetActive(Bool const active) -> void
{
    if (_active != active)
    {
        _active = active;

        auto parameter = Event<PlatformApplicationEvent::ActiveChanged>();
        parameter->SetActive(active);
        auto event = Event<>(parameter);
        SendApplicationEvent(event);
    }
}

///
/// @brief Initialize.
///
auto PlatformApplicationWin::Initialize() -> void
{
}
}

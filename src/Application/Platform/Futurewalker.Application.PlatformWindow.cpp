// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformWindow.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
/// @param delegate
///
PlatformWindow::PlatformWindow(PassKey<PlatformWindow>, Delegate const& delegate)
  : _delegate {delegate}
{
}

///
/// @brief
///
auto PlatformWindow::SetDelegate(Delegate const& delegate) -> void
{
    _delegate = delegate;
}

///
/// @brief
///
PlatformWindow::~PlatformWindow() = default;

///
/// @brief
///
auto PlatformWindow::SendWindowEvent(Event<>& event) -> Async<Bool>
{
    co_return co_await SendEvent(event, _delegate.sendWindowEvent);
}

///
/// @brief
///
auto PlatformWindow::SendFrameEvent(Event<>& event) -> Async<Bool>
{
    co_return co_await SendEvent(event, _delegate.sendFrameEvent);
}

///
/// @brief
///
/// @param event
///
auto PlatformWindow::SendPointerEvent(Event<>& event) -> Async<Bool>
{
    co_return co_await SendEvent(event, _delegate.sendPointerEvent);
}

///
/// @brief
///
/// @param event
///
auto PlatformWindow::SendKeyEvent(Event<>& event) -> Async<Bool>
{
    co_return co_await SendEvent(event, _delegate.sendKeyEvent);
}

///
/// @brief
///
/// @param event
///
auto PlatformWindow::SendInputEvent(Event<>& event) -> Async<Bool>
{
    co_return co_await SendEvent(event, _delegate.sendInputEvent);
}

///
/// @brief
///
auto PlatformWindow::SendHitTestEvent(Event<>& event) -> Async<Bool>
{
    co_return co_await SendEvent(event, _delegate.sendHitTestEvent);
}

///
/// @brief
///
auto PlatformWindow::SendWindowEventDetached(Event<>& event) -> Bool
{
    return SendEventDetached(event, &PlatformWindow::SendWindowEvent);
}

///
/// @brief
///
auto PlatformWindow::SendFrameEventDetached(Event<>& event) -> Bool
{
    return SendEventDetached(event, &PlatformWindow::SendFrameEvent);
}

///
/// @brief
///
auto PlatformWindow::SendPointerEventDetached(Event<>& event) -> Bool
{
    return SendEventDetached(event, &PlatformWindow::SendPointerEvent);
}

///
/// @brief
///
auto PlatformWindow::SendKeyEventDetached(Event<>& event) -> Bool
{
    return SendEventDetached(event, &PlatformWindow::SendKeyEvent);
}

///
/// @brief
///
auto PlatformWindow::SendInputEventDetached(Event<>& event) -> Bool
{
    return SendEventDetached(event, &PlatformWindow::SendInputEvent);
}

///
/// @brief
///
auto PlatformWindow::SendHitTestEventDetached(Event<>& event) -> Bool
{
    return SendEventDetached(event, &PlatformWindow::SendHitTestEvent);
}

///
/// @brief
///
auto PlatformWindow::SendEvent(Event<>& event, EventFunction const& func) -> Async<Bool>
{
    if (func)
    {
        co_return co_await func(event);
    }
    co_return false;
}

///
/// @brief
///
auto PlatformWindow::SendEventDetached(Event<>& event, auto (PlatformWindow::*func)(Event<>&)->Async<Bool>) -> Bool
{
    // GetSelf() may return null during destructor call.
    if (auto const self = GetSelf())
    {
        auto const r = Shared<Bool>::Make(false);
        auto const e = Shared<Event<>>::Make(event);
        AsyncFunction::SpawnFn([=]() mutable -> Task<void> {
            try
            {
                *r = co_await ((&*self)->*func)(*e);
            }
            catch (...)
            {
                FW_DEBUG_ASSERT(false);
            }
        }).Detach();
        event = *e;
        return *r;
    }

    if constexpr (BuildConfig::IsDebug())
    {
        auto const& eventParameter = *event;
        FW_DEBUG_LOG_WARNING("PlatformWindow::SendEventDetached(): Tried to send event from destructing window, ignoring. [{}]", typeid(eventParameter).name());
    }
    return false;
}
}

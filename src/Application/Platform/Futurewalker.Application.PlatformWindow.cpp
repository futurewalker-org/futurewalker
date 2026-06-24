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
auto PlatformWindow::SendWindowEvent(Event<>& event) -> Bool
{
    return SendEvent(event, _delegate.sendWindowEvent);
}

///
/// @brief
///
auto PlatformWindow::SendFrameEvent(Event<>& event) -> Bool
{
    return SendEvent(event, _delegate.sendFrameEvent);
}

///
/// @brief
///
/// @param event
///
auto PlatformWindow::SendPointerEvent(Event<>& event) -> Bool
{
    return SendEvent(event, _delegate.sendPointerEvent);
}

///
/// @brief
///
/// @param event
///
auto PlatformWindow::SendKeyEvent(Event<>& event) -> Bool
{
    return SendEvent(event, _delegate.sendKeyEvent);
}

///
/// @brief
///
/// @param event
///
auto PlatformWindow::SendInputEvent(Event<>& event) -> Bool
{
    return SendEvent(event, _delegate.sendInputEvent);
}

///
/// @brief
///
auto PlatformWindow::SendHitTestEvent(Event<>& event) -> Bool
{
    return SendEvent(event, _delegate.sendHitTestEvent);
}
///
/// @brief
///
auto PlatformWindow::SendEvent(Event<>& event, EventFunction const& func) -> Bool
{
    if (func)
    {
        return func(event);
    }
    return false;
}
}

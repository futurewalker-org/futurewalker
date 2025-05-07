// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.PlatformWindow.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
/// @param delegate
///
PlatformWindow::PlatformWindow(Delegate const& delegate)
  : _delegate {delegate}
{
}

///
/// @brief
///
PlatformWindow::~PlatformWindow() = default;

///
/// @brief
///
/// @param event
///
auto PlatformWindow::SendPointerEvent(Event& event) -> Async<Bool>
{
    if (_delegate.sendPointerEvent)
    {
        co_return co_await _delegate.sendPointerEvent(event);
    }
    co_return false;
}

///
/// @brief
///
/// @param event
///
auto PlatformWindow::SendKeyEvent(Event& event) -> Async<Bool>
{
    if (_delegate.sendKeyEvent)
    {
        co_return co_await _delegate.sendKeyEvent(event);
    }
    co_return false;
}

///
/// @brief
///
/// @param event
///
auto PlatformWindow::SendInputEvent(Event& event) -> Async<Bool>
{
    if (_delegate.sendInputEvent)
    {
        co_return co_await _delegate.sendInputEvent(event);
    }
    co_return false;
}

///
/// @brief
///
auto PlatformWindow::SendFrameEvent(Event& event) -> Async<Bool>
{
    if (_delegate.sendFrameEvent)
    {
        co_return co_await _delegate.sendFrameEvent(event);
    }
    co_return false;
}

///
/// @brief
///
/// @param event
///
/// @return
///
auto PlatformWindow::SendWindowEvent(Event& event) -> Async<Bool>
{
    if (_delegate.sendWindowEvent)
    {
        co_return co_await _delegate.sendWindowEvent(event);
    }
    co_return false;
}
}

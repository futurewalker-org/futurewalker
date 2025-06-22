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

///
/// @brief
///
auto PlatformWindow::SendWindowEventDetached(Event const& event) -> void
{
    AsyncFunction::SpawnFn([self = GetSelf(), e = event]() mutable -> Task<void> {
        try
        {
            co_await self->SendWindowEvent(e);
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
    });
}

///
/// @brief
///
auto PlatformWindow::SendFrameEventDetached(Event const& event) -> void
{
    AsyncFunction::SpawnFn([self = GetSelf(), e = event]() mutable -> Task<void> {
        try
        {
            co_await self->SendFrameEvent(e);
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
    });
}

///
/// @brief
///
auto PlatformWindow::SendPointerEventDetached(Event const& event) -> void
{
    AsyncFunction::SpawnFn([self = GetSelf(), e = event]() mutable -> Task<void> {
        try
        {
            co_await self->SendPointerEvent(e);
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
    });
}

///
/// @brief
///
auto PlatformWindow::SendKeyEventDetached(Event const& event) -> void
{
    AsyncFunction::SpawnFn([self = GetSelf(), e = event]() mutable -> Task<void> {
        try
        {
            co_await self->SendKeyEvent(e);
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
    });
}

///
/// @brief
///
auto PlatformWindow::SendInputEventDetached(Event const& event) -> void
{
    AsyncFunction::SpawnFn([self = GetSelf(), e = event]() mutable -> Task<void> {
        try
        {
            co_await self->SendInputEvent(e);
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
    });
}
}

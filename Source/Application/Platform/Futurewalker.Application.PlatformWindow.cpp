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
auto PlatformWindow::SendWindowEvent(Event<>& event) -> Async<Bool>
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
auto PlatformWindow::SendFrameEvent(Event<>& event) -> Async<Bool>
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
auto PlatformWindow::SendPointerEvent(Event<>& event) -> Async<Bool>
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
auto PlatformWindow::SendKeyEvent(Event<>& event) -> Async<Bool>
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
auto PlatformWindow::SendInputEvent(Event<>& event) -> Async<Bool>
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
auto PlatformWindow::SendHitTestEvent(Event<>& event) -> Async<Bool>
{
    if (_delegate.sendHitTestEvent)
    {
        co_return co_await _delegate.sendHitTestEvent(event);
    }
    co_return false;
}

///
/// @brief
///
auto PlatformWindow::SendWindowEventDetached(Event<>& event) -> Bool
{
    auto const r = Shared<Bool>::Make(false);
    auto const e = Shared<Event<>>::Make(event);
    auto const self = GetSelf();
    AsyncFunction::SpawnFn([=]() mutable -> Task<void> {
        try
        {
            *r = co_await self->SendWindowEvent(*e);
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
    }).Detach();
    event = *e;
    return *r;
}

///
/// @brief
///
auto PlatformWindow::SendFrameEventDetached(Event<>& event) -> Bool
{
    auto const r = Shared<Bool>::Make(false);
    auto const e = Shared<Event<>>::Make(event);
    auto const self = GetSelf();
    AsyncFunction::SpawnFn([=]() mutable -> Task<void> {
        try
        {
            *r = co_await self->SendFrameEvent(*e);
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
    }).Detach();
    event = *e;
    return *r;
}

///
/// @brief
///
auto PlatformWindow::SendPointerEventDetached(Event<>& event) -> Bool
{
    auto const r = Shared<Bool>::Make(false);
    auto const e = Shared<Event<>>::Make(event);
    auto const self = GetSelf();
    AsyncFunction::SpawnFn([=]() mutable -> Task<void> {
        try
        {
            *r = co_await self->SendPointerEvent(*e);
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
    }).Detach();
    event = *e;
    return *r;
}

///
/// @brief
///
auto PlatformWindow::SendKeyEventDetached(Event<>& event) -> Bool
{
    auto const r = Shared<Bool>::Make(false);
    auto const e = Shared<Event<>>::Make(event);
    auto const self = GetSelf();
    AsyncFunction::SpawnFn([=]() mutable -> Task<void> {
        try
        {
            *r = co_await self->SendKeyEvent(*e);
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
    }).Detach();
    event = *e;
    return *r;
}

///
/// @brief
///
auto PlatformWindow::SendInputEventDetached(Event<>& event) -> Bool
{
    auto const r = Shared<Bool>::Make(false);
    auto const e = Shared<Event<>>::Make(event);
    auto const self = GetSelf();
    AsyncFunction::SpawnFn([=]() mutable -> Task<void> {
        try
        {
            *r = co_await self->SendInputEvent(*e);
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
    }).Detach();
    event = *e;
    return *r;
}

///
/// @brief
///
auto PlatformWindow::SendHitTestEventDetached(Event<>& event) -> Bool
{
    auto const r = Shared<Bool>::Make(false);
    auto const e = Shared<Event<>>::Make(event);
    auto const self = GetSelf();
    AsyncFunction::SpawnFn([=]() mutable -> Task<void> {
        try
        {
            *r = co_await self->SendHitTestEvent(*e);
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
    }).Detach();
    event = *e;
    return *r;
}
}
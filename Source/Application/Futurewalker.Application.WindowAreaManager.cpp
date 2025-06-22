// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.WindowAreaManager.hpp"
#include "Futurewalker.Application.WindowAreaManagerEvent.hpp"

#include "Futurewalker.Event.Event.hpp"
#include "Futurewalker.Event.EventReceiver.hpp"
#include "Futurewalker.Event.EventReceiver.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Make window area manager.
///
/// @param delegate Delegate.
///
auto WindowAreaManager::Make() -> Shared<WindowAreaManager>
{
    return Shared<WindowAreaManager>::Make(PassKey<WindowAreaManager>());
}

///
/// @brief Constructor.
///
/// @param delegate Delegate.
///
WindowAreaManager::WindowAreaManager(PassKey<WindowAreaManager>)
{
    _eventReceiver = EventReceiver::Make();
}

///
/// @brief Get rectangle of avaiable area in title bar.
///
auto WindowAreaManager::GetTitleBarAvailableAreaRect() const -> Rect<Dp>
{
    return _titleBarAvailableAreaRect;
}

///
/// @brief Set avaiable area in title bar.
///
auto WindowAreaManager::SetTitleBarAvailableAreaRect(Rect<Dp> const& rect) -> void
{
    if (_titleBarAvailableAreaRect != rect)
    {
        _titleBarAvailableAreaRect = rect;
        auto event = Event<>(Event<WindowAreaManagerEvent::TitleBarAvailableAreaRectChanged>());
        _eventReceiver->SendEventDetached(event);
    }
}

auto WindowAreaManager::GetTitleBarAreaRect() const -> Rect<Dp>
{
    return _titleBarAreaRect;
}

auto WindowAreaManager::SetTitleBarAreaRect(Rect<Dp> const& rect) -> void
{
    if (_titleBarAreaRect != rect)
    {
        _titleBarAreaRect = rect;
        auto event = Event<>(Event<WindowAreaManagerEvent::TitleBarAreaRectChanged>());
        _eventReceiver->SendEventDetached(event);
    }
}

auto WindowAreaManager::GetContentAreaRect() const -> Rect<Dp>
{
    return _contentAreaRect;
}

auto WindowAreaManager::SetContentAreaRect(Rect<Dp> const& rect) -> void
{
    if (_contentAreaRect != rect)
    {
        _contentAreaRect = rect;
        auto event = Event<>(Event<WindowAreaManagerEvent::ContentAreaRectChanged>());
        _eventReceiver->SendEventDetached(event);
    }
}

auto WindowAreaManager::GetEventReceiver() -> EventReceiver&
{
    return *_eventReceiver;
}

auto WindowAreaManager::GetEventReceiver() const -> EventReceiver const&
{
    return *_eventReceiver;
}
}

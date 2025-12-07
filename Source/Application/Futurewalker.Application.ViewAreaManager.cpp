// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ViewAreaManager.hpp"
#include "Futurewalker.Application.ViewAreaManagerEvent.hpp"

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
auto ViewAreaManager::Make() -> Shared<ViewAreaManager>
{
    return Shared<ViewAreaManager>::Make(PassKey<ViewAreaManager>());
}

///
/// @brief Constructor.
///
/// @param delegate Delegate.
///
ViewAreaManager::ViewAreaManager(PassKey<ViewAreaManager>)
{
    _eventReceiver = EventReceiver::Make();
}

///
/// @brief Get insets which occupied by the view area.
///
/// @param[in] areaType View area type.
///
/// @note Insets are relative to the root view.
///
auto ViewAreaManager::GetAreaInsets(ViewArea const areaType) const -> EdgeInsets
{
    if (auto it = _insets.find(areaType); it != _insets.end())
    {
        return it->second;
    }
    return {};
}

///
/// @brief Set insets of view area.
///
/// @param[in] areaType View area type.
/// @param[in] insets Insets to set.
///
auto ViewAreaManager::SetAreaInsets(ViewArea const areaType, EdgeInsets const& insets) -> void
{
    _insets[areaType] = insets;
    SendAreaGeometryChangedEvent();
}

///
/// @brief Get bounding rectangles occupied by system controls in the view area.
///
/// @param[in] areaType View area type.
///
/// @note Bounding rectangles are relative to the root view.
///
auto ViewAreaManager::GetAreaBounds(ViewArea const areaType) const -> std::vector<Rect<Dp>>
{
    if (auto it = _boundingRects.find(areaType); it != _boundingRects.end())
    {
        return it->second;
    }
    return {};
}

///
/// @brief Set bounding rectangles of view area.
///
/// @param[in] areaType View area type.
/// @param[in] rects Bounding rectangles to set.
///
auto ViewAreaManager::SetAreaBounds(ViewArea const areaType, std::vector<Rect<Dp>> const& rects) -> void
{
    _boundingRects[areaType] = rects;
    SendAreaGeometryChangedEvent();
}

auto ViewAreaManager::GetEventReceiver() -> EventReceiver&
{
    return *_eventReceiver;
}

auto ViewAreaManager::GetEventReceiver() const -> EventReceiver const&
{
    return *_eventReceiver;
}

auto ViewAreaManager::SendAreaGeometryChangedEvent() -> void
{
    auto event = Event<ViewAreaManagerEvent::GeometryChanged>();
    _eventReceiver->SendEventDetached(event);
}
}

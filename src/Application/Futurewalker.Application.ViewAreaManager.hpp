// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.ViewAreaManagerType.hpp"
#include "Futurewalker.Application.EdgeInsets.hpp"

#include "Futurewalker.Event.EventReceiverType.hpp"
#include "Futurewalker.Event.EventReceiverType.hpp"
#include "Futurewalker.Event.EventParameter.hpp"

#include "Futurewalker.Async.Async.hpp"

#include "Futurewalker.Core.Function.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.PassKey.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief View area manager.
///
class ViewAreaManager : NonCopyable
{
public:
    static auto Make() -> Shared<ViewAreaManager>;

    ViewAreaManager(PassKey<ViewAreaManager>);

    auto GetAreaInsets(ViewArea const areaType) const -> EdgeInsets;
    auto SetAreaInsets(ViewArea const areaType, EdgeInsets const& insets) -> void;

    auto GetAreaBounds(ViewArea const areaType) const -> std::vector<Rect<Dp>>;
    auto SetAreaBounds(ViewArea const areaType, std::vector<Rect<Dp>> const& rects) -> void;

    auto GetEventReceiver() -> EventReceiver&;
    auto GetEventReceiver() const -> EventReceiver const&;

private:
    auto SendAreaGeometryChangedEvent() -> void;

private:
    Shared<EventReceiver> _eventReceiver;
    HashMap<ViewArea, EdgeInsets> _insets;
    HashMap<ViewArea, std::vector<Rect<Dp>>> _boundingRects;
};
}
}

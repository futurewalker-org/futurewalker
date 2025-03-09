// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.HitTestEventType.hpp"
#include "Futurewalker.Application.ViewType.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

#include "Futurewalker.Graphics.Point.hpp"
#include "Futurewalker.Graphics.GraphicsUnits.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Hit test event.
///
class HitTestEvent : public EventParameter
{
public:
    auto GetPosition() const -> Point<Dp>;
    auto SetPosition(Point<Dp> const& position) -> void;

    auto GetHit() const -> Bool;
    auto SetHit(Bool const hit) -> void;

private:
    Point<Dp> _position;
    Bool _hit = false;
};
}
}

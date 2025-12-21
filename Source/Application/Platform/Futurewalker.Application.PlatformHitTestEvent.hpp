// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformHitTestEventType.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformHitTestEvent : public EventParameter
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

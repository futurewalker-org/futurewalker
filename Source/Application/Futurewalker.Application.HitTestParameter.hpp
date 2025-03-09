// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.HitTestParameterType.hpp"

#include "Futurewalker.Graphics.Point.hpp"
#include "Futurewalker.Graphics.GraphicsUnits.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class HitTestParameter
{
public:
    auto GetPosition() const -> Point<Dp> const&;
    auto SetPosition(Point<Dp> const& position) -> void;

private:
    Point<Dp> _position;
};
}
}

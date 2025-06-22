// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.HitTestParameterType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

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

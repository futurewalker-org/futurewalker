// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.BoxConstraintsType.hpp"
#include "Futurewalker.Application.AxisConstraints.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
class BoxConstraints
{
public:
    static auto MakeUnconstrained() -> BoxConstraints;
    static auto MakeExact(Dp const width, Dp const height) -> BoxConstraints;
    static auto MakeMinMax(AxisConstraints const& horizontal, AxisConstraints const& vertical) -> BoxConstraints;

public:
    auto operator<=>(BoxConstraints const&) const = default;

    auto GetWidthConstraints() const -> AxisConstraints;
    auto SetWidthConstraints(AxisConstraints const& width) -> void;

    auto GetHeightConstraints() const -> AxisConstraints;
    auto SetHeightConstraints(AxisConstraints const& height) -> void;

    auto GetMaxSize() const -> Size<Dp>;
    auto GetMinSize() const -> Size<Dp>;

    auto ConstrainSize(Size<Dp> const& size) const -> Size<Dp>;

private:
    BoxConstraints(AxisConstraints const& width, AxisConstraints const& height);

private:
    AxisConstraints _width;
    AxisConstraints _height;
};
}
}

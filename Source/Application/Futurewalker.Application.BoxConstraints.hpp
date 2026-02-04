// SPDX-License-Identifier: MPL-2.0
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
    static auto MakeExact(Size<Dp> const& size) -> BoxConstraints;
    static auto MakeMinMax(Dp const min, Dp const max) -> BoxConstraints;

    static auto Constrain(BoxConstraints const& cs, Size<Dp> const& size) -> Size<Dp>;
    static auto Offset(BoxConstraints const& cs, Dp const horizontal, Dp const vertical) -> BoxConstraints;

public:
    BoxConstraints() = default;
    BoxConstraints(AxisConstraints const& width, AxisConstraints const& height);

    auto operator<=>(BoxConstraints const&) const = default;

    auto GetWidthConstraints() const -> AxisConstraints const&;
    auto SetWidthConstraints(AxisConstraints const& width) -> void;

    auto GetHeightConstraints() const -> AxisConstraints const&;
    auto SetHeightConstraints(AxisConstraints const& height) -> void;

    auto GetMaxSize() const -> Size<Dp>;
    auto GetMinSize() const -> Size<Dp>;

private:
    AxisConstraints _width;
    AxisConstraints _height;
};
}
}

// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.AxisConstraintsType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Axis constraints.
///
/// Constraints are described as a (min, max) pair of size limits.
///
/// @note Both minimum and maximum values cannot be negative.
/// @note While maximum value can be infinite, minimum value must be finite and smaller thant maximum value.
///
class AxisConstraints
{
public:
    static auto MakeUnconstrained() noexcept -> AxisConstraints;
    static auto MakeExact(Dp const value) noexcept -> AxisConstraints;
    static auto MakeMinMax(Dp const min, Dp const max) noexcept -> AxisConstraints;

    static auto IsNearlyEqual(AxisConstraints const& lhs, AxisConstraints const& rhs, Float64 const tolerance = 1e-6) noexcept -> Bool;
    static auto IsNearlyEqualRelative(AxisConstraints const& lhs, AxisConstraints const& rhs, Float64 const precision = 1e-6, Float64 const minTolerance = 1e-6) noexcept -> Bool;
    static auto Constrain(AxisConstraints const& cs, Dp const value) -> Dp;
    static auto Offset(AxisConstraints const& cs, Dp const offset) -> AxisConstraints;

public:
    AxisConstraints() = default;
    AxisConstraints(AxisConstraints const&) = default;

    auto operator=(AxisConstraints const&) -> AxisConstraints& = default;
    auto operator<=>(AxisConstraints const&) const = default;

    auto GetMin() const -> Dp;
    auto SetMin(Dp const min) -> void;

    auto GetMax() const -> Dp;
    auto SetMax(Dp const max) -> void;

    auto IsBounded() const -> Bool;
    auto IsExact() const -> Bool;

private:
    AxisConstraints(Dp const min, Dp const max);

private:
    Dp _min = 0.0;
    Dp _max = Dp::Infinity();
};
}
}

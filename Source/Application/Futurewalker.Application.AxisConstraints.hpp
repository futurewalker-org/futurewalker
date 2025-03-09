// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.AxisConstraintsType.hpp"

#include "Futurewalker.Graphics.GraphicsUnits.hpp"

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
    static auto MakeExact(const Dp value) noexcept -> AxisConstraints;
    static auto MakeMinMax(const Dp min, const Dp max) noexcept -> AxisConstraints;

    static auto IsNearlyEqual(const AxisConstraints& lhs, const AxisConstraints& rhs, const Float64 tolerance = 1e-6) noexcept -> Bool;
    static auto IsNearlyEqualRelative(const AxisConstraints& lhs, const AxisConstraints& rhs, const Float64 precision = 1e-6, const Float64 minTolerance = 1e-6) noexcept -> Bool;
    static auto Constrain(const AxisConstraints& cs, const Dp value) -> Dp;

public:
    AxisConstraints() = default;
    AxisConstraints(const AxisConstraints&) = default;

    auto operator=(const AxisConstraints&) -> AxisConstraints& = default;
    auto operator<=>(const AxisConstraints&) const = default;

    auto GetMin() const -> Dp;
    auto SetMin(const Dp min) -> void;

    auto GetMax() const -> Dp;
    auto SetMax(const Dp max) -> void;

    auto IsBounded() const -> Bool;
    auto IsExact() const -> Bool;

private:
    AxisConstraints(const Dp min, const Dp max);

private:
    Dp _min = 0.0;
    Dp _max = Dp::Infinity();
};
}
}

// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.AxisConstraints.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Make unconstrained (0, infinite) constraints....
///
auto AxisConstraints::MakeUnconstrained() noexcept -> AxisConstraints
{
    return AxisConstraints(0.0, Dp::Infinity());
}

///
/// @brief Make exact (x, x) constraints.
///
/// @param value Positive, finite value of constraints
///
auto AxisConstraints::MakeExact(Dp const value) noexcept -> AxisConstraints
{
    return AxisConstraints(value, value);
}

///
/// @brief Make (min, max) constraints.
///
/// @param min Minimum value
/// @param max Maximum value
///
auto AxisConstraints::MakeMinMax(Dp const min, Dp const max) noexcept -> AxisConstraints
{
    return AxisConstraints(min, max);
}

///
/// @brief Returns true if two values are close within absolute tolerance.
///
/// @param lhs
/// @param rhs
/// @param tolerance
///
auto AxisConstraints::IsNearlyEqual(AxisConstraints const& lhs, AxisConstraints const& rhs, Float64 const tolerance) noexcept -> Bool
{
    return Dp::IsNearlyEqual(lhs._min, rhs._min, tolerance) && Dp::IsNearlyEqual(lhs._max, rhs._max, tolerance);
}

///
/// @brief Returns true if two values are close within relative tolerance.
///
/// @param lhs
/// @param rhs
/// @param precision
/// @param minTolerance
///
auto AxisConstraints::IsNearlyEqualRelative(AxisConstraints const& lhs, AxisConstraints const& rhs, Float64 const precision, Float64 const minTolerance) noexcept -> Bool
{
    return Dp::IsNearlyEqualRelative(lhs._min, rhs._min, precision, minTolerance) && Dp::IsNearlyEqualRelative(lhs._max, rhs._max, precision, minTolerance);
}

///
/// @brief Constrain value by constraints.
///
/// @param cs Constraints to apply
/// @param value Value to be constrained
///
/// @return Constrained value
///
auto AxisConstraints::Constrain(AxisConstraints const& cs, Dp const value) -> Dp
{
    return Dp::Clamp(value, cs._min, cs._max);
}

///
/// @brief Offset constraints by a value.
///
/// @param cs Constraints to offset.
/// @param offset Offset value.
///
/// @return New constraints with offset applied.
///
auto AxisConstraints::Offset(AxisConstraints const& cs, Dp const offset) -> AxisConstraints
{
    if (Dp::IsFinite(offset))
    {
        auto const min = cs._min + offset;
        auto const max = cs._max + offset;
        return AxisConstraints::MakeMinMax(min, max);
    }
    return cs;
}

///
/// @brief
///
auto AxisConstraints::GetMin() const -> Dp
{
    return _min;
}

///
/// @brief
///
/// @param min
///
/// @note `min` should not be NaN.
/// @note `min` will be clamped between [0, max].
///
auto AxisConstraints::SetMin(Dp const min) -> void
{
    if (Dp::IsNaN(min))
    {
        FW_DEBUG_ASSERT(false);
    }
    else
    {
        _min = Dp::Clamp(min, 0, _max);
    }
}

///
/// @brief
///
auto AxisConstraints::GetMax() const -> Dp
{
    return _max;
}

///
/// @brief
///
/// @param max
///
/// @note `max` should not be NaN.
/// @note `max` will be clamped between [min, infinity].
///
auto AxisConstraints::SetMax(Dp const max) -> void
{
    if (Dp::IsNaN(max))
    {
        FW_DEBUG_ASSERT(false);
    }
    else
    {
        _max = Dp::Max(max, _min);
    }
}

///
/// @brief Returns true if constraints are bounded
///
auto AxisConstraints::IsBounded() const -> Bool
{
    return Dp::IsFinite(_max);
}

///
/// @brief Returns true if constraints are exact
///
auto AxisConstraints::IsExact() const -> Bool
{
    return _min == _max;
}

///
/// @brief Construct from (min, max) pair.
///
/// @param min Minimum value
/// @param max Maximum value
///
AxisConstraints::AxisConstraints(Dp const min, Dp const max)
  : _min {0}
  , _max {Dp::Infinity()}
{
    SetMin(min);
    SetMax(max);
}
}

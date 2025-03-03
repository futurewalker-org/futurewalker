// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Graphics.RadiusType.hpp"

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief A specialization of Radius2 for Float types.
///
/// @tparam T Floating point type.
/// @tparam Tag Tag type.
///
template <Concepts::FloatingPoint T, class Tag>
class Radius2<Float<T, Tag>>
{
public:
    ///
    /// @brief Value type.
    ///
    using ValueType = Float<T, Tag>;

    ///
    /// @brief Returns true if two values are nearly equal
    ///
    /// @param lhs Radius to compare
    /// @param rhs Radius to compare
    /// @param tolerance Absolute tolerance
    ///
    static inline constexpr Bool IsNearlyEquals(const Radius2& lhs ,const Radius2& rhs, const T& tolerance = 1e-6) noexcept
    {
        return ValueType::IsNearlyEqual(lhs._x, rhs._x, tolerance) && ValueType::IsNearlyEqual(lhs._y, rhs._y, tolerance);
    }

    ///
    /// @brief Returns true if two values are nearly equal
    ///
    /// @param lhs Radius to compare
    /// @param rhs Radius to compare
    /// @param precision Relative precision
    /// @param minTolerance Absolute minimum tolerance
    ///
    static inline constexpr Bool IsNearlyEqualRelative(const Radius2& lhs, const Radius2& rhs, const T& precision = 1e-6, const T& minTolerance = 1e-6) noexcept
    {
        return ValueType::IsNearlyEqualRelative(lhs._x, rhs._y, precision, minTolerance) && ValueType::IsNearlyEqualRelative(lhs._x, rhs._y, precision, minTolerance);
    }

    ///
    /// @brief Get minimum components.
    ///
    /// @param lhs Radius to compare
    /// @param rhs Radius to compare
    ///
    /// @return Radius of minimum components
    ///
    static inline constexpr auto Min(const Radius2& lhs, const Radius2& rhs) -> Radius2
    {
        return Radius2(ValueType::Min(lhs._x, rhs._x), ValueType::Min(lhs._y, rhs._y));
    }

    ///
    /// @brief Get maximum components.
    ///
    /// @param lhs Radius to compare
    /// @param rhs Radius to compare
    ///
    /// @return Radius of maximum components
    ///
    static inline constexpr auto Max(const Radius2& lhs, const Radius2& rhs) -> Radius2
    {
        return Radius2(ValueType::Max(lhs._x, rhs._x), ValueType::Max(lhs._y, rhs._y));
    }

    ///
    /// @brief Round components.
    ///
    /// @param radius Radius to round
    ///
    /// @return Rounded radius
    ///
    static inline constexpr auto Round(const Radius2& radius) -> Radius2
    {
        return Radius2(ValueType::Round(radius._x), ValueType::Round(radius._y));
    }

    ///
    /// @brief Floor components.
    ///
    /// @param radius Radius to floor
    ///
    /// @return Floored radius
    ///
    static inline constexpr auto Floor(const Radius2& radius) -> Radius2
    {
        return Radius2(ValueType::Floor(radius._x), ValueType::Floor(radius._y));
    }

    ///
    /// @brief Ceil components.
    ///
    /// @param radius Radius to ceil
    ///
    /// @return Ceiled radius
    ///
    static inline constexpr auto Ceil(const Radius2& radius) -> Radius2
    {
        return Radius2(ValueType::Ceil(radius._x), ValueType::Ceil(radius._y));
    }

public:
    ///
    /// @brief Default constructor.
    ///
    /// Initializes both X and Y values to zero.
    ///
    inline constexpr Radius2() = default;

    ///
    /// @brief Copy constructor.
    ///
    inline constexpr Radius2(const Radius2&) = default;

    ///
    /// @brief Copy assignment operator.
    ///
    inline constexpr auto operator=(const Radius2&) -> Radius2& = default;

    ///
    /// @brief Construct Radius2 from X and Y values.
    ///
    /// @param x X value of radius.
    /// @param y Y value of radius.
    ///
    template <class X, class Y>
    inline constexpr Radius2(const X& x, const Y& y)
      : _x {x}
      , _y {y}
    {
    }

    ///
    /// @brief Get X.
    ///
    inline constexpr auto GetX() const noexcept -> const ValueType&
    {
        return _x;
    }

    ///
    /// @brief Set X.
    ///
    inline constexpr auto SetX(const ValueType& x) noexcept -> void
    {
        _x = x;
    }

    ///
    /// @brief Get Y.
    ///
    inline constexpr auto GetY() const noexcept -> const ValueType&
    {
        return _y;
    }

    ///
    /// @brief Set Y.
    ///
    inline constexpr auto SetY(const ValueType& y) noexcept -> void
    {
        _y = y;
    }

    ///
    /// @brief Returns true if components are finite.
    ///
    inline constexpr auto IsFinite() const noexcept -> Bool
    {
        return ValueType::IsFinite(_x) && ValueType::IsFinite(_y);
    }

    ///
    /// @brief Convert Radius to other geometry types.
    ///
    template <template <class> class U>
    auto As() const -> U<ValueType>
    {
        return U<ValueType>(_x, _y);
    }

    friend inline constexpr bool operator==(const Radius2& l, const Radius2& r) = default;
    friend inline constexpr bool operator!=(const Radius2& l, const Radius2& r) = default;

private:
    ValueType _x = static_cast<T>(0);
    ValueType _y = static_cast<T>(0);
};
}
}

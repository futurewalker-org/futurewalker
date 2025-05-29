// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Geometry.PointType.hpp"
#include "Futurewalker.Geometry.Offset.hpp"

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief A specialization of Point2 for Float types.
///
/// @tparam T Floating point type.
/// @tparam Tag Tag type.
///
template <Concepts::FloatingPoint T, class Tag>
class Point2<Float<T, Tag>>
{
public:
    ///
    /// @brief Value type
    ///
    using ValueType = Float<T, Tag>;

    ///
    /// @brief Returns true if two points are nearly equal
    ///
    /// @param lhs Point to compare
    /// @param rhs Point to compare
    /// @param tolerance Absolute tolerance
    ///
    static inline constexpr Bool IsNearlyEquals(Point2 const& lhs, Point2 const& rhs, T const& tolerance = 1e-6) noexcept
    {
        return ValueType::IsNearlyEqual(lhs._x, rhs._x, tolerance) && ValueType::IsNearlyEqual(lhs._y, rhs._y, tolerance);
    }

    ///
    /// @brief Returns true if two points are nearly equal
    ///
    /// @param lhs Point to compare
    /// @param rhs Point to compare
    /// @param precision Relative precision
    /// @param minTolerance Absolute minimum tolerance
    ///
    static inline constexpr Bool IsNearlyEqualRelative(Point2 const& lhs, Point2 const& rhs, T const& precision = 1e-6, T const& minTolerance = 1e-6) noexcept
    {
        return ValueType::IsNearlyEqualRelative(lhs._x, rhs._y, precision, minTolerance) && ValueType::IsNearlyEqualRelative(lhs._x, rhs._y, precision, minTolerance);
    }

    ///
    /// @brief Calculate distance between two points
    ///
    /// @param lhs Point to reference
    /// @param rhs Point to reference
    ///
    /// @return Euclidean distance between two points
    ///
    static inline constexpr auto DistanceBetween(Point2 const& lhs, Point2 const& rhs)
    {
        return ValueType::Sqrt(ValueType::Pow(rhs._x - lhs._x, 2) + ValueType::Pow(rhs._y - lhs._y, 2));
    }

    ///
    /// @brief Get minimum components.
    ///
    /// @param lhs Point to compare
    /// @param rhs Point to compare
    ///
    /// @return Point of minimum components
    ///
    static inline constexpr auto Min(Point2 const& lhs, Point2 const& rhs) -> Point2
    {
        return Point2(ValueType::Min(lhs._x, rhs._x), ValueType::Min(lhs._y, rhs._y));
    }

    ///
    /// @brief Get maximum components.
    ///
    /// @param lhs Point to compare
    /// @param rhs Point to compare
    ///
    /// @return Point of maximum components
    ///
    static inline constexpr auto Max(Point2 const& lhs, Point2 const& rhs) -> Point2
    {
        return Point2(ValueType::Max(lhs._x, rhs._x), ValueType::Max(lhs._y, rhs._y));
    }

    ///
    /// @brief Round components.
    ///
    /// @param point Point to round
    ///
    /// @return Rounded point
    ///
    static inline constexpr auto Round(Point2 const& point) -> Point2
    {
        return Point2(ValueType::Round(point._x), ValueType::Round(point._y));
    }

    ///
    /// @brief Floor components.
    ///
    /// @param point Point to floor
    ///
    /// @return Floored point
    ///
    static inline constexpr auto Floor(Point2 const& point) -> Point2
    {
        return Point2(ValueType::Floor(point._x), ValueType::Floor(point._y));
    }

    ///
    /// @brief Ceil components.
    ///
    /// @param point Point to ceil
    ///
    /// @return Ceiled point
    ///
    static inline constexpr auto Ceil(Point2 const& point) -> Point2
    {
        return Point2(ValueType::Ceil(point._x), ValueType::Ceil(point._y));
    }

public:
    ///
    /// @brief Default constructor.
    ///
    /// Initializes both X and Y values to zero.
    ///
    inline constexpr Point2() = default;

    ///
    /// @brief Copy constructor.
    ///
    inline constexpr Point2(Point2 const&) = default;

    ///
    /// @brief Copy assignment operator.
    ///
    inline constexpr auto operator=(Point2 const&) -> Point2& = default;

    ///
    /// @brief Construct Point2 from X and Y values.
    ///
    /// @param x X value of point.
    /// @param y Y value of point.
    ///
    template <class X, class Y>
    inline constexpr Point2(const X& x, const Y& y)
      : _x {x}
      , _y {y}
    {
    }

    ///
    /// @brief
    ///
    /// @return
    ///
    inline constexpr auto GetX() const noexcept -> ValueType const&
    {
        return _x;
    }

    ///
    /// @brief
    ///
    /// @return
    ///
    inline constexpr auto SetX(ValueType const& x) noexcept -> void
    {
        _x = x;
    }

    ///
    /// @brief
    ///
    /// @return
    ///
    inline constexpr auto GetY() const noexcept -> ValueType const&
    {
        return _y;
    }

    ///
    /// @brief
    ///
    /// @return
    ///
    inline constexpr auto SetY(ValueType const& y) noexcept -> void
    {
        _y = y;
    }

    ///
    /// @brief
    ///
    inline constexpr auto IsFinite() const noexcept -> Bool
    {
        return ValueType::IsFinite(_x) && ValueType::IsFinite(_y);
    }

    ///
    /// @brief operator==
    ///
    friend inline constexpr bool operator==(Point2 const& l, Point2 const& r)
    {
        return (l._x == r._x) && (l._y == r._y);
    }

    ///
    /// @brief operator!=
    ///
    friend inline constexpr bool operator!=(Point2 const& l, Point2 const& r)
    {
        return (l._x != r._x) || (l._y != r._y);
    }

    ///
    /// @brief Offset point by amount.
    ///
    /// @param off Amount of offset.
    ///
    inline constexpr auto operator+=(Offset2<ValueType> const& off) noexcept -> Point2&
    {
        _x += off.GetDeltaX();
        _y += off.GetDeltaY();
        return *this;
    }

    ///
    /// @brief Offset point by amount.
    ///
    /// @param off Amount of offset.
    ///
    inline constexpr auto operator-=(Offset2<ValueType> const& off) noexcept -> Point2&
    {
        _x -= off.GetDeltaX();
        _y -= off.GetDeltaY();
        return *this;
    }

    friend inline constexpr auto operator+(Point2 const& l, Offset2<ValueType> const& r) noexcept
    {
        auto tmp = l;
        tmp += r;
        return tmp;
    }

    friend inline constexpr auto operator-(Point2 const& l, Offset2<ValueType> const& r) noexcept
    {
        auto tmp = l;
        tmp -= r;
        return tmp;
    }

    ///
    /// @brief Convert Point to other geometry types.
    ///
    template <template <class> class U>
    auto As() const -> U<ValueType>
    {
        return U<ValueType>(_x, _y);
    }

private:
    ValueType _x = static_cast<T>(0);
    ValueType _y = static_cast<T>(0);
};
}
}

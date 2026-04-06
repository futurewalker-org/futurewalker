// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Geometry.PointType.hpp"
#include "Futurewalker.Geometry.Vector.hpp"

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
struct Point2<Float<T, Tag>>
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
        return ValueType::IsNearlyEqual(lhs.x, rhs.x, tolerance) && ValueType::IsNearlyEqual(lhs.y, rhs.y, tolerance);
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
        return ValueType::IsNearlyEqualRelative(lhs.x, rhs.x, precision, minTolerance) && ValueType::IsNearlyEqualRelative(lhs.y, rhs.y, precision, minTolerance);
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
        return ValueType::Sqrt(ValueType::Pow(rhs.x - lhs.x, 2) + ValueType::Pow(rhs.y - lhs.y, 2));
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
        return Point2(ValueType::Min(lhs.x, rhs.x), ValueType::Min(lhs.y, rhs.y));
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
        return Point2(ValueType::Max(lhs.x, rhs.x), ValueType::Max(lhs.y, rhs.y));
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
        return Point2(ValueType::Round(point.x), ValueType::Round(point.y));
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
        return Point2(ValueType::Floor(point.x), ValueType::Floor(point.y));
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
        return Point2(ValueType::Ceil(point.x), ValueType::Ceil(point.y));
    }

public:
    ///
    /// @brief
    ///
    inline constexpr auto IsFinite() const noexcept -> Bool
    {
        return ValueType::IsFinite(x) && ValueType::IsFinite(y);
    }

    ///
    /// @brief operator==
    ///
    friend inline constexpr bool operator==(Point2 const& l, Point2 const& r)
    {
        return (l.x == r.x) && (l.y == r.y);
    }

    ///
    /// @brief operator!=
    ///
    friend inline constexpr bool operator!=(Point2 const& l, Point2 const& r)
    {
        return (l.x != r.x) || (l.y != r.y);
    }

    ///
    /// @brief Offset point by amount.
    ///
    /// @param off Amount of offset.
    ///
    inline constexpr auto operator+=(Vector2<ValueType> const& off) noexcept -> Point2&
    {
        x += off.x;
        y += off.y;
        return *this;
    }

    ///
    /// @brief Offset point by amount.
    ///
    /// @param off Amount of offset.
    ///
    inline constexpr auto operator-=(Vector2<ValueType> const& off) noexcept -> Point2&
    {
        x -= off.x;
        y -= off.y;
        return *this;
    }

    friend inline constexpr auto operator+(Point2 const& l, Vector2<ValueType> const& r) noexcept
    {
        auto tmp = l;
        tmp += r;
        return tmp;
    }

    friend inline constexpr auto operator-(Point2 const& l, Vector2<ValueType> const& r) noexcept
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
        return U<ValueType>(x, y);
    }

    ///
    /// @brief X component.
    ///
    ValueType x = static_cast<T>(0);

    ///
    /// @brief Y component.
    ///
    ValueType y = static_cast<T>(0);
};
}
}

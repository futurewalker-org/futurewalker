// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Geometry.RadiusType.hpp"

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
struct Radius2<Float<T, Tag>>
{
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
    static inline constexpr Bool IsNearlyEquals(Radius2 const& lhs, Radius2 const& rhs, T const& tolerance = 1e-6) noexcept
    {
        return ValueType::IsNearlyEqual(lhs.x, rhs.x, tolerance) && ValueType::IsNearlyEqual(lhs.y, rhs.y, tolerance);
    }

    ///
    /// @brief Returns true if two values are nearly equal
    ///
    /// @param lhs Radius to compare
    /// @param rhs Radius to compare
    /// @param precision Relative precision
    /// @param minTolerance Absolute minimum tolerance
    ///
    static inline constexpr Bool IsNearlyEqualRelative(Radius2 const& lhs, Radius2 const& rhs, T const& precision = 1e-6, T const& minTolerance = 1e-6) noexcept
    {
        return ValueType::IsNearlyEqualRelative(lhs.x, rhs.x, precision, minTolerance) && ValueType::IsNearlyEqualRelative(lhs.y, rhs.y, precision, minTolerance);
    }

    ///
    /// @brief Get minimum components.
    ///
    /// @param lhs Radius to compare
    /// @param rhs Radius to compare
    ///
    /// @return Radius of minimum components
    ///
    static inline constexpr auto Min(Radius2 const& lhs, Radius2 const& rhs) -> Radius2
    {
        return Radius2(ValueType::Min(lhs.x, rhs.x), ValueType::Min(lhs.y, rhs.y));
    }

    ///
    /// @brief Get maximum components.
    ///
    /// @param lhs Radius to compare
    /// @param rhs Radius to compare
    ///
    /// @return Radius of maximum components
    ///
    static inline constexpr auto Max(Radius2 const& lhs, Radius2 const& rhs) -> Radius2
    {
        return Radius2(ValueType::Max(lhs.x, rhs.x), ValueType::Max(lhs.y, rhs.y));
    }

    ///
    /// @brief Round components.
    ///
    /// @param radius Radius to round
    ///
    /// @return Rounded radius
    ///
    static inline constexpr auto Round(Radius2 const& radius) -> Radius2
    {
        return Radius2(ValueType::Round(radius.x), ValueType::Round(radius.y));
    }

    ///
    /// @brief Floor components.
    ///
    /// @param radius Radius to floor
    ///
    /// @return Floored radius
    ///
    static inline constexpr auto Floor(Radius2 const& radius) -> Radius2
    {
        return Radius2(ValueType::Floor(radius.x), ValueType::Floor(radius.y));
    }

    ///
    /// @brief Ceil components.
    ///
    /// @param radius Radius to ceil
    ///
    /// @return Ceiled radius
    ///
    static inline constexpr auto Ceil(Radius2 const& radius) -> Radius2
    {
        return Radius2(ValueType::Ceil(radius.x), ValueType::Ceil(radius.y));
    }

    ///
    /// @brief Returns true if components are finite.
    ///
    inline constexpr auto IsFinite() const noexcept -> Bool
    {
        return ValueType::IsFinite(x) && ValueType::IsFinite(y);
    }

    ///
    /// @brief Convert Radius to other geometry types.
    ///
    template <template <class> class U>
    auto As() const -> U<ValueType>
    {
        return U<ValueType>(x, y);
    }

    friend inline constexpr bool operator==(Radius2 const& l, Radius2 const& r) = default;
    friend inline constexpr bool operator!=(Radius2 const& l, Radius2 const& r) = default;

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

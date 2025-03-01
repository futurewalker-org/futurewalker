// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.NonConstructible.hpp"
#include "Futurewalker.Core.Primitive.Boolean.hpp"

#include <cmath>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Math functions.
///
/// Provides basic math functions for primitive types.
///
class Math : NonConstructible
{
public:
    template <Concepts::FloatingPoint T>
    static inline constexpr Bool IsFinite(const T& value) noexcept;

    template <Concepts::FloatingPoint T>
    static inline constexpr Bool IsInfinite(const T& value) noexcept;

    template <Concepts::FloatingPoint T>
    static inline constexpr Bool IsNaN(const T& value) noexcept;

    template <Concepts::FloatingPoint T>
    static inline constexpr auto Trunc(const T& value) noexcept -> T;

    template <Concepts::FloatingPoint T>
    static inline constexpr auto Floor(const T& value) noexcept -> T;

    template <Concepts::FloatingPoint T>
    static inline constexpr auto Ceil(const T& value) noexcept -> T;

    template <Concepts::FloatingPoint T>
    static inline constexpr auto Round(const T& value) noexcept -> T;

    template <Concepts::FloatingPoint T>
    static inline constexpr auto Abs(const T& value) noexcept -> T;

    template <Concepts::FloatingPoint T>
    static inline constexpr auto Sqrt(const T& value) noexcept -> T;

    template <Concepts::FloatingPoint T>
    static inline constexpr auto Pow(const T& base, const T& exponent) noexcept -> T;

    template <Concepts::FloatingPoint T>
    static inline constexpr Bool IsNearlyEqual(const T& lhs, const T& rhs, const T& tolerance = 1e-6) noexcept;

    template <Concepts::FloatingPoint T>
    static inline constexpr Bool IsNearlyEqualRelative(const T& lhs, const T& rhs, const T& precision = 1e-6, const T& minTolerance = 1e-6) noexcept;
};

///
/// @brief Check if floating point value is finite.
///
/// Do not rely on value of `errno` after calling this function.
///
/// @tparam T Floating point type
///
/// @param value Floating point value
///
/// @return `true` when `value` is finite, otherwise `false`.
///
template <Concepts::FloatingPoint T>
inline constexpr Bool Math::IsFinite(const T& value) noexcept
{
    return std::isfinite(value);
}

///
/// @brief Check if floating point value is infinite.
///
/// @tparam T Floating point type
///
/// @param value Floating point type
///
/// @return `true` when `value` is infinite, otherwise `false`.
///
template <Concepts::FloatingPoint T>
inline constexpr Bool Math::IsInfinite(const T& value) noexcept
{
    return std::isinf(value);
}

///
/// @brief Check if floating point value is not-a-number.
///
/// Do not rely on value of `errno` after calling this function.
///
/// @tparam T Floating point type
///
/// @param value Floating point value
///
/// @return `true` when `value` is not-a-number, otherwise `false`.W
///
template <Concepts::FloatingPoint T>
inline constexpr Bool Math::IsNaN(const T& value) noexcept
{
    return std::isnan(value);
}

///
/// @brief Truncate floating point value.
///
/// @tparam T Floating point type.
///
/// @param value Floating point value.
///
/// @return Truncated value of `value`.
///
template <Concepts::FloatingPoint T>
inline constexpr auto Math::Trunc(const T& value) noexcept -> T
{
    if (IsFinite(value))
    {
        return static_cast<T>(static_cast<int64_t>(value));
    }
    return value;
}

///
/// @brief Floor floating point value.
///
/// @tparam T Floating point type
///
/// @param value Floating point value
///
/// @return Floored value of `value`.
///
template <Concepts::FloatingPoint T>
inline constexpr auto Math::Floor(const T& value) noexcept -> T
{
    if (IsFinite(value))
    {
        const auto digit = Trunc(value);
        return ((value - digit) < static_cast<T>(0.0)) ? digit - static_cast<T>(1.0) : digit;
    }
    return value;
}

///
/// @brief Ceil floating point value.
///
/// @tparam T Floating point type.
///
/// @param value Floating point value.
///
/// @return Ceiled value of `value`.
///
template <Concepts::FloatingPoint T>
inline constexpr auto Math::Ceil(const T& value) noexcept -> T
{
    if (IsFinite(value))
    {
        const auto digit = Trunc(value);
        return ((value - digit) <= static_cast<T>(0.0)) ? digit : digit + static_cast<T>(1.0);
    }
    return value;
}

///
/// @brief Round floating point value.
///
/// @tparam T Floating point type.
///
/// @param value Floating point value.
///
/// @return Rounded value of `value`.
///
template <Concepts::FloatingPoint T>
inline constexpr auto Math::Round(const T& value) noexcept -> T
{
    if (IsFinite(value))
    {
        const auto floor = Floor(value);
        if ((value - floor) < static_cast<T>(0.5))
        {
            return floor;
        }
        return Ceil(value);
    }
    return value;
}

///
/// @brief Get absolute value of floating point value.
///
/// Do not rely on value of `errno` after calling this function.
///
/// @tparam T Floating point type.
///
/// @param value Floating point value.
///
/// @return Absolute value of `value`.
///
template <Concepts::FloatingPoint T>
inline constexpr auto Math::Abs(const T& value) noexcept -> T
{
    return std::abs(value);
}

///
/// @brief Get square root of floating point value. 
///
/// @tparam T Floating point type.
///
/// @param value Floating point value.
///
/// @return Square root of `value`.
///
template <Concepts::FloatingPoint T>
inline constexpr auto Math::Sqrt(const T& value) noexcept -> T
{
    return std::sqrt(value);
}

///
/// @brief Compute power of floating point value.
///
/// @tparam T Floating point type.
///
/// @param base Floating point value.
/// @param exponent Floating point value.
///
/// @return `base` raised by power of `exponent`.
///
template <Concepts::FloatingPoint T>
inline constexpr auto Math::Pow(const T& base, const T& exponent) noexcept -> T
{
    return std::pow(base, exponent);
}

///
/// @brief Check if two floating point values are equal within absolute tolerance.
///
/// Compares two floating point values by absolute tolerance.
/// When either `lhs` or `rhs` are very large compared to `tolerance`, this function effectively becomes a direct comparision of two floating point values.
/// If you want to avoid this, compare values with relative precision i.e. `IsNearlyEqualRelative()`.
///
/// @tparam T Floating point type.
///
/// @param lhs Floating point value.
/// @param rhs Floating point value.
/// @param tolerance Absolute tolerance of comparison.
///
/// @return `true` if `lhs` and `rhs` are equal within `tolerance`, otherwise `false`.
///
template <Concepts::FloatingPoint T>
inline constexpr Bool Math::IsNearlyEqual(const T& lhs, const T& rhs, const T& tolerance) noexcept
{
    if (rhs == lhs)
    {
        return true;
    }
    return Abs(rhs - lhs) <= tolerance;
}

///
/// @brief Check if two floating point values are equal within relative precision.
///
/// Compares two floating point values by relative tolerance based on maximum absolute value of `lhs` and `rhs`.
/// When either `lhs` or `rhs` are close to zero, comparision based on relative precision have to be infinitely precise, thus effectively becomes a direct comparision.
/// To avoid this issue, values within absolute tolerance `minTolerance` are always considered equal.
///
/// @tparam T Floating point type.
///
/// @param lhs Floating point value.
/// @param rhs Floating point value.
/// @param precision Precision of relative tolerance. (0.01=>1%)
/// @param minTolerance Minimum absolute tolerance of comparision.
///
/// @return `true` if `lhs` and `rhs` are equal within relative `precision`, otherwise `false`.
///
template <Concepts::FloatingPoint T>
inline constexpr Bool Math::IsNearlyEqualRelative(const T& lhs, const T& rhs, const T& precision, const T& minTolerance) noexcept
{
    const auto l = Abs(lhs);
    const auto r = Abs(rhs);
    const auto diff = Abs(rhs - lhs);

    if (lhs == rhs)
    {
        return true;
    }
    else if (diff <= minTolerance)
    {
        return true;
    }
    return diff <= std::max(l, r) * precision;
}
}
}

// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Primitive.FloatType.hpp"
#include "Futurewalker.Core.Primitive.IntegerType.hpp"
#include "Futurewalker.Core.Primitive.BooleanType.hpp"
#include "Futurewalker.Core.Concepts.hpp"
#include "Futurewalker.Core.Math.hpp"
#include "Futurewalker.Core.Utility.hpp" 

#include <boost/operators.hpp>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Safe wrapper for builtin float types.
///
template <Concepts::FloatingPoint T, class Tag>
// clang-format off
class Float final
  : boost::additive<Float<T, Tag>,
    boost::multiplicative<Float<T, Tag>,
    boost::incrementable<Float<T, Tag>,
    boost::decrementable<Float<T, Tag>
    >>>>
// clang-format on
{
    template <Concepts::FloatingPoint T_, class Tag_>
    friend class Float;

public:
    ///
    /// @brief Value type.
    ///
    using ValueType = T;

    ///
    /// @brief Tag type.
    ///
    using TagType = Tag;

    ///
    /// @brief Get positive infinity value.
    ///
    static inline constexpr Float Infinity() noexcept
    {
        return std::numeric_limits<T>::infinity();
    }

    ///
    /// @brief Get quiet NaN value.
    ///
    static inline constexpr Float NaN() noexcept
    {
        return std::numeric_limits<T>::quiet_NaN();
    }

    ///
    /// @brief Get machine epsilon value.
    ///
    static inline constexpr Float Epsilon() noexcept
    {
        return std::numeric_limits<T>::epsilon();
    }

    ///
    /// @brief Returns true if given value is finite.
    ///
    /// @param value Value to test
    ///
    static inline constexpr Bool IsFinite(Float const& value) noexcept
    {
        return Math::IsFinite(value._value);
    }

    ///
    /// @brief Returns true if given value is infinite.
    ///
    /// @param value Value to test
    ///
    static inline constexpr Bool IsInfinite(Float const& value) noexcept
    {
        return Math::IsInfinite(value._value);
    }

    ///
    /// @brief Returns true if given value is NaN.
    ///
    /// @param value Value to test
    ///
    static inline constexpr Bool IsNaN(Float const& value) noexcept
    {
        return Math::IsNaN(value._value);
    }

    ///
    /// @brief Truncate value.
    ///
    /// @param value Value to truncate
    ///
    static inline constexpr auto Trunc(Float const& value) -> Float
    {
        return Math::Trunc(value._value);
    }

    ///
    /// @brief Floor value.
    ///
    /// @param value Value to floor
    ///
    static inline constexpr auto Floor(Float const& value) -> Float
    {
        return Math::Floor(value._value);
    }

    ///
    /// @brief Ceil value.
    ///
    /// @param value Value to ceil
    ///
    static inline constexpr auto Ceil(Float const& value) -> Float
    {
        return Math::Ceil(value._value);
    }

    ///
    /// @brief Round value.
    ///
    /// @param value Value to round.
    ///
    static inline constexpr auto Round(Float const& value) -> Float
    {
        return Math::Round(value._value);
    }

    ///
    /// @brief Get absolute value.
    ///
    /// @param value Vaule
    ///
    static inline constexpr auto Abs(Float const& value) -> Float
    {
        return Math::Abs(value._value);
    }

    ///
    /// @brief Get sqare root of value.
    ///
    /// @param value Value
    ///
    static inline constexpr auto Sqrt(Float const& value) -> Float
    {
        return Math::Sqrt(value._value);
    }

    ///
    /// @brief Get power of value.
    ///
    /// @param base Base value
    /// @param exponent Exponent value
    ///
    static inline constexpr auto Pow(Float const& base, ValueType const& exponent) -> Float
    {
        return Math::Pow(base._value, exponent);
    }

    ///
    /// @brief Check if two floating point values are equal within absolute tolerance.
    ///
    /// @param lhs Lhs
    /// @param rhs Rhs
    /// @param tolerance Absolute tolerance
    ///
    static inline constexpr auto IsNearlyEqual(Float const& lhs, Float const& rhs, Float<T, void> const& tolerance = 1e-6) noexcept -> Bool
    {
        return Math::IsNearlyEqual(lhs._value, rhs._value, tolerance._value);
    }

    ///
    /// @brief Check if two floating point values are equal within relative precision.
    ///
    /// @param lhs Lhs
    /// @param rhs Rhs
    /// @param precision Relative precision
    /// @param minTolerance Minimum absolute tolerance
    ///
    static inline constexpr auto IsNearlyEqualRelative(Float const& lhs, Float const& rhs, Float<T, void> const& precision = 1e-6, Float<T, void> const& minTolerance = 1e-6) noexcept -> Bool
    {
        return Math::IsNearlyEqualRelative(lhs._value, rhs._value, precision._value, minTolerance._value);
    }

    ///
    /// @brief Get ordered pair of (min, max) from two values.
    ///
    /// @param a Value to compare.
    /// @param b Value to compare.
    ///
    /// @return (min, max) pair
    ///
    static inline constexpr auto MinMax(Float const& a, Float const& b) -> std::pair<Float, Float>
    {
        return Utility::MinMax(a, b);
    }

    ///
    /// @brief Get larger value from two values.
    ///
    /// @param a Value to compare
    /// @param b Value to compare
    ///
    static inline constexpr auto Max(Float const& a, Float const& b) -> Float
    {
        return Utility::Max(a, b);
    }

    ///
    /// @brief Return smaller value from two values.
    ///
    /// @param a Value to compare
    /// @param b Value to compare
    ///
    static inline constexpr auto Min(Float const& a, Float const& b) -> Float
    {
        return Utility::Min(a, b);
    }

    ///
    /// @brief Clamp value between min and max.
    ///
    /// @param val Value to clamp
    /// @param min Minimum value
    /// @param max Maximum value
    ///
    static inline constexpr auto Clamp(Float const& val, Float const& min, Float const& max) -> Float
    {
        return Utility::Clamp(val, min, max);
    }

public:
    inline constexpr Float() = delete;
    inline constexpr Float(Float const&) = default;
    inline constexpr Float& operator=(Float const&) = default;

    ///
    /// @brief Construct from float value.
    ///
    /// @param[in] value An float value. 
    ///
    template <class U>
    inline constexpr explicit(!Concepts::NarrowConvertibleTo<U, T>) Float(U const& value) noexcept
      : _value {static_cast<T>(value)}
    {
    }

    ///
    /// @brief Construct from float value.
    ///
    /// @param[in] value An float value.
    ///
    template <class U>
    inline constexpr explicit(!Concepts::NarrowConvertibleTo<U, T>) Float(Float<U, Tag> const& value) noexcept
      : _value {static_cast<T>(value._value)}
    {
    }

    ///
    /// @brief Construct from integer value.
    ///
    /// @param[in] value An integer value.
    ///
    template <class U>
    inline constexpr explicit(!Concepts::NarrowConvertibleTo<U, T>) Float(Integer<U, Tag> const& value) noexcept
      : _value {static_cast<T>(value)}
    {
    }

    ///
    /// @brief Convert Float to underlying type.
    ///
    template <class U>
    requires Concepts::ImplicitlyConvertibleTo<T, U>
    inline constexpr explicit operator U() const noexcept
    {
        return static_cast<U>(_value);
    }

public:
    inline constexpr auto operator+=(Float const& x) noexcept -> Float&
    {
        _value += x._value;
        return *this;
    }

    inline constexpr auto operator-=(Float const& x) noexcept -> Float&
    {
        _value -= x._value;
        return *this;
    }

    inline constexpr auto operator*=(Float const& x) noexcept -> Float&
    {
        _value *= x._value;
        return *this;
    }

    inline constexpr auto operator/=(Float const& x) noexcept -> Float&
    {
        _value /= x._value;
        return *this;
    }

    inline constexpr auto operator%=(Float const& x) noexcept -> Float&
    {
        _value %= x._value;
        return *this;
    }

    inline constexpr auto operator++() noexcept -> Float&
    {
        ++_value;
        return *this;
    }

    inline constexpr auto operator--() noexcept -> Float&
    {
        --_value;
        return *this;
    }

    inline constexpr auto operator-() const noexcept -> Float
    {
        return Float(-_value);
    }

    inline constexpr auto operator+() const noexcept -> Float
    {
        return Float(+_value);
    }

    friend inline constexpr auto operator<=>(Float const& l, Float const& r) noexcept
    {
        return l._value <=> r._value;
    }

    friend inline constexpr bool operator==(Float const& l, Float const& r) noexcept
    {
        return l._value == r._value;
    }

private:
    T _value; ///< Floating point value.
};
}
}
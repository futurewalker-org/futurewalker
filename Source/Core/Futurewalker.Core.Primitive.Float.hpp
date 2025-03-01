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
    /// @brief 
    ///
    /// @param value 
    ///
    /// @return 
    ///
    static inline constexpr Bool IsFinite(const Float& value)
    {
        return Math::IsFinite(value._value);
    }

    ///
    /// @brief 
    ///
    /// @param value 
    /// @return 
    ///
    static inline constexpr Bool IsInfinite(const Float& value)
    {
        return Math::IsInfinite(value._value);
    }

    ///
    /// @brief 
    ///
    /// @param value 
    ///
    /// @return 
    ///
    static inline constexpr Bool IsNaN(const Float& value)
    {
        return Math::IsNaN(value._value);
    }

    ///
    /// @brief
    ///
    /// @param value
    ///
    /// @return
    ///
    static inline constexpr auto Trunc(const Float& value) -> Float
    {
        return Math::Trunc(value._value);
    }

    ///
    /// @brief
    ///
    /// @param value
    ///
    /// @return
    ///
    static inline constexpr auto Floor(const Float& value) -> Float
    {
        return Math::Floor(value._value);
    }

    ///
    /// @brief
    ///
    /// @param value
    ///
    /// @return
    ///
    static inline constexpr auto Ceil(const Float& value) -> Float
    {
        return Math::Ceil(value._value);
    }

    ///
    /// @brief
    ///
    /// @param value
    ///
    /// @return
    ///
    static inline constexpr auto Round(const Float& value) -> Float
    {
        return Math::Round(value._value);
    }

    ///
    /// @brief 
    ///
    /// @param value 
    ///
    /// @return 
    ///
    static inline constexpr auto Abs(const Float& value) -> Float
    {
        return Math::Abs(value._value);
    }

    ///
    /// @brief 
    ///
    /// @param value 
    ///
    /// @return 
    ///
    static inline constexpr auto Sqrt(const Float& value) -> Float
    {
        return Math::Sqrt(value._value);
    }

    ///
    /// @brief 
    ///
    /// @param base 
    /// @param exponent 
    ///
    /// @return 
    ///
    static inline constexpr auto Pow(const Float& base, const ValueType& exponent) -> Float
    {
        return Math::Pow(base._value, exponent);
    }

    ///
    /// @brief 
    ///
    /// @param lhs 
    /// @param rhs 
    /// @param tolerance 
    ///
    /// @return 
    ///
    static inline constexpr auto IsNearlyEqual(const Float& lhs, const Float& rhs, const Float<T, void>& tolerance = 1e-6) noexcept -> Bool
    {
        return Math::IsNearlyEqual(lhs._value, rhs._value, tolerance._value);
    }

    ///
    /// @brief 
    ///
    /// @param lhs 
    /// @param rhs 
    /// @param precision 
    /// @param minTolerance 
    ///
    /// @return 
    ///
    static inline constexpr auto IsNearlyEqualRelative(const Float& lhs, const Float& rhs, const Float<T, void>& precision = 1e-6, const Float<T, void>& minTolerance = 1e-6) noexcept -> Bool
    {
        return Math::IsNearlyEqualRelative(lhs._value, rhs._value, precision._value, minTolerance._value);
    }

    ///
    /// @brief 
    ///
    /// @param a 
    /// @param b 
    ///
    /// @return 
    ///
    static inline constexpr auto MinMax(const Float& a, const Float& b) -> std::pair<Float, Float>
    {
        return Utility::MinMax(a, b);
    }

    ///
    /// @brief 
    ///
    /// @param a 
    /// @param b 
    /// @return 
    ///
    static inline constexpr auto Max(const Float& a, const Float& b) -> Float
    {
        return Utility::Max(a, b);
    }

    ///
    /// @brief 
    ///
    /// @param a 
    /// @param b 
    ///
    /// @return 
    ///
    static inline constexpr auto Min(const Float& a, const Float& b) -> Float
    {
        return Utility::Min(a, b);
    }

    ///
    /// @brief 
    ///
    /// @param val 
    /// @param min 
    /// @param max 
    ///
    /// @return 
    ///
    static inline constexpr auto Clamp(const Float& val, const Float& min, const Float& max) -> Float
    {
        return Utility::Clamp(val, min, max);
    }

public:
    inline constexpr Float() = delete;
    inline constexpr Float(const Float&) = default;
    inline constexpr Float& operator=(const Float&) = default;

    ///
    /// @brief Construct from float value.
    ///
    /// @param[in] value An float value. 
    ///
    template <class U>
    inline constexpr explicit(!Concepts::NarrowConvertibleTo<U, T>) Float(const U& value) noexcept
      : _value {static_cast<T>(value)}
    {
    }

    ///
    /// @brief Construct from float value.
    ///
    /// @param[in] value An float value.
    ///
    template <class U>
    inline constexpr explicit(!Concepts::NarrowConvertibleTo<U, T>) Float(const Float<U, Tag>& value) noexcept
      : _value {static_cast<T>(value._value)}
    {
    }

    ///
    /// @brief Construct from integer value.
    ///
    /// @param[in] value An integer value.
    ///
    template <class U>
    inline constexpr explicit(!Concepts::NarrowConvertibleTo<U, T>) Float(const Integer<U, Tag>& value) noexcept
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
    inline constexpr auto operator+=(const Float& x) noexcept -> Float&
    {
        _value += x._value;
        return *this;
    }

    inline constexpr auto operator-=(const Float& x) noexcept -> Float&
    {
        _value -= x._value;
        return *this;
    }

    inline constexpr auto operator*=(const Float& x) noexcept -> Float&
    {
        _value *= x._value;
        return *this;
    }

    inline constexpr auto operator/=(const Float& x) noexcept -> Float&
    {
        _value /= x._value;
        return *this;
    }

    inline constexpr auto operator%=(const Float& x) noexcept -> Float&
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

    friend inline constexpr auto operator<=>(const Float& l, const Float& r) noexcept
    {
        return l._value <=> r._value;
    }

    friend inline constexpr bool operator==(const Float& l, const Float& r) noexcept
    {
        return l._value == r._value;
    }

private:
    T _value; ///< Floating point value.
};
}
}
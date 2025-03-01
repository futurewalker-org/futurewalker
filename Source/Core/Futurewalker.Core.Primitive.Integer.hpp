// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Primitive.IntegerType.hpp"
#include "Futurewalker.Core.Primitive.FloatType.hpp"
#include "Futurewalker.Core.Concepts.hpp"
#include "Futurewalker.Core.Utility.hpp"

#include <boost/operators.hpp>
#include <boost/functional/hash.hpp>

#include <boost/safe_numerics/checked_integer.hpp>

#include <functional>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Safe wrapper for builtin integer types.
///
template <Concepts::Integral T, class Tag>
class Integer final
  : boost::integer_arithmetic<Integer<T, Tag>,
    boost::bitwise<Integer<T, Tag>,
    boost::unit_steppable<Integer<T, Tag>
    >>>
{
    template <Concepts::Integral T_, class Tag_>
    friend class Integer;

public:
    ///
    /// @brief
    ///
    using ValueType = T;

    ///
    /// @brief
    ///
    using TagType = Tag;

    ///
    /// @brief
    ///
    /// @param a
    /// @param b
    ///
    /// @return
    ///
    static inline constexpr auto MinMax(const Integer& a, const Integer& b) noexcept -> std::pair<Integer, Integer>
    {
        return Utility::MinMax(a, b);
    }

    ///
    /// @brief 
    ///
    /// @param a 
    /// @param b 
    ///
    /// @return 
    ///
    static inline constexpr auto Max(const Integer& a, const Integer& b) noexcept -> Integer
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
    static inline constexpr auto Min(const Integer& a, const Integer& b) noexcept -> Integer
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
    static inline constexpr auto Clamp(const Integer& val, const Integer& min, const Integer& max) -> Integer
    {
        return Utility::Clamp(val, min, max);
    }

public:
    inline constexpr Integer() = delete;
    inline constexpr Integer(const Integer&) = default;
    inline constexpr Integer& operator=(const Integer&) = default;

    ///
    /// @brief Construct from integer value.
    ///
    /// @param[in] value An integer value.
    ///
    template <class U>
    requires Concepts::ImplicitlyConvertibleTo<U, T>
    inline constexpr explicit(!Concepts::NarrowConvertibleTo<U, T>) Integer(const U& value) noexcept
      : _value {static_cast<T>(value)}
    {
    }

    ///
    /// @brief Construct from integer value.
    ///
    /// @param[in] value An integer value.
    ///
    template <class U>
    requires Concepts::ImplicitlyConvertibleTo<U, T>
    inline constexpr explicit(!Concepts::NarrowConvertibleTo<U, T>) Integer(const Integer<U, Tag>& value) noexcept
      : _value {static_cast<T>(value._value)}
    {
    }

    ///
    /// @brief Construct from floating point value.
    ///
    /// @param[in] value A floating point value.
    ///
    template <class U>
    requires Concepts::ImplicitlyConvertibleTo<U, T>
    inline constexpr explicit(!Concepts::NarrowConvertibleTo<U, T>) Integer(const Float<U, Tag>& value) noexcept
      : _value {static_cast<T>(value)}
    {
    }

    ///
    /// @brief Convert Integer to underlying type.
    ///
    template <class U>
    requires (Concepts::ExplicitlyConvertibleTo<T, U>)
    inline explicit constexpr operator U() const noexcept
    {
        return static_cast<U>(_value);
    }

public:
    inline constexpr Integer& operator+=(const Integer& x) noexcept
    {
        _value += x._value;
        return *this;
    }

    inline constexpr Integer& operator-=(const Integer& x) noexcept
    {
        _value -= x._value;
        return *this;
    }

    inline constexpr Integer& operator*=(const Integer& x) noexcept
    {
        _value *= x._value;
        return *this;
    }

    inline constexpr Integer& operator/=(const Integer& x) noexcept
    {
        _value /= x._value;
        return *this;
    }

    inline constexpr Integer& operator%=(const Integer& x) noexcept
    {
        _value %= x._value;
        return *this;
    }

    inline constexpr Integer& operator|=(const Integer& x) noexcept
    {
        _value |= x._value;
        return *this;
    }

    inline constexpr Integer& operator&=(const Integer& x) noexcept
    {
        _value &= x._value;
        return *this;
    }

    inline constexpr Integer& operator^=(const Integer& x) noexcept
    {
        _value ^= x._value;
        return *this;
    }

    inline constexpr Integer& operator++() noexcept
    {
        ++_value;
        return *this;
    }

    inline constexpr Integer& operator--() noexcept
    {
        --_value;
        return *this;
    }

    inline constexpr auto operator-() const noexcept -> Integer
    {
        return Integer(-_value);
    }

    inline constexpr auto operator+() const noexcept -> Integer
    {
        return Integer(+_value);
    }

    friend inline constexpr auto operator<=>(const Integer& l, const Integer& r) noexcept
    {
        return l._value <=> r._value;
    }

    friend inline constexpr bool operator==(const Integer& l, const Integer& r) noexcept
    {
        return l._value == r._value;
    }

private:
    T _value; ///< Integer value.
};
}
} 

///
/// @brief Hash support.
///
template <class T, class TagType>
struct std::hash<FW_NS::Integer<T, TagType>>
{
    constexpr auto operator()(const FW_NS::Integer<T, TagType>& v) const noexcept -> size_t
    {
        return std::hash<T>()(static_cast<T>(v));
    }
};

///
/// @brief Hash support.
///
template <class T, class TagType>
struct boost::hash<FW_NS::Integer<T, TagType>>
{
    constexpr auto operator()(const FW_NS::Integer<T, TagType>& v) const noexcept -> size_t
    {
        return std::hash<T>()(static_cast<T>(v));
    }
};

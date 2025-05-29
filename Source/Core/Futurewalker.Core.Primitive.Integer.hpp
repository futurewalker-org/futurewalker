// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Primitive.IntegerType.hpp"
#include "Futurewalker.Core.Primitive.FloatType.hpp"
#include "Futurewalker.Core.Concepts.hpp"
#include "Futurewalker.Core.Utility.hpp"

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
class Integer
{
    template <Concepts::Integral T_, class Tag_>
    friend class Integer;

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
    /// @brief Get minimum and maximum value pair.
    ///
    /// @param a Value to compare.
    /// @param b Value to compare.
    ///
    /// @return (min, max) pair.
    ///
    static inline constexpr auto MinMax(Integer const& a, Integer const& b) noexcept -> std::pair<Integer, Integer>
    {
        return Utility::MinMax(a, b);
    }

    ///
    /// @brief Get larger value.
    ///
    /// @param a Value to compare.
    /// @param b Value to compare.
    ///
    /// @return Larger value.
    ///
    static inline constexpr auto Max(Integer const& a, Integer const& b) noexcept -> Integer
    {
        return Utility::Max(a, b);
    }

    ///
    /// @brief Get smaller value.
    ///
    /// @param a Value to compare.
    /// @param b Value to compare.
    ///
    /// @return Smaller value.
    ///
    static inline constexpr auto Min(Integer const& a, Integer const& b) noexcept -> Integer
    {
        return Utility::Min(a, b);
    }

    ///
    /// @brief Clamp value.
    ///
    /// @param val Value to clamp.
    /// @param min Minimum value.
    /// @param max Maximum value.
    ///
    /// @return Clamped value.
    ///
    static inline constexpr auto Clamp(Integer const& val, Integer const& min, Integer const& max) -> Integer
    {
        return Utility::Clamp(val, min, max);
    }

public:
    inline constexpr Integer() = delete;
    inline constexpr Integer(Integer const&) = default;
    inline constexpr Integer& operator=(Integer const&) = default;

    ///
    /// @brief Construct from integer value.
    ///
    /// @param[in] value An integer value.
    ///
    template <class U>
    requires Concepts::ImplicitlyConvertibleTo<U, T>
    inline constexpr explicit(!Concepts::NarrowConvertibleTo<U, T>) Integer(U const& value) noexcept
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
    inline constexpr explicit(!Concepts::NarrowConvertibleTo<U, T>) Integer(Integer<U, Tag> const& value) noexcept
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
    inline constexpr explicit(!Concepts::NarrowConvertibleTo<U, T>) Integer(Float<U, Tag> const& value) noexcept
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
    inline constexpr Integer& operator+=(Integer const& x) noexcept
    {
        _value += x._value;
        return *this;
    }

    inline constexpr Integer& operator-=(Integer const& x) noexcept
    {
        _value -= x._value;
        return *this;
    }

    inline constexpr Integer& operator*=(Integer const& x) noexcept
    {
        _value *= x._value;
        return *this;
    }

    inline constexpr Integer& operator/=(Integer const& x) noexcept
    {
        _value /= x._value;
        return *this;
    }

    inline constexpr Integer& operator%=(Integer const& x) noexcept
    {
        _value %= x._value;
        return *this;
    }

    inline constexpr Integer& operator|=(Integer const& x) noexcept
    {
        _value |= x._value;
        return *this;
    }

    inline constexpr Integer& operator&=(Integer const& x) noexcept
    {
        _value &= x._value;
        return *this;
    }

    inline constexpr Integer& operator^=(Integer const& x) noexcept
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

    friend inline constexpr auto operator+(Integer const& l, Integer const& r) noexcept
    {
        auto tmp = l;
        tmp+= r;
        return tmp;
    }

    friend inline constexpr auto operator-(Integer const& l, Integer const& r) noexcept
    {
        auto tmp = l;
        tmp-= r;
        return tmp;
    }

    friend inline constexpr auto operator*(Integer const& l, Integer const& r) noexcept
    {
        auto tmp = l;
        tmp*= r;
        return tmp;
    }

    friend inline constexpr auto operator/(Integer const& l, Integer const& r) noexcept
    {
        auto tmp = l;
        tmp/= r;
        return tmp;
    }

    friend inline constexpr auto operator%(Integer const& l, Integer const& r) noexcept
    {
        auto tmp = l;
        tmp%= r;
        return tmp;
    }

    friend inline constexpr auto operator&(Integer const& l, Integer const& r) noexcept
    {
        auto tmp = l;
        tmp&= r;
        return tmp;
    }

    friend inline constexpr auto operator|(Integer const& l, Integer const& r) noexcept
    {
        auto tmp = l;
        tmp|= r;
        return tmp;
    }

    friend inline constexpr auto operator^(Integer const& l, Integer const& r) noexcept
    {
        auto tmp = l;
        tmp^= r;
        return tmp;
    }

    friend inline constexpr Integer operator++(Integer& l, int) noexcept
    {
        auto tmp = l;
        ++l;
        return tmp;
    }

    friend inline constexpr Integer operator--(Integer& l, int) noexcept
    {
        auto tmp = l;
        --l;
        return tmp;
    }

    friend inline constexpr auto operator<=>(Integer const& l, Integer const& r) noexcept
    {
        return l._value <=> r._value;
    }

    friend inline constexpr bool operator==(Integer const& l, Integer const& r) noexcept
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
    constexpr auto operator()(FW_NS::Integer<T, TagType> const& v) const noexcept -> size_t
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
    constexpr auto operator()(FW_NS::Integer<T, TagType> const& v) const noexcept -> size_t
    {
        return std::hash<T>()(static_cast<T>(v));
    }
};

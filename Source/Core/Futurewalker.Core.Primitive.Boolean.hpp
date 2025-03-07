// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Primitive.BooleanType.hpp"
#include "Futurewalker.Core.Concepts.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Boolean wrapper class.
///
template <class T, class Tag>
class Boolean final
{
public:
    inline constexpr Boolean() = delete;
    inline constexpr Boolean(Boolean const&) = default;
    inline constexpr auto operator=(Boolean const&) -> Boolean& = default;

    ///
    /// @brief Construct boolean value.
    ///
    /// @param[in] value Boolean value.
    ///
    inline constexpr Boolean(T const& value) noexcept
      : _value {value}
    {
    }

    ///
    /// @brief Disallow implicit conversions.
    ///
    template <class U>
    requires(Concepts::Arithmetic<U> || Concepts::Pointer<U> || Concepts::Array<U>)
    Boolean(U const&) = delete;

    ///
    /// @brief Conversion operator to bool.
    ///
    /// @note Some libraries like Catch2 does static_cast<const bool&>() internally, which breaks with explicit operator bool() on gcc and msvc.
    ///
    inline explicit constexpr operator bool const&() const noexcept
    {
        return static_cast<bool const&>(_value);
    }

    ///
    /// @brief Negation of boolean.
    ///
    inline constexpr auto operator!() const noexcept -> Boolean
    {
        return Boolean(!_value);
    }

public:
    friend inline constexpr bool operator==(Boolean const& l, Boolean const& r) noexcept
    {
        return l._value == r._value;
    }

    friend inline constexpr bool operator!=(Boolean const& l, Boolean const& r) noexcept
    {
        return l._value != r._value;
    }

private:
    T _value;
};
}
}
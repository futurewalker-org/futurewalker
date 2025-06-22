// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Prelude.hpp"

#include <type_traits>
#include <utility>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT 
{
template <class T>
struct EnumClassFlagTraits : std::false_type
{
};
}
}

template <class E>
requires FW_NS::EnumClassFlagTraits<E>::value
inline constexpr auto operator&(E const lhs, E const rhs) noexcept
{
    return static_cast<E>(std::to_underlying(lhs) & std::to_underlying(rhs));
}

template <class E>
requires FW_NS::EnumClassFlagTraits<E>::value
inline constexpr auto operator&=(E& lhs, E const rhs) noexcept -> E&
{
    lhs = static_cast<E>(std::to_underlying(lhs) & std::to_underlying(rhs));
    return lhs;
}

template <class E>
requires FW_NS::EnumClassFlagTraits<E>::value
inline constexpr auto operator|(E const lhs, E const rhs) noexcept
{
    return static_cast<E>(std::to_underlying(lhs) | std::to_underlying(rhs));
}

template <class E>
requires FW_NS::EnumClassFlagTraits<E>::value
inline constexpr auto operator|=(E& lhs, E const rhs) noexcept -> E&
{
    lhs = static_cast<E>(std::to_underlying(lhs) | std::to_underlying(rhs));
    return lhs;
}

template <class E>
requires FW_NS::EnumClassFlagTraits<E>::value
inline constexpr auto operator^(E const lhs, E const rhs) noexcept
{
    return static_cast<E>(std::to_underlying(lhs) ^ std::to_underlying(rhs));
}

template <class E>
requires FW_NS::EnumClassFlagTraits<E>::value
inline constexpr auto operator^=(E& lhs, E const rhs) noexcept -> E&
{
    lhs = static_cast<E>(std::to_underlying(lhs) ^ std::to_underlying(rhs));
    return lhs;
}

template <class E>
requires FW_NS::EnumClassFlagTraits<E>::value
inline constexpr auto operator~(E const e) noexcept
{
    return static_cast<E>(~std::to_underlying(e));
}

#define FW_ENABLE_ENUM_CLASS_BITWISE_OPERATORS(E) template <> struct EnumClassFlagTraits<E> : std::true_type {}

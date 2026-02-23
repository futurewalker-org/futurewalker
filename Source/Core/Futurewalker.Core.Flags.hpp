// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Prelude.hpp"

#include "Futurewalker.Core.Concepts.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Bit flags for enum class types.
///
/// @tparam E Enum class type.
///
template <Concepts::EnumClass E>
class Flags
{
public:
    Flags() = delete;

    using EnumType = E;
    using UnderlyingType = std::underlying_type_t<E>;

    ///
    /// @brief Construct bit flags from enum value.
    ///
    /// @note Intentionally allows implicit conversion.
    ///
    inline constexpr Flags(E const e) noexcept
      : _e(e)
    {
    }

    ///
    /// @brief Copy constructor.
    ///
    inline constexpr Flags(Flags const&) noexcept = default;

    ///
    /// @brief Copy assignment operator.
    ///
    inline constexpr auto operator=(Flags const&) noexcept -> Flags& = default;

    ///
    /// @brief Three-way comparison operator.
    ///
    inline constexpr auto operator<=>(Flags const&) const noexcept -> std::strong_ordering = default;

    ///
    /// @brief Check if the current flags contain specified flags.
    ///
    /// @param[in] flags Flags to check.
    ///
    inline constexpr auto Contains(Flags const& flags) const noexcept -> Bool
    {
        return (std::to_underlying(_e) & std::to_underlying(flags._e)) == std::to_underlying(flags._e);
    }

    ///
    /// @brief Convert to enum value.
    ///
    inline explicit constexpr operator EnumType() const noexcept
    {
        return _e;
    }

    ///
    /// @brief Convert to underlying integer type.
    ///
    inline explicit constexpr operator UnderlyingType() const noexcept
    {
        return std::to_underlying(_e);
    }

    inline constexpr auto operator&=(Flags const& rhs) noexcept -> Flags&
    {
        _e = static_cast<E>(std::to_underlying(_e) & std::to_underlying(rhs._e));
        return *this;
    }

    inline constexpr auto operator|=(Flags const& rhs) noexcept -> Flags&
    {
        _e = static_cast<E>(std::to_underlying(_e) | std::to_underlying(rhs._e));
        return *this;
    }

    inline constexpr auto operator^=(Flags const& rhs) noexcept -> Flags&
    {
        _e = static_cast<E>(std::to_underlying(_e) ^ std::to_underlying(rhs._e));
        return *this;
    }

    friend inline constexpr auto operator&(Flags const& lhs, Flags const& rhs) noexcept -> Flags
    {
        return static_cast<E>(std::to_underlying(lhs._e) & std::to_underlying(rhs._e));
    }

    friend inline constexpr auto operator|(Flags const& lhs, Flags const& rhs) noexcept -> Flags
    {
        return static_cast<E>(std::to_underlying(lhs._e) | std::to_underlying(rhs._e));
    }

    friend inline constexpr auto operator^(Flags const& lhs, Flags const& rhs) noexcept -> Flags
    {
        return static_cast<E>(std::to_underlying(lhs._e) ^ std::to_underlying(rhs._e));
    }

    friend inline constexpr auto operator~(Flags const& e) noexcept -> Flags
    {
        return static_cast<E>(~std::to_underlying(e._e));
    }

private:
    E _e;
};
}
}

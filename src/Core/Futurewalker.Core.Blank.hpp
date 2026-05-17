// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Prelude.hpp"

#include <compare>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Empty struct.
///
struct Blank
{
};

inline constexpr auto operator<=>(Blank const&, Blank const&) noexcept -> std::strong_ordering
{
    return std::strong_ordering::equal;
}

inline constexpr auto operator==(Blank const&, Blank const&) noexcept -> bool
{
    return true;
}

inline constexpr auto operator!=(Blank const&, Blank const&) noexcept -> bool
{
    return false;
}
}
}

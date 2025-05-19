// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Graphics.Prelude.hpp"

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Font width value.
///
class FontWidth final : public Integer<int32_t, FontWidth>
{
public:
    using Integer<int32_t, FontWidth>::Integer;

    // clang-format off
    static inline constexpr auto UltraCondensed() { return FontWidth(1); }
    static inline constexpr auto ExtraCondensed() { return FontWidth(2); }
    static inline constexpr auto Condensed() { return FontWidth(3); }
    static inline constexpr auto SemiCondensed() { return FontWidth(4); }
    static inline constexpr auto Normal() { return FontWidth(5); }
    static inline constexpr auto SemiExpanded() { return FontWidth(6); }
    static inline constexpr auto Expanded() { return FontWidth(7); }
    static inline constexpr auto ExtraExpanded() { return FontWidth(8); }
    static inline constexpr auto UltraExpanded() { return FontWidth(9); }
    // clang-format on
};
}
}

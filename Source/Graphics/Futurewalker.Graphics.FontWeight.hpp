// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Graphics.Prelude.hpp"

#include "Futurewalker.Core.Primitive.hpp" 

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Font weight value.
///
class FontWeight final : public Integer<int32_t, FontWeight>
{
public:
    using Integer<int32_t, FontWeight>::Integer;

    // clang-format off
    static inline constexpr auto Thin() noexcept { return FontWeight(100); }
    static inline constexpr auto ExtraLight() noexcept { return FontWeight(200); }
    static inline constexpr auto Light() noexcept { return FontWeight(300); }
    static inline constexpr auto Normal() noexcept { return FontWeight(400); }
    static inline constexpr auto Medium() noexcept { return FontWeight(500); }
    static inline constexpr auto SemiBold() noexcept { return FontWeight(600); }
    static inline constexpr auto Bold() noexcept { return FontWeight(700); }
    static inline constexpr auto ExtraBold() noexcept { return FontWeight(800); }
    static inline constexpr auto Black() noexcept { return FontWeight(900); }
    static inline constexpr auto ExtraBlack() noexcept { return FontWeight(1000); }
    // clang-format on
};
}
}

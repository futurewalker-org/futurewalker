// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.SkiaTypeface.hpp"

#include <ranges>

namespace FW_GRAPHICS_DETAIL_NS
{
SkiaTypeface::SkiaTypeface(sk_sp<SkTypeface> const& typeface)
  : _typeface {typeface}
{
}

auto SkiaTypeface::GetGlyphs(std::span<char32_t const> codePoints) -> std::vector<GlyphId>
{
    auto const unichars = codePoints | std::ranges::views::transform([](auto cp) { return std::bit_cast<SkUnichar>(cp); }) | std::ranges::to<std::vector>();
    auto glyphs = std::vector<SkGlyphID>(codePoints.size());
    _typeface->unicharsToGlyphs(unichars.data(), static_cast<int>(unichars.size()), glyphs.data());
    return glyphs | std::ranges::views::transform([](auto id) { return GlyphId(id); }) | std::ranges::to<std::vector>();
}

auto SkiaTypeface::GetTypeface() const -> sk_sp<SkTypeface>
{
    return _typeface;
}
}

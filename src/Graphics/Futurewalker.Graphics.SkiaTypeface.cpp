// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.SkiaTypeface.hpp"

#include <include/core/SkFontMetrics.h>
#include <include/core/SkFont.h>

namespace FW_GRAPHICS_DETAIL_NS
{
SkiaTypeface::SkiaTypeface(sk_sp<SkTypeface> const& typeface)
  : _typeface {typeface}
{
}

auto SkiaTypeface::GetMetrics(FontSize const size) const -> FontMetrics
{
    auto const font = SkFont(GetSkTypeface(), static_cast<SkScalar>(size));
    auto metrics = SkFontMetrics();
    font.getMetrics(&metrics);

    return FontMetrics {
        .ascent = -metrics.fAscent,
        .descent = metrics.fDescent,
        .leading = metrics.fLeading,
        .xHeight = metrics.fXHeight,
        .capHeight = metrics.fCapHeight,
    };
}

auto SkiaTypeface::GetSkTypeface() const -> sk_sp<SkTypeface>
{
    return _typeface;
}
}

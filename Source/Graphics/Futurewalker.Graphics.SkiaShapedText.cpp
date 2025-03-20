// SPDX-License-Identifier: MIT

#include "Futurewalker.Graphics.SkiaShapedText.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
SkiaShapedText::SkiaShapedText(const sk_sp<SkTextBlob>& textBlob)
  : ShapedText()
  , _textBlob {textBlob}
{
}

auto SkiaShapedText::GetBounds() const -> Rect<Dp>
{
    if (_textBlob)
    {
        auto const bounds = _textBlob->bounds();
        return Rect<Dp>(bounds.fLeft, bounds.fTop, bounds.fRight, bounds.fBottom);
    }
    return {};
}

auto SkiaShapedText::GetTextBlob() const -> sk_sp<SkTextBlob>
{
    return _textBlob;
}
}

// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.SkiaShapedText.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
SkiaShapedText::SkiaShapedText(sk_sp<SkTextBlob> const& textBlob, Shared<TextLayoutInfo> const& textLayoutInfo)
  : ShapedText()
  , _textBlob {textBlob}
  , _textLayoutInfo {textLayoutInfo}
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

auto SkiaShapedText::GetLayoutInfo() const -> TextLayoutInfo const&
{
    return *_textLayoutInfo;
}

auto SkiaShapedText::GetTextBlob() const -> sk_sp<SkTextBlob>
{
    return _textBlob;
}
}

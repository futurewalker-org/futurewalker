// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.SkiaShapedTextType.hpp"
#include "Futurewalker.Graphics.ShapedText.hpp"

#include "Futurewalker.Graphics.TextLayoutInfo.hpp" 

#include "Futurewalker.Core.String.hpp"

#include <include/core/SkTextBlob.h>

namespace FW_GRAPHICS_DETAIL_NS
{
class SkiaShapedText : public ShapedText
{
public:
    SkiaShapedText(sk_sp<SkTextBlob> const& textBlob, Shared<TextLayoutInfo> const& textLayoutInfo);

    auto GetBounds() const -> Rect<Dp> override;
    auto GetLayoutInfo() const -> TextLayoutInfo const& override;

    auto GetTextBlob() const -> sk_sp<SkTextBlob>;

private:
    sk_sp<SkTextBlob> _textBlob;
    Shared<TextLayoutInfo> _textLayoutInfo;
};
}

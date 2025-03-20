// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Graphics.SkiaShapedTextType.hpp"
#include "Futurewalker.Graphics.ShapedText.hpp"

#include "Futurewalker.Core.String.hpp"

#include <include/core/SkTextBlob.h>

namespace FW_GRAPHICS_DETAIL_NS
{
class SkiaShapedText : public ShapedText
{
public:
    SkiaShapedText(const sk_sp<SkTextBlob>& textBlob);

    auto GetBounds() const -> Rect<Dp> override;

    auto GetTextBlob() const -> sk_sp<SkTextBlob>;

private:
    sk_sp<SkTextBlob> _textBlob;
};
}

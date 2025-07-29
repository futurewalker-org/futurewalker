// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.SkiaTextShaperType.hpp"
#include "Futurewalker.Graphics.TextShaper.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
class SkiaTextShaper : public TextShaper
{
public:
    auto ShapeText(String const& text, Shared<Typeface> const& typeface, FontSize const size, Dp const maxWidth) -> Shared<ShapedText> override;
    auto ShapeGlyph(char32_t const codePoint, Shared<Typeface> const& typeface, FontSize const size) -> Shared<ShapedText> override;
};
}
}

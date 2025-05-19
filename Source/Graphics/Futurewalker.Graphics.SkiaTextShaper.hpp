// SPDX-License-Identifier: MIT
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
    auto Shape(String const& text, Shared<Typeface> const& typeface, FontSize const size, Dp const maxWidth) -> Shared<ShapedText> override;
};
}
}

// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.PlatformSkiaFontManager.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
PlatformSkiaFontManager::~PlatformSkiaFontManager() = default;

auto PlatformSkiaFontManager::FontSlantToSkFontSlant(FontSlant const slant) -> SkFontStyle::Slant
{
    switch (slant)
    {
        case FontSlant::Upright:
            return SkFontStyle::kUpright_Slant;
        case FontSlant::Italic:
            return SkFontStyle::kItalic_Slant;
        case FontSlant::Oblique:
            return SkFontStyle::kOblique_Slant;
    }
    return SkFontStyle::kUpright_Slant;
}

auto PlatformSkiaFontManager::FontStyleToSkFontStyle(FontStyle const& fontStyle) -> SkFontStyle
{
    auto const weight = static_cast<int>(fontStyle.GetWeight());
    auto const width = static_cast<int>(fontStyle.GetWidth());
    auto const slant = FontSlantToSkFontSlant(fontStyle.GetSlant());
    return SkFontStyle(weight, width, slant);
}
}

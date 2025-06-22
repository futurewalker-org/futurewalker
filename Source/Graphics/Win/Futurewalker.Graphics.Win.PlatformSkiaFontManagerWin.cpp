// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.Win.PlatformSkiaFontManagerWin.hpp"

#include "Futurewalker.Graphics.SkiaTypeface.hpp"

#include "Futurewalker.Core.StringFunction.hpp"

#include <include/ports/SkTypeface_win.h>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace
{
auto FontSlantToSkFontSlant(FontSlant const slant)
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

auto FontStyleToSkFontStyle(FontStyle const& fontStyle)
{
    auto const weight = static_cast<int>(fontStyle.GetWeight());
    auto const width = static_cast<int>(fontStyle.GetWidth());
    auto const slant = FontSlantToSkFontSlant(fontStyle.GetSlant());
    return SkFontStyle(weight, width, slant);
}
}

PlatformSkiaFontManagerWin::PlatformSkiaFontManagerWin()
{
    _fontMgr = SkFontMgr_New_DirectWrite();
}

auto PlatformSkiaFontManagerWin::FindTypefaceByFamilyNameAndStyle(String const& familyName, FontStyle const& fontStyle) -> Shared<Typeface>
{
    if (_fontMgr)
    {
        auto const skFamilyName = StringFunction::ConvertStringToStdString(familyName);
        auto const skFontStyle = FontStyleToSkFontStyle(fontStyle);
        auto r = Shared<SkiaTypeface>::Make(_fontMgr->matchFamilyStyle(skFamilyName.c_str(), skFontStyle));
        return r;
    }
    return {};
}

auto PlatformSkiaFontManagerWin::GetSkFontMgr() -> sk_sp<SkFontMgr>
{
    return _fontMgr;
}
}

namespace FW_DETAIL_NS
{
auto Locator::Resolver<Graphics::PlatformSkiaFontManagerWin>::Resolve() -> Shared<Graphics::PlatformSkiaFontManagerWin>
{
    return Shared<Graphics::PlatformSkiaFontManagerWin>::Make();
}
}

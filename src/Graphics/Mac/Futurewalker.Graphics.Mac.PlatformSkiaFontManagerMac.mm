// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.Mac.PlatformSkiaFontManagerMac.hpp"

#include "Futurewalker.Graphics.SkiaTypeface.hpp"
#include "Futurewalker.Graphics.SkiaFunction.hpp"

#include "Futurewalker.Core.StringFunction.hpp"

#include <include/ports/SkTypeface_mac.h>
#include <include/ports/SkFontMgr_mac_ct.h>

namespace FW_GRAPHICS_DETAIL_NS
{
PlatformSkiaFontManagerMac::PlatformSkiaFontManagerMac()
{
    _fontMgr = SkFontMgr_New_CoreText(nullptr);
}

auto PlatformSkiaFontManagerMac::FindTypefaceByFamilyNameAndStyle(String const& familyName, FontStyle const& fontStyle) -> Shared<Typeface>
{
    if (_fontMgr)
    {
        auto const skFamilyName = familyName.ToStdString();
        auto const skFontStyle = SkiaFunction::FontStyleToSkFontStyle(fontStyle);
        auto r = Shared<SkiaTypeface>::Make(_fontMgr->matchFamilyStyle(skFamilyName.c_str(), skFontStyle));
        return r;
    }
    return {};
}

auto PlatformSkiaFontManagerMac::FindTypefaceByGenericFontKind(GenericFontKind const genericKind, FontStyle const& fontStyle) -> Shared<Typeface>
{
    (void)fontStyle;

    if (_fontMgr)
    {
        if (genericKind == GenericFontKind::UiSansSerif || genericKind == GenericFontKind::UiSerif || genericKind == GenericFontKind::UiMonospace)
        {
            auto const ctFont = CTFontCreateUIFontForLanguage(kCTFontUIFontSystem, 30, nullptr);
            auto const skTypeface = SkMakeTypefaceFromCTFont(ctFont);
            CFRelease(ctFont);
            return Shared<SkiaTypeface>::Make(skTypeface);
        }
    }
    return {};
}

auto PlatformSkiaFontManagerMac::GetSkFontMgr() -> sk_sp<SkFontMgr>
{
    return _fontMgr;
}
}

namespace FW_DETAIL_NS
{
auto Locator::Resolver<Graphics::PlatformSkiaFontManagerMac>::Resolve() -> Shared<Graphics::PlatformSkiaFontManagerMac>
{
    return Shared<Graphics::PlatformSkiaFontManagerMac>::Make();
}
}

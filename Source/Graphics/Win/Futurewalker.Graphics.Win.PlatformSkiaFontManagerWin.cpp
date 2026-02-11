// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.Win.PlatformSkiaFontManagerWin.hpp"

#include "Futurewalker.Graphics.SkiaTypeface.hpp"
#include "Futurewalker.Graphics.SkiaFunction.hpp"

#include "Futurewalker.Core.StringFunction.hpp"

#include <include/ports/SkTypeface_win.h>

namespace FW_GRAPHICS_DETAIL_NS
{
PlatformSkiaFontManagerWin::PlatformSkiaFontManagerWin()
{
    _fontMgr = SkFontMgr_New_DirectWrite();
}

auto PlatformSkiaFontManagerWin::FindTypefaceByFamilyNameAndStyle(String const& familyName, FontStyle const& fontStyle) -> Shared<Typeface>
{
    if (_fontMgr)
    {
        auto const skFamilyName = StringFunction::ConvertStringToStdString(familyName);
        auto const skFontStyle = SkiaFunction::FontStyleToSkFontStyle(fontStyle);
        return Shared<SkiaTypeface>::Make(_fontMgr->matchFamilyStyle(skFamilyName.c_str(), skFontStyle));
    }
    return {};
}

auto PlatformSkiaFontManagerWin::FindTypefaceByGenericFontKind(GenericFontKind const genericKind, FontStyle const& fontStyle) -> Shared<Typeface>
{
    if (_fontMgr)
    {
        if (genericKind == GenericFontKind::UiSansSerif || genericKind == GenericFontKind::UiSerif || genericKind == GenericFontKind::UiMonospace)
        {
            auto nonclientMetrics = NONCLIENTMETRICS {
                .cbSize = sizeof(NONCLIENTMETRICS),
            };
            if (::SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &nonclientMetrics, 0))
            {
                // Resulting font family depends on language settings of the OS.
                // For example, in Japanese Windows, this will return "Yu Gothic UI" instead of "Segoe UI".
                auto logFont = nonclientMetrics.lfMessageFont;
                logFont.lfItalic = fontStyle.GetSlant() == FontSlant::Italic;
                logFont.lfWeight = static_cast<LONG>(fontStyle.GetWeight());
                return Shared<SkiaTypeface>::Make(SkCreateTypefaceFromLOGFONT(logFont));
            }
        }
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

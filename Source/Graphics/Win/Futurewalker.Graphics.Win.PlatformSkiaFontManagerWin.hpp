// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.Win.PlatformSkiaFontManagerWinType.hpp"
#include "Futurewalker.Graphics.PlatformSkiaFontManager.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include <include/core/SkFontMgr.h>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformSkiaFontManagerWin final : public PlatformSkiaFontManager
{
public:
    PlatformSkiaFontManagerWin();

    auto FindTypefaceByFamilyNameAndStyle(String const& familyName, FontStyle const& style) -> Shared<Typeface> override;
    auto GetSkFontMgr() -> sk_sp<SkFontMgr> override;

private:
    sk_sp<SkFontMgr> _fontMgr;
};
}
}

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
template <>
struct Locator::Resolver<Graphics::PlatformSkiaFontManagerWin>
{
    using Interface = Graphics::PlatformSkiaFontManager;
    static auto Resolve() -> Shared<Graphics::PlatformSkiaFontManagerWin>;
};
}
}

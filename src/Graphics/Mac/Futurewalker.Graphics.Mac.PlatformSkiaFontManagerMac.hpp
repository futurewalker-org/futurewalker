// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.PlatformSkiaFontManager.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include <include/core/SkFontMgr.h>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformSkiaFontManagerMac : public PlatformSkiaFontManager
{
public:
    PlatformSkiaFontManagerMac();
    auto FindTypefaceByFamilyNameAndStyle(String const& familyName, FontStyle const& style) -> Shared<Typeface> override;
    auto FindTypefaceByGenericFontKind(GenericFontKind const genericKind, FontStyle const& fontStyle) -> Shared<Typeface> override;
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
struct Locator::Resolver<Graphics::PlatformSkiaFontManagerMac>
{
    using Interface = Graphics::PlatformFontManager;
    static auto Resolve() -> Shared<Graphics::PlatformSkiaFontManagerMac>;
};
}
}


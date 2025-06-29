// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.PlatformSkiaFontManagerType.hpp"
#include "Futurewalker.Graphics.FontStyle.hpp"
#include "Futurewalker.Graphics.TypefaceType.hpp"
#include "Futurewalker.Graphics.FontKind.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.String.hpp"

#include <include/core/SkFontMgr.h>
#include <include/core/SkFontStyle.h>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
class PlatformSkiaFontManager : NonCopyable
{
public:
    virtual ~PlatformSkiaFontManager() = 0;
    virtual auto FindTypefaceByFamilyNameAndStyle(String const& familyName, FontStyle const& style) -> Shared<Typeface> = 0;
    virtual auto GetSkFontMgr() -> sk_sp<SkFontMgr> = 0;

protected:
    static auto FontSlantToSkFontSlant(FontSlant const slant) -> SkFontStyle::Slant;
    static auto FontStyleToSkFontStyle(FontStyle const& fontStyle) -> SkFontStyle;
};
}
}

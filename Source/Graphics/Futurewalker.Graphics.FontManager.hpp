// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.FontManagerType.hpp"
#include "Futurewalker.Graphics.PlatformFontManagerType.hpp"
#include "Futurewalker.Graphics.FontStyle.hpp"
#include "Futurewalker.Graphics.FontFamilyType.hpp"
#include "Futurewalker.Graphics.TypefaceType.hpp"
#include "Futurewalker.Graphics.FontKind.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.String.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
class FontManager : NonCopyable
{
public:
    explicit FontManager(Shared<PlatformFontManager> const& platform);

    auto FindTypefaceByFamilyAndStyle(FontFamily const& fontFamily, FontStyle const& fontStyle) -> Shared<Typeface>;
    auto FindTypefaceByFamilyNameAndStyle(String const& familyName, FontStyle const& fontStyle) -> Shared<Typeface>;
    auto FindTypefaceByGenericFontKind(GenericFontKind const genericKind, FontStyle const& fontStyle) -> Shared<Typeface>;
    auto FindTypefaceByFontKind(FontKind const fontKind) -> Shared<Typeface>;
    auto RegisterFontKind(FontKind const fontKind, Shared<Typeface> const& typeface) -> void;

private:
    HashMap<FontKind, Shared<Typeface>> _map;
    Shared<PlatformFontManager> _platform;
};
}
}

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
template <>
struct Locator::Resolver<Graphics::FontManager>
{
    using Interface = Graphics::FontManager;
    static auto Resolve() -> Shared<Graphics::FontManager>;
};
}
}

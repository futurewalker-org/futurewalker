// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.FontManager.hpp"
#include "Futurewalker.Graphics.PlatformFontManager.hpp" 
#include "Futurewalker.Graphics.FontFamily.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
FontManager::FontManager(Shared<PlatformFontManager> const& platform)
  : _platform {platform}
{
}

auto FontManager::FindTypefaceByFamilyAndStyle(FontFamily const& fontFamily, FontStyle const& fontStyle) -> Shared<Typeface>
{
    if (_platform)
    {
        if (auto const familyName = fontFamily.GetFamilyName())
        {
            return FindTypefaceByFamilyNameAndStyle(*familyName, fontStyle);
        }
        else if (auto const kind = fontFamily.GetFontKind())
        {
            return FindTypefaceByFontKind(*kind);
        }
        else if (auto const genericKind = fontFamily.GetGenericFontKind())
        {
            return FindTypefaceByGenericFontKind(*genericKind, fontStyle);
        }
    }
    return {};
}

auto FontManager::FindTypefaceByFamilyNameAndStyle(String const& familyName, FontStyle const& fontStyle) -> Shared<Typeface>
{
    if (_platform)
    {
        return _platform->FindTypefaceByFamilyNameAndStyle(familyName, fontStyle);
    }
    return {};
}

auto FontManager::FindTypefaceByGenericFontKind(GenericFontKind const genericKind, FontStyle const& fontStyle) -> Shared<Typeface>
{
    if (_platform)
    {
        return _platform->FindTypefaceByGenericFontKind(genericKind, fontStyle);
    }
    return {};
}

auto FontManager::FindTypefaceByFontKind(FontKind const fontKind) -> Shared<Typeface>
{
    auto it = _map.find(fontKind);
    if (it != _map.end())
    {
        return it->second;
    }
    return {};
}

auto FontManager::RegisterFontKind(FontKind const fontKind, Shared<Typeface> const& typeface) -> void
{
    _map.insert_or_assign(fontKind, typeface);
}
}

namespace FW_DETAIL_NS
{
auto Locator::Resolver<Graphics::FontManager>::Resolve() -> Shared<Graphics::FontManager>
{
    auto const platform = Locator::Resolve<Graphics::PlatformFontManager>();
    return Shared<Graphics::FontManager>::Make(platform);
}
}

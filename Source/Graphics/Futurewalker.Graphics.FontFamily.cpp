// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.FontFamily.hpp"
#include "Futurewalker.Graphics.FontKind.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
FontFamily::FontFamily()
  : _var {String(u8"sans-serif")}
{
}

FontFamily::FontFamily(FontKind const fontKind)
  : _var {fontKind}
{
}

FontFamily::FontFamily(String const& familyName)
  : _var {familyName}
{
}

auto FontFamily::GetFontKind() const -> Optional<FontKind>
{
    if (auto const kind = std::get_if<FontKind>(&_var))
    {
        return *kind;
    }
    return {};
}

auto FontFamily::GetFontFamily() const -> Optional<String>
{
    if (auto const family = std::get_if<String>(&_var))
    {
        return *family;
    }
    return {};
}
}

// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.FontFamilyType.hpp"
#include "Futurewalker.Graphics.FontKind.hpp"

#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.Optional.hpp"

#include <variant>

#include <boost/variant2.hpp>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Font family.
///
class FontFamily
{
public:
    FontFamily();
    FontFamily(FontFamily const&) = default;
    FontFamily(FontFamily&&) noexcept = default;

    explicit FontFamily(GenericFontKind const genericKind);
    explicit FontFamily(FontKind const fontKind);
    explicit FontFamily(String const& familyName);

    auto GetGenericFontKind() const -> Optional<GenericFontKind>;
    auto GetFontKind() const -> Optional<FontKind>;
    auto GetFamilyName() const -> Optional<String>;

    auto operator=(FontFamily const&) -> FontFamily& = default;
    auto operator=(FontFamily&&) noexcept -> FontFamily& = default;

    friend auto operator<=>(FontFamily const& l, FontFamily const& r) noexcept = default;

private:
    std::variant<GenericFontKind, FontKind, String> _var;
};
}
}

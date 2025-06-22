// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.FontKind.hpp"

#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.Optional.hpp"

#include <variant>

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

    explicit FontFamily(FontKind const fontKind);
    explicit FontFamily(String const& familyName);

    auto GetFontKind() const -> Optional<FontKind>;
    auto GetFontFamily() const -> Optional<String>;

    auto operator=(FontFamily const&) -> FontFamily& = default;

    friend auto operator<=>(FontFamily const& l, FontFamily const& r) noexcept = default;

private:
    std::variant<FontKind, String> _var;
};
}
}

// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.PlatformFontManagerType.hpp"
#include "Futurewalker.Graphics.FontStyle.hpp"
#include "Futurewalker.Graphics.TypefaceType.hpp"
#include "Futurewalker.Graphics.FontKind.hpp"
#include "Futurewalker.Graphics.FontFamilyType.hpp"

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
class PlatformFontManager : public NonCopyable
{
public:
    virtual ~PlatformFontManager() = 0;
    virtual auto FindTypefaceByFamilyNameAndStyle(String const& familyName, FontStyle const& style) -> Shared<Typeface> = 0;
    virtual auto FindTypefaceByGenericFontKind(GenericFontKind const genericKind, FontStyle const& fontStyle) -> Shared<Typeface> = 0;
};
}
}

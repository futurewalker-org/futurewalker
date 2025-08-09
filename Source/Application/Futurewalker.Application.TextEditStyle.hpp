// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

#include "Futurewalker.Attribute.StaticAttribute.hpp"

#include "Futurewalker.Graphics.FontWeight.hpp"
#include "Futurewalker.Graphics.FontWidth.hpp"
#include "Futurewalker.Graphics.FontSize.hpp"
#include "Futurewalker.Graphics.FontFamily.hpp"
#include "Futurewalker.Graphics.FontStyleType.hpp"

#include "Futurewalker.Color.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
struct TextEditStyle : NonConstructible
{
    FW_STATIC_ATTRIBUTE(RGBAColor, BackgroundColor);
    FW_STATIC_ATTRIBUTE(RGBAColor, TextColor);
    FW_STATIC_ATTRIBUTE(Graphics::FontSize, FontSize);
    FW_STATIC_ATTRIBUTE(Graphics::FontWeight, FontWeight);
    FW_STATIC_ATTRIBUTE(Graphics::FontWidth, FontWidth);
    FW_STATIC_ATTRIBUTE(Graphics::FontSlant, FontSlant);
    FW_STATIC_ATTRIBUTE(Graphics::FontFamily, FontFamily);
};
}
}

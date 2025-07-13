// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.TextViewStyleType.hpp"

#include "Futurewalker.Attribute.StaticAttribute.hpp"

#include "Futurewalker.Graphics.FontWeight.hpp"
#include "Futurewalker.Graphics.FontWidth.hpp"
#include "Futurewalker.Graphics.FontSize.hpp"
#include "Futurewalker.Graphics.FontFamily.hpp"
#include "Futurewalker.Graphics.FontStyleType.hpp"

#include "Futurewalker.Color.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class TextViewStyle : NonConstructible
{
public:
    FW_STATIC_ATTRIBUTE(RGBAColor, Color);
    FW_STATIC_ATTRIBUTE(Graphics::FontSize, FontSize);
    FW_STATIC_ATTRIBUTE(Graphics::FontWeight, FontWeight);
    FW_STATIC_ATTRIBUTE(Graphics::FontWidth, FontWidth);
    FW_STATIC_ATTRIBUTE(Graphics::FontSlant, FontSlant);
    FW_STATIC_ATTRIBUTE(Graphics::FontFamily, FontFamily);
    FW_STATIC_ATTRIBUTE(TextViewHorizontalAlignment, HorizontalAlignment);
    FW_STATIC_ATTRIBUTE(TextViewVerticalAlignment, VerticalAlignment);
};
}
}

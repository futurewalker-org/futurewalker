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
    static StaticAttribute<RGBAColor> const Color;
    static StaticAttribute<Graphics::FontSize> const FontSize;
    static StaticAttribute<Graphics::FontWeight> const FontWeight;
    static StaticAttribute<Graphics::FontWidth> const FontWidth;
    static StaticAttribute<Graphics::FontSlant> const FontSlant;
    static StaticAttribute<Graphics::FontFamily> const FontFamily;
    static StaticAttribute<TextViewHorizontalAlignment> const HorizontalAlignment;
    static StaticAttribute<TextViewVerticalAlignment> const VerticalAlignment;
};
}
}

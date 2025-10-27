// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.Prelude.hpp"

#include "Futurewalker.Application.TextViewType.hpp"

#include "Futurewalker.Graphics.FontSize.hpp"
#include "Futurewalker.Graphics.FontWeight.hpp"
#include "Futurewalker.Graphics.FontWidth.hpp"
#include "Futurewalker.Graphics.FontFamily.hpp"
#include "Futurewalker.Graphics.FontStyle.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Color.RGBAColor.hpp"

#include "Futurewalker.Attribute.StaticAttributeType.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
struct TextViewStyle : NonConstructible
{
    FW_STATIC_ATTRIBUTE(RGBAColor, Color);
    FW_STATIC_ATTRIBUTE(Channel, Alpha);
    FW_STATIC_ATTRIBUTE(RGBAColor, DisabledColor);
    FW_STATIC_ATTRIBUTE(Channel, DisabledAlpha);
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

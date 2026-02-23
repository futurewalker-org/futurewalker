// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.Prelude.hpp"

#include "Futurewalker.Application.CornerRadius.hpp"
#include "Futurewalker.Application.EdgeInsets.hpp"
#include "Futurewalker.Application.TextViewType.hpp"
#include "Futurewalker.Application.AxisConstraints.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Color.RGBAColor.hpp"

#include "Futurewalker.Attribute.StaticAttribute.hpp"

#include "Futurewalker.Graphics.FontWeight.hpp"
#include "Futurewalker.Graphics.FontWidth.hpp"
#include "Futurewalker.Graphics.FontSize.hpp"
#include "Futurewalker.Graphics.FontFamily.hpp"
#include "Futurewalker.Graphics.FontStyleType.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
struct TextButtonStyle : NonConstructible
{
    FW_STATIC_ATTRIBUTE(RGBAColor, BackgroundColor);
    FW_STATIC_ATTRIBUTE(Channel, BackgroundAlpha);
    FW_STATIC_ATTRIBUTE(RGBAColor, DisabledBackgroundColor);
    FW_STATIC_ATTRIBUTE(Channel, DisabledBackgroundAlpha);
    FW_STATIC_ATTRIBUTE(RGBAColor, BorderColor);
    FW_STATIC_ATTRIBUTE(Channel, BorderAlpha);
    FW_STATIC_ATTRIBUTE(Dp, BorderWidth);
    FW_STATIC_ATTRIBUTE(RGBAColor, DisabledBorderColor);
    FW_STATIC_ATTRIBUTE(Channel, DisabledBorderAlpha);
    FW_STATIC_ATTRIBUTE(Dp, DisabledBorderWidth);
    FW_STATIC_ATTRIBUTE(RGBAColor, FocusedBorderColor);
    FW_STATIC_ATTRIBUTE(Channel, FocusedBorderAlpha);
    FW_STATIC_ATTRIBUTE(Dp, FocusedBorderWidth);
    FW_STATIC_ATTRIBUTE(RGBAColor, HighlightColor);
    FW_STATIC_ATTRIBUTE(CornerRadius, CornerRadius);
    FW_STATIC_ATTRIBUTE(EdgeInsets, Padding);
    FW_STATIC_ATTRIBUTE(Channel, HoverHighlightAlpha);
    FW_STATIC_ATTRIBUTE(Channel, PressHighlightAlpha);
    FW_STATIC_ATTRIBUTE(RGBAColor, TextColor);
    FW_STATIC_ATTRIBUTE(RGBAColor, DisabledTextColor);
    FW_STATIC_ATTRIBUTE(Graphics::FontSize, TextFontSize);
    FW_STATIC_ATTRIBUTE(Graphics::FontWeight, TextFontWeight);
    FW_STATIC_ATTRIBUTE(Graphics::FontWidth, TextFontWidth);
    FW_STATIC_ATTRIBUTE(Graphics::FontSlant, TextFontSlant);
    FW_STATIC_ATTRIBUTE(Graphics::FontFamily, TextFontFamily);
    FW_STATIC_ATTRIBUTE(TextViewHorizontalAlignment, TextHorizontalAlignment);
    FW_STATIC_ATTRIBUTE(TextViewVerticalAlignment, TextVerticalAlignment);
};
}
}

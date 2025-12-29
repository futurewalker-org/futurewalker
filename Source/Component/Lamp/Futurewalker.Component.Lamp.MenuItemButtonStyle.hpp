// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.Prelude.hpp"

#include "Futurewalker.Application.CornerRadius.hpp"
#include "Futurewalker.Application.EdgeInsets.hpp"

#include "Futurewalker.Attribute.StaticAttribute.hpp"

#include "Futurewalker.Color.RGBAColor.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
struct MenuItemButtonStyle : NonConstructible
{
    FW_STATIC_ATTRIBUTE(RGBAColor, BackgroundColor);
    FW_STATIC_ATTRIBUTE(Channel, BackgroundAlpha);
    FW_STATIC_ATTRIBUTE(RGBAColor, DisabledBackgroundColor);
    FW_STATIC_ATTRIBUTE(Channel, DisabledBackgroundAlpha);
    FW_STATIC_ATTRIBUTE(RGBAColor, BorderColor);
    FW_STATIC_ATTRIBUTE(Channel, BorderAlpha);
    FW_STATIC_ATTRIBUTE(RGBAColor, DisabledBorderColor);
    FW_STATIC_ATTRIBUTE(Channel, DisabledBorderAlpha);
    FW_STATIC_ATTRIBUTE(RGBAColor, HighlightColor);
    FW_STATIC_ATTRIBUTE(Dp, BorderWidth);
    FW_STATIC_ATTRIBUTE(CornerRadius, CornerRadius);
    FW_STATIC_ATTRIBUTE(EdgeInsets, Padding);
    FW_STATIC_ATTRIBUTE(Channel, HoverHighlightAlpha);
    FW_STATIC_ATTRIBUTE(Channel, PressHighlightAlpha);
    FW_STATIC_ATTRIBUTE(RGBAColor, TextColor);
    FW_STATIC_ATTRIBUTE(Channel, TextAlpha);
    FW_STATIC_ATTRIBUTE(RGBAColor, DisabledTextColor);
    FW_STATIC_ATTRIBUTE(Channel, DisabledTextAlpha);
    FW_STATIC_ATTRIBUTE(RGBAColor, IconColor);
    FW_STATIC_ATTRIBUTE(Channel, IconAlpha);
    FW_STATIC_ATTRIBUTE(RGBAColor, DisabledIconColor);
    FW_STATIC_ATTRIBUTE(Channel, DisabledIconAlpha);
};
}
}

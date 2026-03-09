// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.Prelude.hpp"

#include "Futurewalker.Application.EdgeInsets.hpp"
#include "Futurewalker.Application.CornerRadius.hpp"

#include "Futurewalker.Attribute.StaticAttribute.hpp"

#include "Futurewalker.Color.RGBAColor.hpp" 

#include "Futurewalker.Unit.Unit.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Style for menu view.
///
struct MenuViewStyle : NonConstructible
{
    FW_STATIC_ATTRIBUTE(RGBAColor, IconColor);
    FW_STATIC_ATTRIBUTE(RGBAColor, DisabledIconColor);
    FW_STATIC_ATTRIBUTE(RGBAColor, TextColor);
    FW_STATIC_ATTRIBUTE(RGBAColor, DisabledTextColor);
    FW_STATIC_ATTRIBUTE(RGBAColor, HighlightColor);
    FW_STATIC_ATTRIBUTE(RGBAColor, BackgroundColor);
    FW_STATIC_ATTRIBUTE(RGBAColor, DisabledBackgroundColor);
    FW_STATIC_ATTRIBUTE(RGBAColor, BorderColor);
    FW_STATIC_ATTRIBUTE(RGBAColor, DisabledBorderColor);
    FW_STATIC_ATTRIBUTE(Channel, IconAlpha);
    FW_STATIC_ATTRIBUTE(Channel, DisabledIconAlpha);
    FW_STATIC_ATTRIBUTE(Channel, TextAlpha);
    FW_STATIC_ATTRIBUTE(Channel, DisabledTextAlpha);
    FW_STATIC_ATTRIBUTE(Channel, HoverHighlightAlpha);
    FW_STATIC_ATTRIBUTE(Channel, PressHighlightAlpha);
    FW_STATIC_ATTRIBUTE(Dp, BorderWidth);
    FW_STATIC_ATTRIBUTE(EdgeInsets, Padding);
    FW_STATIC_ATTRIBUTE(FW_NS::CornerRadius, CornerRadius);
    FW_STATIC_ATTRIBUTE(EdgeInsets, ItemPadding);
    FW_STATIC_ATTRIBUTE(Dp, ItemSpacing);
    FW_STATIC_ATTRIBUTE(FW_NS::CornerRadius, ItemCornerRadius);
};
}
}

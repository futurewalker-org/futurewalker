// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.Prelude.hpp"

#include "Futurewalker.Application.CornerRadius.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Color.RGBAColor.hpp"

#include "Futurewalker.Attribute.StaticAttributeType.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
struct ButtonViewStyle : NonConstructible
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
    FW_STATIC_ATTRIBUTE(Channel, HoverHighlightAlpha);
    FW_STATIC_ATTRIBUTE(Channel, PressHighlightAlpha);
    FW_STATIC_ATTRIBUTE(CornerRadius, CornerRadius);
    FW_STATIC_ATTRIBUTE(Dp, BorderWidth);
};
}
}

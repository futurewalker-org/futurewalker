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
struct CheckBoxStyle : NonConstructible
{
    FW_STATIC_ATTRIBUTE(Dp, Size);
    FW_STATIC_ATTRIBUTE(CornerRadius, CornerRadius);
    FW_STATIC_ATTRIBUTE(RGBAColor, BackgroundColor);
    FW_STATIC_ATTRIBUTE(Channel, BackgroundAlpha);
    FW_STATIC_ATTRIBUTE(RGBAColor, BackgroundColorDisabled);
    FW_STATIC_ATTRIBUTE(Channel, BackgroundAlphaDisabled);
    FW_STATIC_ATTRIBUTE(RGBAColor, BorderColor);
    FW_STATIC_ATTRIBUTE(Channel, BorderAlpha);
    FW_STATIC_ATTRIBUTE(RGBAColor, BorderColorDisabled);
    FW_STATIC_ATTRIBUTE(Channel, BorderAlphaDisabled);
    FW_STATIC_ATTRIBUTE(RGBAColor, BorderColorFocused);
    FW_STATIC_ATTRIBUTE(Channel, BorderAlphaFocused);
    FW_STATIC_ATTRIBUTE(RGBAColor, CheckMarkColor);
    FW_STATIC_ATTRIBUTE(Channel, CheckMarkAlpha);
    FW_STATIC_ATTRIBUTE(RGBAColor, CheckMarkColorDisabled);
    FW_STATIC_ATTRIBUTE(Channel, CheckMarkAlphaDisabled);
    FW_STATIC_ATTRIBUTE(EdgeInsets, Padding);
    FW_STATIC_ATTRIBUTE(Dp, LabelGap);
};
}
}

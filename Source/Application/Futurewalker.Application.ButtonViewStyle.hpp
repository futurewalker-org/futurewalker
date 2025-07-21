// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"
#include "Futurewalker.Application.CornerRadius.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

#include "Futurewalker.Attribute.StaticAttribute.hpp"

#include "Futurewalker.Color.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
struct ButtonViewStyle : NonConstructible
{
    FW_STATIC_ATTRIBUTE(RGBAColor, BackgroundColor);
    FW_STATIC_ATTRIBUTE(RGBAColor, HighlightColor);
    FW_STATIC_ATTRIBUTE(RGBAColor, BorderColor);
    FW_STATIC_ATTRIBUTE(AlphaChannel, HoverHighlightAlpha);
    FW_STATIC_ATTRIBUTE(AlphaChannel, PressHighlightAlpha);
    FW_STATIC_ATTRIBUTE(CornerRadius, CornerRadius);
    FW_STATIC_ATTRIBUTE(Dp, BorderWidth);
};
}
}

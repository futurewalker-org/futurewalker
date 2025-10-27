// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.Prelude.hpp"

#include "Futurewalker.Color.RGBAColor.hpp"

#include "Futurewalker.Attribute.StaticAttributeType.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
struct IconViewStyle : NonConstructible
{
    FW_STATIC_ATTRIBUTE(Dp, Size);
    FW_STATIC_ATTRIBUTE(RGBAColor, Color);
    FW_STATIC_ATTRIBUTE(Channel, Alpha);
    FW_STATIC_ATTRIBUTE(RGBAColor, DisabledColor);
    FW_STATIC_ATTRIBUTE(Channel, DisabledAlpha);
};
}
}

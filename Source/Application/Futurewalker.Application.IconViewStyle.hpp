// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

#include "Futurewalker.Color.RGBAColor.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Attribute.StaticAttribute.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
struct IconViewStyle : NonConstructible
{
    FW_STATIC_ATTRIBUTE(Dp, IconSize);
    FW_STATIC_ATTRIBUTE(RGBAColor, IconColor);
};
}
}

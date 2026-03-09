// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.Prelude.hpp"

#include "Futurewalker.Application.EdgeInsets.hpp"

#include "Futurewalker.Attribute.StaticAttribute.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
struct MenuItemViewStyle : NonConstructible
{
    FW_STATIC_ATTRIBUTE(EdgeInsets, Padding);
};
}
}

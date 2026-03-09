// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.Prelude.hpp"

#include "Futurewalker.Application.ViewType.hpp"
#include "Futurewalker.Application.MenuItemType.hpp"

#include "Futurewalker.Core.Function.hpp"
#include "Futurewalker.Core.Memory.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
class MenuItemViewBuilder;
using MenuItemViewBuilderFunction = Function<Shared<View>(MenuItemViewBuilder const& builder, MenuItem const& item)>;
}
}

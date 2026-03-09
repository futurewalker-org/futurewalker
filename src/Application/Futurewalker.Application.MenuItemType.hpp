// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

#include "Futurewalker.Core.Identifier.hpp"

#include <vector>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class MenuItem;
using MenuItemId = Identifier<UInt64, MenuItem>;
using MenuItemArray = std::vector<MenuItem>;

enum class MenuItemType
{
    Item,
    Separator,
    SubMenu,
};
}
}

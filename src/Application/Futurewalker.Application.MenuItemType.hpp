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

///
/// @brief Menu item role.
///
/// Menu items with specific roles may be moved to system specific locations in the menu.
///
enum class MenuItemRole
{
    Other,
    About,       ///< About menu item.
    AppSpecific, ///< App specific menu item(s).
    Preferences, ///< Preferences menu item.
    Services,    ///< Services menu item.
    Quit,        ///< Quit menu item.
};
}
}

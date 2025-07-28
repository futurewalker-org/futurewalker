// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.MenuItemType.hpp"

#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.Memory.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Menu item.
///
class MenuItem
{
public:
    explicit MenuItem(MenuItemType const type);

    auto GetType() const -> MenuItemType;
    auto SetType(MenuItemType const type) -> void;

    auto GetId() const -> MenuItemId;
    auto SetId(MenuItemId const id) -> void;

    auto GetSubMenu() const -> MenuItemArray;
    auto SetSubMenu(MenuItemArray const& subMenu) -> void;

    auto GetTitle() const -> String;
    auto SetTitle(String const& title) -> void;

    auto GetSubtitle() const -> String;
    auto SetSubtitle(String const& subtitle) -> void;

private:
    struct State
    {
        MenuItemType type = MenuItemType::Item;
        MenuItemId id = 0u;
        MenuItemArray subMenu;
        String title;
        String subtitle;
    };
    Shared<State> _state;

private:
    auto IsUnique() const -> Bool;
    auto GetImmutableState() const -> Pointer<State const>;
    auto GetMutableState() -> State&;
};
}
}

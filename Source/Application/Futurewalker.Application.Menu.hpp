// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.MenuType.hpp"
#include "Futurewalker.Application.MenuItemType.hpp"

#include "Futurewalker.Core.Memory.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Menu.
///
class Menu
{
public:
    auto GetItems() const -> MenuItemArray;
    auto SetItems(MenuItemArray const& items) -> void;

    auto AddItem(MenuItem const& item) -> void;

private:
    struct State
    {
        MenuItemArray items;
    };
    Shared<State> _state;

private:
    auto IsUnique() const -> Bool;
    auto GetImmutableState() const -> Pointer<State const>;
    auto GetMutableState() -> State&;
};
}
}

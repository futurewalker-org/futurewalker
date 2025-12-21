// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.MenuType.hpp"
#include "Futurewalker.Application.MenuItemType.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Optional.hpp"

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
    auto IsEmpty() const -> Bool;
    auto GetItemCount() const -> SInt64;
    auto GetItemAt(SInt64 index) const -> Optional<MenuItem>;
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

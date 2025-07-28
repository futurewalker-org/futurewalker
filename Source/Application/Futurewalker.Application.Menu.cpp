// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Menu.hpp"
#include "Futurewalker.Application.MenuItem.hpp"

namespace FW_DETAIL_NS
{
auto Menu::GetItems() const -> MenuItemArray
{
    if (auto state = GetImmutableState())
    {
        return state->items;
    }
    return {};
}

auto Menu::SetItems(MenuItemArray const& items) -> void
{
    GetMutableState().items = items;
}

auto Menu::AddItem(MenuItem const& item) -> void
{
    GetMutableState().items.push_back(item);
}

auto Menu::IsUnique() const -> Bool
{
    return _state.GetUseCount() == 1;
}

auto Menu::GetImmutableState() const -> Pointer<State const>
{
    return _state.GetPointer();
}

auto Menu::GetMutableState() -> State&
{
    if (!_state)
    {
        _state = Shared<State>::Make();
    }

    if (!IsUnique())
    {
        _state = Shared<State>::Make(*_state);
    }
    return *_state;
}
}

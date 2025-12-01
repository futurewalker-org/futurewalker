// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Menu.hpp"
#include "Futurewalker.Application.MenuItem.hpp"

namespace FW_DETAIL_NS
{
auto Menu::GetItemCount() const -> SInt64
{
    if (auto state = GetImmutableState())
    {
        return static_cast<SInt64>(state->items.size());
    }
    return 0;
}

auto Menu::GetItemAt(SInt64 index) const -> Optional<MenuItem>
{
    if (auto state = GetImmutableState())
    {
        auto const itemsSize = static_cast<SInt64>(state->items.size());
        if (0 <= index && index < itemsSize)
        {
            return state->items[static_cast<std::size_t>(index)];
        }
    }
    return {};
}

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

// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.MenuItem.hpp"

namespace FW_DETAIL_NS
{
MenuItem::MenuItem(MenuItemType const type)
{
    SetType(type);
}

auto MenuItem::GetType() const -> MenuItemType
{
    if (auto const state = GetImmutableState())
    {
        return state->type;
    }
    return MenuItemType::Item;
}

auto MenuItem::SetType(MenuItemType const type) -> void
{
    GetMutableState().type = type;
}

auto MenuItem::GetId() const -> MenuItemId
{
    if (auto const state = GetImmutableState())
    {
        return state->id;
    }
    return 0u;
}

auto MenuItem::SetId(MenuItemId const id) -> void
{
    GetMutableState().id = id;
}

auto MenuItem::GetSubMenu() const -> MenuItemArray
{
    if (auto const state = GetImmutableState())
    {
        return state->subMenu;
    }
    return {};
}

auto MenuItem::SetSubMenu(MenuItemArray const& subMenu) -> void
{
    GetMutableState().subMenu = subMenu;
}

auto MenuItem::GetTitle() const -> String
{
    if (auto const state = GetImmutableState())
    {
        return state->title;
    }
    return String();
}

auto MenuItem::SetTitle(String const& title) -> void
{
    GetMutableState().title = title;
}

auto MenuItem::GetSubtitle() const -> String
{
    if (auto const state = GetImmutableState())
    {
        return state->subtitle;
    }
    return String();
}

auto MenuItem::SetSubtitle(String const& subtitle) -> void
{
    GetMutableState().subtitle = subtitle;
}

auto MenuItem::IsUnique() const -> Bool
{
    return _state.GetUseCount() == 1;
}

auto MenuItem::GetImmutableState() const -> Pointer<State const>
{
    return _state.GetPointer();
}

auto MenuItem::GetMutableState() -> State&
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

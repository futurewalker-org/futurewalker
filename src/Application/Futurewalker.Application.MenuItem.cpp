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

auto MenuItem::GetRole() const -> MenuItemRole
{
    return GetImmutableState()->role;
}

auto MenuItem::SetRole(MenuItemRole const role) -> void
{
    GetMutableState().role = role;
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

auto MenuItem::GetSubMenu() const -> Menu
{
    if (auto const state = GetImmutableState())
    {
        return state->subMenu;
    }
    return {};
}

auto MenuItem::SetSubMenu(Menu const& subMenu) -> void
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

auto MenuItem::GetLeadingIcon() const -> Icon
{
    if (auto const state = GetImmutableState())
    {
        return state->leadingIcon;
    }
    return Icon();
}

auto MenuItem::SetLeadingIcon(Icon const& icon) -> void
{
    GetMutableState().leadingIcon = icon;
}

auto MenuItem::GetTrailingIcon() const -> Icon
{
    if (auto const state = GetImmutableState())
    {
        return state->trailingIcon;
    }
    return Icon();
}

auto MenuItem::SetTrailingIcon(Icon const& icon) -> void
{
    GetMutableState().trailingIcon = icon;
}

auto MenuItem::GetAccessKey() const -> String
{
    if (auto const state = GetImmutableState())
    {
        return state->accessKey;
    }
    return String();
}

auto MenuItem::SetAccessKey(String const& key) -> void
{
    GetMutableState().accessKey = key;
}

auto MenuItem::GetAccessKeyModifiers() const -> ModifierKeyFlags
{
    if (auto const state = GetImmutableState())
    {
        return state->accessKeyModifiers;
    }
    return ModifierKeyFlags::None;
}

auto MenuItem::SetAccessKeyModifiers(ModifierKeyFlags const modifiers) -> void
{
    GetMutableState().accessKeyModifiers = modifiers;
}

auto MenuItem::GetCommandId() const -> CommandId
{
    if (auto const state = GetImmutableState())
    {
        return state->commandId;
    }
    return 0U;
}

auto MenuItem::SetCommandId(CommandId const commandId) -> void
{
    GetMutableState().commandId = commandId;
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

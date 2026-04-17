// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.MenuItemType.hpp"
#include "Futurewalker.Application.Menu.hpp"
#include "Futurewalker.Application.KeyEventType.hpp"
#include "Futurewalker.Application.Icon.hpp"

#include "Futurewalker.Action.CommandId.hpp"

#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Flags.hpp"

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

    auto GetRole() const -> MenuItemRole;
    auto SetRole(MenuItemRole const role) -> void;

    auto GetId() const -> MenuItemId;
    auto SetId(MenuItemId const id) -> void;

    auto GetSubMenu() const -> Menu;
    auto SetSubMenu(Menu const& subMenu) -> void;

    auto GetTitle() const -> String;
    auto SetTitle(String const& title) -> void;

    auto GetSubtitle() const -> String;
    auto SetSubtitle(String const& subtitle) -> void;

    auto GetLeadingIcon() const -> Icon;
    auto SetLeadingIcon(Icon const& icon) -> void;

    auto GetTrailingIcon() const -> Icon;
    auto SetTrailingIcon(Icon const& icon) -> void;

    auto GetAccessKey() const -> String;
    auto SetAccessKey(String const& key) -> void;

    auto GetAccessKeyModifiers() const -> Flags<ModifierKeyFlag>;
    auto SetAccessKeyModifiers(Flags<ModifierKeyFlag> const modifiers) -> void;

    auto GetCommandId() const -> CommandId;
    auto SetCommandId(CommandId const commandId) -> void;

private:
    struct State
    {
        MenuItemType type = MenuItemType::Item;
        MenuItemRole role = MenuItemRole::Other;
        MenuItemId id = 0u;
        Menu subMenu;
        String title;
        String subtitle;
        Icon leadingIcon;
        Icon trailingIcon;
        String accessKey;
        Flags<ModifierKeyFlag> accessKeyModifiers = ModifierKeyFlag::None;
        CommandId commandId = 0U;
    };
    Shared<State> _state;

private:
    auto IsUnique() const -> Bool;
    auto GetImmutableState() const -> Pointer<State const>;
    auto GetMutableState() -> State&;
};
}
}

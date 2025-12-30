// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.MenuBarType.hpp"
#include "Futurewalker.Component.Lamp.PopupMenuType.hpp"

#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.Menu.hpp"
#include "Futurewalker.Application.MenuItem.hpp"
#include "Futurewalker.Application.FlexLayoutType.hpp"

#include <vector>

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
class MenuBar : public View
{
public:
    static auto Make() -> Shared<MenuBar>;
    static auto MakeWithMenu(Menu const& menu) -> Shared<MenuBar>;

    MenuBar(PassKey<View> key);

    auto GetMenu() const -> Menu;
    auto SetMenu(Menu const& menu) -> void;

protected:
    auto Initialize() -> void override;
    auto ReceiveViewEvent(Event<>& event) -> Async<Bool>;
    auto ReceivePopupMenuEvent(Event<>& event) -> Async<Bool>;

private:
    struct MenuItemInfo
    {
        MenuItem item;
        Shared<View> button;
    };
    auto FindMenuItemInfo(Shared<View> const& button) const -> Optional<MenuItemInfo>;
    auto CreatePopup() -> void;
    auto DestroyPopup() -> void;
    auto UpdatePopup(MenuItemInfo const& info) -> void;
    auto BuildView() -> void;

private:
    Menu _menu;
    Shared<PopupMenu> _popup;
    Shared<FlexLayout> _row;
    std::vector<MenuItemInfo> _itemInfo;
};
}
}

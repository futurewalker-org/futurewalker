// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.MenuViewType.hpp"
#include "Futurewalker.Component.Lamp.MenuViewStyle.hpp"
#include "Futurewalker.Component.Lamp.MenuViewEvent.hpp"
#include "Futurewalker.Component.Lamp.MenuItemViewBuilder.hpp"
#include "Futurewalker.Component.Lamp.PopupMenu.hpp"

#include "Futurewalker.Application.FlexLayoutType.hpp"
#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.Menu.hpp"
#include "Futurewalker.Application.MenuItem.hpp"
#include "Futurewalker.Application.PaddingView.hpp"
#include "Futurewalker.Application.ContainerView.hpp"
#include "Futurewalker.Application.PopupType.hpp"

#include "Futurewalker.Action.CommandObserverType.hpp"

#include "Futurewalker.Attribute.AttributeAccessor.hpp"

#include "Futurewalker.Async.TaskHandle.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Menu view.
///
/// Displays list of menu items.
///
class MenuView : public View
{
public:
    static auto Make() -> Shared<MenuView>;
    static auto MakeWithMenu(Menu const& menu) -> Shared<MenuView>;

    MenuView(PassKey<View> key);

    auto GetMenu() const -> Menu;
    auto SetMenu(Menu const& menu) -> void;

    auto GetItemViewBuilder() const -> MenuItemViewBuilder;
    auto SetItemViewBuilder(MenuItemViewBuilder const& builder) -> void;

protected:
    auto Initialize() -> void override;
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;
    auto ReceiveCommandEvent(Event<>& event) -> Async<Bool>;
    auto ReceivePopupEvent(Event<>& event) -> Async<Bool>;

private:
    struct ItemData
    {
        MenuItem item;
        Unique<CommandObserver> command;
        Shared<View> view;
    };
    auto CreatePopup() -> void;
    auto DestroyPopup() -> void;
    auto UpdatePopup(SInt64 const index) -> void;
    auto RebuildData(Menu const& menu) -> void;
    auto RebuildView() -> void;
    auto UpdateStyle() -> void;
    auto UpdateEnabled(ItemData const& data) -> void;

private:
    Shared<ContainerView> _container;
    Shared<PaddingView> _padding;
    Shared<FlexLayout> _flex;
    std::vector<ItemData> _data;
    MenuItemViewBuilder _builder;
    Shared<PopupMenu> _popup;
    SInt64 _index = -1;
    TaskHandle<void> _createPopupTask;
};
}
}

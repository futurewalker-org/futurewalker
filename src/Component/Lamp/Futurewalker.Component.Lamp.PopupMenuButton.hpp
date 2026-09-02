// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.PopupMenuButtonType.hpp"
#include "Futurewalker.Component.Lamp.PopupMenuButtonEvent.hpp"
#include "Futurewalker.Component.Lamp.PopupMenuButtonStyle.hpp"
#include "Futurewalker.Component.Lamp.PopupMenuType.hpp"
#include "Futurewalker.Component.Lamp.ButtonRenderViewType.hpp"
#include "Futurewalker.Component.Lamp.MenuButtonViewType.hpp"

#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.Menu.hpp"
#include "Futurewalker.Application.CornerRadius.hpp"
#include "Futurewalker.Application.PaddingViewType.hpp"
#include "Futurewalker.Application.PopupType.hpp"

#include "Futurewalker.Attribute.AttributeArg.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Button which displays popup menu when pressed.
///
class PopupMenuButton : public View
{
public:
    static auto Make() -> Shared<PopupMenuButton>;
    static auto MakeWithContent(Shared<View> const& content) -> Shared<PopupMenuButton>;

    auto GetContent() -> Shared<View>;
    auto SetContent(Shared<View> const& content) -> void;

    auto GetMenu() const -> Menu;
    auto SetMenu(Menu const& menu) -> void;

    auto SetAnchorEdge(PopupAnchorEdge edge) -> void;
    auto SetAnchorAlignment(PopupAnchorAlignment alignment) -> void;

    PopupMenuButton(PassKey<View> key);

protected:
    auto Initialize() -> void override;

private:
    auto ReceiveEvent(Event<>& event) -> Bool;
    auto CreatePopup() -> void;
    auto DestroyPopup() -> void;
    auto UpdatePopup() -> void;

private:
    Menu _menu;
    PopupAnchorEdge _anchorEdge = PopupAnchorEdge::Bottom;
    PopupAnchorAlignment _anchorAlignment = PopupAnchorAlignment::Start;
    Shared<PopupMenu> _popupMenu;
    Shared<PaddingView> _paddingView;
    Shared<MenuButtonView> _buttonView;
};
}
}

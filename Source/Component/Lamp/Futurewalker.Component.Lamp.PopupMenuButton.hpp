// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.PopupMenuButtonType.hpp"
#include "Futurewalker.Component.Lamp.PopupMenuButtonEvent.hpp"
#include "Futurewalker.Component.Lamp.PopupMenuButtonStyle.hpp"
#include "Futurewalker.Component.Lamp.PopupMenuType.hpp"
#include "Futurewalker.Component.Lamp.ButtonRenderViewType.hpp"

#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.Menu.hpp"
#include "Futurewalker.Application.ButtonViewType.hpp"
#include "Futurewalker.Application.CornerRadius.hpp"
#include "Futurewalker.Application.PaddingViewType.hpp"

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

    auto SetAnchorEdge(PopupMenuAnchorEdge edge) -> void;
    auto SetAnchorAlignment(PopupMenuAnchorAlignment alignment) -> void;

    PopupMenuButton(PassKey<View> key);

protected:
    auto Initialize() -> void override;

private:
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;
    auto CreatePopup() -> void;
    auto DestroyPopup() -> void;
    auto UpdatePopup() -> void;
    auto IsDown() const -> Bool;
    auto IsEnter() const -> Bool; 
    auto SetDown(Bool const down) -> void;
    auto SetEnter(Bool const enter) -> void;

private:
    Menu _menu;
    PopupMenuAnchorEdge _anchorEdge = PopupMenuAnchorEdge::Bottom;
    PopupMenuAnchorAlignment _anchorAlignment = PopupMenuAnchorAlignment::Start;
    Shared<PopupMenu> _popupMenu;
    Shared<PaddingView> _paddingView;
    Shared<ButtonRenderView> _buttonRenderView;
};
}
}

// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.MenuItemButtonType.hpp"
#include "Futurewalker.Component.Lamp.MenuItemButtonEvent.hpp"
#include "Futurewalker.Component.Lamp.ButtonRenderViewType.hpp"

#include "Futurewalker.Application.ContainerViewType.hpp"
#include "Futurewalker.Application.View.hpp"

#include "Futurewalker.Attribute.AttributeAccessor.hpp"

#include "Futurewalker.Action.CommandId.hpp"
#include "Futurewalker.Action.CommandObserver.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Button for menu item.
///
class MenuItemButton : public View
{
public:
    static auto Make() -> Shared<MenuItemButton>;
    static auto MakeWithContent(Shared<View> const& content) -> Shared<MenuItemButton>;

    MenuItemButton(PassKey<View> key);

    auto GetContent() -> Shared<View>;
    auto SetContent(Shared<View> const& content) -> void;

protected:
    auto Initialize() -> void override;
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;
    auto ReceiveAttributeEvent(Event<>& event) -> Async<Bool>;

private:
    auto SetDown(Bool const down) -> void;
    auto SetEnter(Bool const enter) -> void;

private:
    Shared<ContainerView> _container;
    Shared<ButtonRenderView> _buttonView;
    AttributeAccessor<RGBAColor> _iconColor;
    AttributeAccessor<RGBAColor> _iconDisabledColor;
    AttributeAccessor<RGBAColor> _textColor;
    AttributeAccessor<RGBAColor> _textDisabledColor;
    AttributeAccessor<Channel> _iconAlpha;
    AttributeAccessor<Channel> _iconDisabledAlpha;
    AttributeAccessor<Channel> _textAlpha;
    AttributeAccessor<Channel> _textDisabledAlpha;
};
}
}

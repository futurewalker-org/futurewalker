// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.MenuButtonViewType.hpp"
#include "Futurewalker.Component.Lamp.MenuButtonViewEvent.hpp"
#include "Futurewalker.Component.Lamp.ButtonRenderViewType.hpp"

#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.ContainerViewType.hpp"
#include "Futurewalker.Application.CornerRadius.hpp"

#include "Futurewalker.Attribute.AttributeArg.hpp"

#include "Futurewalker.Color.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
class MenuButtonView : public View
{
public:
    static auto Make() -> Shared<MenuButtonView>;
    static auto MakeWithContent(Shared<View> const& content) -> Shared<MenuButtonView>;

    auto GetContent() -> Shared<View>;
    auto SetContent(Shared<View> const& content) -> void;

    auto SetBackgroundColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetBackgroundAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetDisabledBackgroundColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetDisabledBackgroundAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetBorderColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetBorderAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetBorderWidth(AttributeArg<Dp> const& width) -> void;
    auto SetDisabledBorderColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetDisabledBorderAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetDisabledBorderWidth(AttributeArg<Dp> const& width) -> void;
    auto SetFocusedBorderColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetFocusedBorderAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetFocusedBorderWidth(AttributeArg<Dp> const& width) -> void;
    auto SetHighlightColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetHoverHighlightAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetPressHighlightAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetCornerRadius(AttributeArg<CornerRadius> const& radius) -> void;

    MenuButtonView(PassKey<View> key);

protected:
    auto Initialize() -> void override;
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;

private:
    Bool _pointerDown = false;
    Bool _pointerEnter = false;
    Bool _keyDown = false;
    Shared<ContainerView> _container;
    Shared<ButtonRenderView> _buttonView;
};
}
}

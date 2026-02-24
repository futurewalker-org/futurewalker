// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.ButtonViewType.hpp"
#include "Futurewalker.Component.Lamp.ButtonViewStyle.hpp"
#include "Futurewalker.Component.Lamp.ButtonViewEvent.hpp"
#include "Futurewalker.Component.Lamp.ButtonRenderViewType.hpp"

#include "Futurewalker.Application.TapGestureViewType.hpp"
#include "Futurewalker.Application.HoverGestureViewType.hpp"
#include "Futurewalker.Application.ContainerViewType.hpp"
#include "Futurewalker.Application.CornerRadius.hpp"
#include "Futurewalker.Application.View.hpp"

#include "Futurewalker.Attribute.AttributeArg.hpp"

#include "Futurewalker.Color.hpp"

#include "Futurewalker.Core.Flags.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
class ButtonView : public View
{
public:
    static auto Make() -> Shared<ButtonView>;
    static auto MakeWithContent(Shared<View> contentView) -> Shared<ButtonView>;

    auto GetContent() -> Shared<View>;
    auto SetContent(Shared<View> content) -> void;

    auto SetActionFlags(Flags<ButtonViewActionFlag> const flags) -> void;
    auto GetActionFlags() const -> Flags<ButtonViewActionFlag>;

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

    ButtonView(PassKey<View> key);

protected:
    auto Initialize() -> void override;
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;

private:
    Shared<ContainerView> _containerView;
    Shared<TapGestureView> _tapGestureView;
    Shared<HoverGestureView> _hoverGestureView;
    Shared<ButtonRenderView> _renderView;
    Flags<ButtonViewActionFlag> _actionFlags = ButtonViewActionFlag::None;
    Bool _tapDown = false;
    Bool _keyDown = false;
    Bool _hoverEnter = false;
};
}
}

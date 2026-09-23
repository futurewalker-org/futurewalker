// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.CheckBoxRenderViewType.hpp"
#include "Futurewalker.Component.Lamp.CheckBoxType.hpp"

#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.Icon.hpp"
#include "Futurewalker.Application.CornerRadius.hpp"

#include "Futurewalker.Attribute.AttributeAccessor.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
class CheckBoxRenderView : public View
{
public:
    static auto Make() -> Shared<CheckBoxRenderView>;

    CheckBoxRenderView(PassKey<View>);

    auto SetState(CheckBoxState const state) -> void;
    auto SetDown(Bool const down) -> void;
    auto SetEnter(Bool const enter) -> void;
    auto SetFocused(Bool const focused, FocusReason const reason) -> void;
    auto SetEnabled(Bool const enabled) -> void;

    auto SetCornerRadius(AttributeArg<CornerRadius> const& cornerRadius) -> void;
    auto SetBackgroundColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetBackgroundColorDisabled(AttributeArg<RGBAColor> const& color) -> void;
    auto SetBackgroundAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetBackgroundAlphaDisabled(AttributeArg<Channel> const& alpha) -> void;
    auto SetBorderColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetBorderColorDisabled(AttributeArg<RGBAColor> const& color) -> void;
    auto SetBorderColorFocused(AttributeArg<RGBAColor> const& color) -> void;
    auto SetBorderAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetBorderAlphaDisabled(AttributeArg<Channel> const& alpha) -> void;
    auto SetBorderAlphaFocused(AttributeArg<Channel> const& alpha) -> void;
    auto SetCheckMarkColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetCheckMarkColorDisabled(AttributeArg<RGBAColor> const& color) -> void;
    auto SetCheckMarkAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetCheckMarkAlphaDisabled(AttributeArg<Channel> const& alpha) -> void;

protected:
    auto Initialize() -> void override;
    auto Draw(DrawScope& scope) -> void override;

private:
    auto ReceiveAttributeEvent(Event<>& event) -> Bool;
    auto InternalGetBackgroundColor() const -> RGBAColor;
    auto InternalGetBorderColor() const -> RGBAColor;
    auto InternalGetCheckMarkColor() const -> RGBAColor;
    auto InternalGetBorderColorFocused() const -> RGBAColor;

private:
    CheckBoxState _state = CheckBoxState::Unchecked;
    Bool _down = false;
    Bool _enter = false;
    Bool _focused = false;
    Bool _enabled = false;
    FocusReason _focusReason = FocusReason::None;
    AttributeAccessor<CornerRadius> _cornerRadius;
    AttributeAccessor<RGBAColor> _backgroundColor;
    AttributeAccessor<RGBAColor> _backgroundColorDisabled;
    AttributeAccessor<Channel> _backgroundAlpha;
    AttributeAccessor<Channel> _backgroundAlphaDisabled;
    AttributeAccessor<RGBAColor> _borderColor;
    AttributeAccessor<RGBAColor> _borderColorDisabled;
    AttributeAccessor<RGBAColor> _borderColorFocused;
    AttributeAccessor<Channel> _borderAlpha;
    AttributeAccessor<Channel> _borderAlphaDisabled;
    AttributeAccessor<Channel> _borderAlphaFocused;
    AttributeAccessor<RGBAColor> _checkMarkColor;
    AttributeAccessor<RGBAColor> _checkMarkColorDisabled;
    AttributeAccessor<Channel> _checkMarkAlpha;
    AttributeAccessor<Channel> _checkMarkAlphaDisabled;
    Icon _checkMarkIcon;
    Icon _indeterminateIcon;
};
}
}

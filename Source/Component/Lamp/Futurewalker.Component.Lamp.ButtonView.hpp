// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.ButtonViewType.hpp"
#include "Futurewalker.Component.Lamp.ButtonViewStyle.hpp"

#include "Futurewalker.Application.ButtonViewType.hpp"
#include "Futurewalker.Application.ButtonViewEvent.hpp"
#include "Futurewalker.Application.CornerRadius.hpp"
#include "Futurewalker.Application.View.hpp"

#include "Futurewalker.Attribute.AttributeAccessor.hpp"

#include "Futurewalker.Color.hpp"

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

    auto SetActionFlags(ButtonViewActionFlags const flags) -> void;
    auto GetActionFlags() const -> ButtonViewActionFlags;

    auto SetBackgroundColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetBackgroundAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetDisabledBackgroundColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetDisabledBackgroundAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetBorderColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetBorderAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetDisabledBorderColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetDisabledBorderAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetHighlightColor(AttributeArg<RGBAColor> const& color) -> void;
    auto SetHoverHighlightAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetPressHighlightAlpha(AttributeArg<Channel> const& alpha) -> void;
    auto SetCornerRadius(AttributeArg<CornerRadius> const& radius) -> void;
    auto SetBorderWidth(AttributeArg<Dp> const& width) -> void;

    ButtonView(PassKey<View> key);

protected:
    auto Initialize() -> void override;
    auto Draw(DrawScope& scope) -> void override;
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;
    auto ReceiveAttributeEvent(Event<>& event) -> Async<Bool>;
    auto SetDown(const Bool down) -> void;
    auto SetEnter(const Bool enter) -> void;

private:
    Shared<::FW_NS::ButtonView> _buttonView;
    AttributeAccessor<RGBAColor> _backgroundColor;
    AttributeAccessor<Channel> _backgroundAlpha;
    AttributeAccessor<RGBAColor> _disabledBackgroundColor;
    AttributeAccessor<Channel> _disabledBackgroundAlpha;
    AttributeAccessor<RGBAColor> _borderColor;
    AttributeAccessor<Channel> _borderAlpha;
    AttributeAccessor<RGBAColor> _disabledBorderColor;
    AttributeAccessor<Channel> _disabledBorderAlpha;
    AttributeAccessor<RGBAColor> _highlightColor;
    AttributeAccessor<Channel> _hoverHighlightAlpha;
    AttributeAccessor<Channel> _pressHighlightAlpha;
    AttributeAccessor<CornerRadius> _cornerRadius;
    AttributeAccessor<Dp> _borderWidth;
    Bool _down = false;
    Bool _enter = false;
};
}
}

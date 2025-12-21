// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.ButtonRenderViewType.hpp"

#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.ContainerViewType.hpp"
#include "Futurewalker.Application.CornerRadius.hpp"

#include "Futurewalker.Attribute.AttributeAccessor.hpp"

#include "Futurewalker.Color.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief A view which renders a button state.
///
class ButtonRenderView : public View
{
public:
    static auto Make() -> Shared<ButtonRenderView>;
    static auto MakeWithContent(Shared<View> const& content) -> Shared<ButtonRenderView>;

    ButtonRenderView(PassKey<View> key);

    auto GetContent() -> Shared<View>;
    auto SetContent(Shared<View> const& content) -> void;

    auto SetDown(Bool const down) -> void;
    auto SetEnter(Bool const enter) -> void;

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

protected:
    auto Initialize() -> void override;
    auto Draw(DrawScope& scope) -> void override;
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;
    auto ReceiveAttributeEvent(Event<>& event) -> Async<Bool>;

private:
    Bool _down = false;
    Bool _enter = false;
    Shared<ContainerView> _container;
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
};
}
}

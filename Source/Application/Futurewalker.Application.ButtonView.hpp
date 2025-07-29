// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.ButtonViewType.hpp"
#include "Futurewalker.Application.ButtonViewEvent.hpp"
#include "Futurewalker.Application.ButtonViewStyle.hpp"
#include "Futurewalker.Application.TapGestureViewType.hpp"
#include "Futurewalker.Application.HoverGestureViewType.hpp"
#include "Futurewalker.Application.View.hpp"

#include "Futurewalker.Attribute.AttributeAccessor.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief A container view that enables button behavior.
///
class ButtonView : public View
{
public:
    static auto Make() -> Shared<ButtonView>;
    static auto MakeWithContent(Shared<View> contentView) -> Shared<ButtonView>;

    auto SetContent(Shared<View> content) -> void;
    auto GetContent() -> Shared<View>;

    ButtonView(PassKey<View> key);

protected:
    auto Initialize() -> void override;
    auto Draw(DrawScope& scope) -> void override;

private:
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;
    auto ReceiveAttributeEvent(Event<>& event) -> Async<Bool>;

    auto SetDown(const Bool down) -> void;
    auto SetEnter(const Bool enter) -> void;

private:
    Shared<TapGestureView> _tapGestureView;
    Shared<HoverGestureView> _hoverGestureView;
    AttributeAccessor<RGBAColor> _backgroundColor;
    AttributeAccessor<RGBAColor> _highlightColor;
    AttributeAccessor<RGBAColor> _borderColor;
    AttributeAccessor<AlphaChannel> _hoverHighlightAlpha;
    AttributeAccessor<AlphaChannel> _pressHighlightAlpha;
    AttributeAccessor<CornerRadius> _cornerRadius;
    AttributeAccessor<Dp> _borderWidth;
    Bool _down = false;
    Bool _enter = false;
};
}
}

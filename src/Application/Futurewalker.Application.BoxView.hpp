// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.BoxViewType.hpp"
#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.CornerRadius.hpp"

#include "Futurewalker.Attribute.AttributeAccessor.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief A view that draws a box and border.
///
/// ## Layout behavior
///
/// BoxView sizes itself to wrap its content.
///
class BoxView : public View
{
public:
    static auto Make() -> Shared<BoxView>;
    static auto MakeWithContent(Shared<View> const& content) -> Shared<BoxView>;

    BoxView(PassKey<View> key);

    auto GetContent() -> Shared<View>;
    auto GetContent() const -> Shared<View const>;
    auto SetContent(Shared<View> const& content) -> void;

    auto SetBackgroundColor(AttributeArg<RGBAColor> color) -> void;
    auto SetBackgroundAlpha(AttributeArg<Channel> alpha) -> void;
    auto SetBorderColor(AttributeArg<RGBAColor> color) -> void;
    auto SetBorderAlpha(AttributeArg<Channel> alpha) -> void;
    auto SetBorderWidth(AttributeArg<Dp> width) -> void;
    auto SetCornerRadius(AttributeArg<CornerRadius> radius) -> void;

protected:
    auto Initialize() -> void override;
    auto Draw(DrawScope& scope) -> void override;
    auto ReceiveAttributeEvent(Event<>& event) -> Async<Bool>;

private:
    AttributeAccessor<RGBAColor> _backgroundColor;
    AttributeAccessor<Channel> _backgroundAlpha;
    AttributeAccessor<RGBAColor> _borderColor;
    AttributeAccessor<Channel> _borderAlpha;
    AttributeAccessor<Dp> _borderWidth;
    AttributeAccessor<CornerRadius> _cornerRadius;
};
}
}

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
/// @brief A view that draws a box with background and border.
///
/// ## Layout behavior
///
/// Under bounded constraints, BoxView will fill the available space.
/// Under unbounded constraints, BoxView will size itself to zero.
///
class BoxView : public View
{
public:
    static auto Make() -> Shared<BoxView>;

    BoxView(PassKey<View> key);

    auto SetBackgroundColor(AttributeArg<RGBAColor> color) -> void;
    auto SetBackgroundAlpha(AttributeArg<Channel> alpha) -> void;
    auto SetBorderColor(AttributeArg<RGBAColor> color) -> void;
    auto SetBorderAlpha(AttributeArg<Channel> alpha) -> void;
    auto SetBorderWidth(AttributeArg<Dp> width) -> void;
    auto SetCornerRadius(AttributeArg<CornerRadius> radius) -> void;

protected:
    auto Initialize() -> void override;
    auto Measure(MeasureScope& scope) -> void override;
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

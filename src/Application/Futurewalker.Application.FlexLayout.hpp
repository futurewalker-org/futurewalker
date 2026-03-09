// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.FlexLayoutType.hpp"
#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.EdgeInsets.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Linear layout similar to CSS Flexbox.
///
class FlexLayout final : public View
{
public:
    static auto Make() -> Shared<FlexLayout>;

    FlexLayout(PassKey<View> key);

    auto GetAddedChildCount() const -> SInt64;
    auto GetAddedChildAt(SInt64 const index) -> Shared<View>;
    auto GetAddedChildIndex(Shared<View> const& child) const -> Optional<SInt64>;
    auto AddChild(Shared<View> const& content) -> void;
    auto RemoveChildren() -> void;
    auto Contains(Shared<View> const& child) const -> Bool;

    auto GetDirection() const -> FlexLayoutDirection;
    auto SetDirection(FlexLayoutDirection const direction) -> void;

    auto GetMainAxisSize() const -> FlexLayoutMainAxisSize;
    auto SetMainAxisSize(FlexLayoutMainAxisSize const size) -> void;

    auto GetCrossAxisSize() const -> FlexLayoutCrossAxisSize;
    auto SetCrossAxisSize(FlexLayoutCrossAxisSize const size) -> void;

    auto GetMainAxisAlignment() const -> FlexLayoutMainAxisAlignment;
    auto SetMainAxisAlignment(FlexLayoutMainAxisAlignment const alignment) -> void;

    auto GetCrossAxisAlignment() const -> FlexLayoutCrossAxisAlignment;
    auto SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment const alignment) -> void;

    static auto GetChildGrowFactor(ReferenceArg<View const> child) -> FlexFactor;
    static auto SetChildGrowFactor(ReferenceArg<View> child, FlexFactor const growFactor) -> void;

    static auto GetChildShrinkFactor(ReferenceArg<View const> child) -> FlexFactor;
    static auto SetChildShrinkFactor(ReferenceArg<View> child, FlexFactor const shrinkFactor) -> void;

    static auto GetChildMargin(ReferenceArg<View const> child) -> EdgeInsets;
    static auto SetChildMargin(ReferenceArg<View> child, EdgeInsets const& margin) -> void;

    static auto GetChildMainAxisFlexibility(ReferenceArg<View const> child) -> FlexLayoutMainAxisFlexibility;
    static auto SetChildMainAxisFlexibility(ReferenceArg<View> child, FlexLayoutMainAxisFlexibility const flexibility) -> void;

protected:
    auto Initialize() -> void override;
    auto Measure(MeasureScope& scope) -> void override;
    auto Arrange(ArrangeScope& scope) -> void override;
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;

private:
    auto GetMainAxisConstraints(MeasureScope const& scope) const -> AxisConstraints;
    auto GetCrossAxisConstraints(MeasureScope const& scope) const -> AxisConstraints;

    auto MeasureChild(MeasureScope& scope, View& child, AxisConstraints const& main, AxisConstraints const& cross) const -> Size<Dp>;
    auto ArrangeChild(ArrangeScope& scope, View& child, Dp const main, Dp const cross) -> void;

    auto GetMainAxisLength(Size<Dp> const& size) const -> Dp;
    auto GetCrossAxisLength(Size<Dp> const& size) const -> Dp;

    auto GetMainAxisLength(EdgeInsets const& margin) const -> Dp;
    auto GetCrossAxisLength(EdgeInsets const& margin) const -> Dp;

    auto GetMainAxisOffset(EdgeInsets const& margin) const -> Dp;
    auto GetCrossAxisOffset(EdgeInsets const& margin) const -> Dp;

    auto MeasureMainAxisSize(AxisConstraints const& main, Dp const totalMain) const -> Dp;
    auto MeasureCrossAxisSize(AxisConstraints const& cross, Dp const maxCross) const -> Dp;

    auto ArrangeMainAxisOffset(Dp& offset, Dp const main, Dp const size, Dp const total) const -> Dp;
    auto ArrangeCrossAxisOffset(Dp const cross, Dp const size) const -> Dp;

    auto SetMeasuredSize(MeasureScope& scope, Dp const main, Dp const cross) -> void;

private:
    FlexLayoutDirection _direction = FlexLayoutDirection::Column;
    FlexLayoutMainAxisSize _mainAxisSize = FlexLayoutMainAxisSize::Min;
    FlexLayoutCrossAxisSize _crossAxisSize = FlexLayoutCrossAxisSize::Min;
    FlexLayoutMainAxisAlignment _mainAxisAlignment = FlexLayoutMainAxisAlignment::Start;
    FlexLayoutCrossAxisAlignment _crossAxisAlignment = FlexLayoutCrossAxisAlignment::Center;
};
}
}

// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.ViewLayoutInfoType.hpp"
#include "Futurewalker.Application.MeasureParameter.hpp"
#include "Futurewalker.Application.ArrangeParameter.hpp"
#include "Futurewalker.Application.LayoutDirection.hpp"
#include "Futurewalker.Application.ViewType.hpp" 

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Layout info.
///
class ViewLayoutInfo
{
public:
    auto GetFrameRect() const -> Rect<Dp>;
    auto GetContentRect() const -> Rect<Dp>;

    auto InvalidatePath() -> Bool;

    auto BeginMeasure(MeasureParameter const& parameter) -> Bool;
    auto EndMeasure() -> void;
    auto IsMeasuring() const -> Bool;

    auto BeginArrange(ArrangeParameter const& parameter) -> Bool;
    auto EndArrange() -> void;
    auto IsArranging() const -> Bool;

    auto GetMeasuredSize() const -> Size<Dp>;
    auto SetMeasuredSize(MeasureParameter const& parameter, Size<Dp> const& size) -> void;

    auto GetLayoutDirection() const -> LayoutDirection;
    auto SetLayoutDirection(LayoutDirection const layoutDirection) -> void;

    auto GetRawLayoutDirection() const -> ViewLayoutDirection;
    auto SetRawLayoutDirection(ViewLayoutDirection const layoutDirection) -> void;

private:
    auto IsMeasureRequired() const -> Bool;
    auto IsMeasureNeeded(MeasureParameter const& parameter) const -> Bool;
    auto IsArrangeRequired() const -> Bool;

private:
    Rect<Dp> _frameRect;
    Rect<Dp> _contentRect;
    Point<Dp> _arrangedPosition;
    Bool _measureRequired = true;
    Bool _arrangeRequired = true;
    Bool _measuring = false;
    Bool _arranging = false;

    MeasureParameter _lastMeasureParameter;

    Bool _measuredSizeSet = false;
    Bool _measuredIntrinsicSizeSet = false;

    struct MeasuredSize
    {
        AxisConstraints widthConstraints;
        AxisConstraints heightConstraints;
        Size<Dp> size;
    };
    MeasuredSize _measuredSize;
    MeasuredSize _measuredIntrinsicSize;

    LayoutDirection _layoutDirection = LayoutDirection::LeftToRight;
    ViewLayoutDirection _rawLayoutDirection = ViewLayoutDirection::Inherited;
};
}

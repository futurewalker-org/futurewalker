// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.FlexLayout.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.ArrangeScope.hpp"
#include "Futurewalker.Application.ViewLayoutFunction.hpp"
#include "Futurewalker.Application.ViewEvent.hpp"

#include "Futurewalker.Core.PropertyStore.hpp"

namespace FW_DETAIL_NS
{
namespace
{
UniqueId ViewPropertyIdFlexGrowFactor;
UniqueId ViewPropertyIdFlexShrinkFactor;
UniqueId ViewPropertyIdMargin;
UniqueId ViewPropertyIdMainAxisFlexibility;

class InvalidateFromChild final : public ViewEvent::Notify
{
};

auto SendInvalidateFromChild(View& child) -> void
{
    auto event = Event(InvalidateFromChild());
    child.SendEventDetached(event);
}
}

///
/// @brief Make instance.
///
auto FlexLayout::Make() -> Shared<FlexLayout>
{
    return View::MakeDerived<FlexLayout>();
}

///
/// @brief Constructor.
///
FlexLayout::FlexLayout(PassKey<View> key)
  : View(key)
{
}

///
/// @brief Get number of added views.
///
auto FlexLayout::GetAddedChildCount() const -> SInt64
{
    return GetChildCount();
}

///
/// @brief Get added view at specified index.
///
auto FlexLayout::GetAddedChildAt(SInt64 const index) -> Shared<View>
{
    return GetChildAt(index);
}

///
/// @brief Add view to layout.
///
auto FlexLayout::AddChild(Shared<View> const& content) -> void
{
    AddChildBack(content);
}

///
/// @brief Get direction.
///
auto FlexLayout::GetDirection() const -> FlexLayoutDirection
{
    return _direction;
}

///
/// @brief Set direction.
///
auto FlexLayout::SetDirection(FlexLayoutDirection const direction) -> void
{
    if (_direction != direction)
    {
        _direction = direction;
        InvalidateLayout();
    }
}

///
/// @brief Get main axis size.
///
auto FlexLayout::GetMainAxisSize() const -> FlexLayoutMainAxisSize
{
    return _mainAxisSize;
}

///
/// @brief Set main axis size.
///
auto FlexLayout::SetMainAxisSize(FlexLayoutMainAxisSize const size) -> void
{
    if (_mainAxisSize != size)
    {
        _mainAxisSize = size;
        InvalidateLayout();
    }
}

///
/// @brief Get cross axis size.
///
auto FlexLayout::GetCrossAxisSize() const -> FlexLayoutCrossAxisSize
{
    return _crossAxisSize;
}

///
/// @brief Set cross axis size.
///
auto FlexLayout::SetCrossAxisSize(FlexLayoutCrossAxisSize const size) -> void
{
    if (_crossAxisSize != size)
    {
        _crossAxisSize = size;
        InvalidateLayout();
    }
}

///
/// @brief Get main axis alignment.
///
auto FlexLayout::GetMainAxisAlignment() const -> FlexLayoutMainAxisAlignment
{
    return _mainAxisAlignment;
}

///
/// @brief Set main axis alignment.
///
auto FlexLayout::SetMainAxisAlignment(FlexLayoutMainAxisAlignment const alignment) -> void
{
    if (_mainAxisAlignment != alignment)
    {
        _mainAxisAlignment = alignment;
        InvalidateLayout();
    }
}

///
/// @brief Get cross axis alignment.
///
auto FlexLayout::GetCrossAxisAlignment() const -> FlexLayoutCrossAxisAlignment
{
    return _crossAxisAlignment;
}

///
/// @brief Set cross axis alignment.
///
auto FlexLayout::SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment const alignment) -> void
{
    if (_crossAxisAlignment != alignment)
    {
        _crossAxisAlignment = alignment;
        InvalidateLayout();
    }
}

///
/// @brief Get child grow factor.
///
auto FlexLayout::GetChildGrowFactor(ReferenceArg<View const> child) -> FlexFactor
{
    if (child)
    {
        if (auto const factor = PropertyStore::GetValue<FlexFactor>(*child, ViewPropertyIdFlexGrowFactor))
        {
            return *factor;
        }
    }
    return 0.0;
}

///
/// @brief Set child grow factor.
///
/// @param[in] child Direct child of FlexLayout.
/// @param[in] growFactor Finite, positive grow factor.
///
auto FlexLayout::SetChildGrowFactor(ReferenceArg<View> child, FlexFactor const growFactor) -> void
{
    if (child && FlexFactor::IsFinite(growFactor))
    {
        PropertyStore::SetValue(*child, ViewPropertyIdFlexGrowFactor, FlexFactor::Max(0, growFactor));
        SendInvalidateFromChild(*child);
    }
}

///
/// @brief Get child shrink factor.
///
auto FlexLayout::GetChildShrinkFactor(ReferenceArg<View const> child) -> FlexFactor
{
    if (auto const factor = PropertyStore::GetValue<FlexFactor>(*child, ViewPropertyIdFlexShrinkFactor))
    {
        return *factor;
    }
    return 0.0;
}

///
/// @brief Set child shrink factor.
///
/// @param[in] child Direct child of FlexLayout.
/// @param[in] shrinkFactor Finite, positive shrink factor.
///
auto FlexLayout::SetChildShrinkFactor(ReferenceArg<View> child, FlexFactor const shrinkFactor) -> void
{
    if (child && FlexFactor::IsFinite(shrinkFactor))
    {
        PropertyStore::SetValue(*child, ViewPropertyIdFlexShrinkFactor, FlexFactor::Max(0, shrinkFactor));
        SendInvalidateFromChild(*child);
    }
}

///
/// @brief Set child margin.
///
auto FlexLayout::GetChildMargin(ReferenceArg<View const> child) -> EdgeInsets
{
    if (child)
    {
        if (auto const margin = PropertyStore::GetValue<EdgeInsets>(*child, ViewPropertyIdMargin))
        {
            return *margin;
        }
    }
    return {};
}

///
/// @brief Set child margin.
///
/// @param[in] child Direct child of FlexLayout.
/// @param[in] margin Finite, positive margin value.
///
auto FlexLayout::SetChildMargin(ReferenceArg<View> child, EdgeInsets const& margin) -> void
{
    if (child)
    {
        PropertyStore::SetValue(*child, ViewPropertyIdMargin, margin);
        SendInvalidateFromChild(*child);
    }
}

///
/// @brief Get child main axis flexibility.
///
auto FlexLayout::GetChildMainAxisFlexibility(ReferenceArg<View const> child) -> FlexLayoutChildMainAxisFlexibility
{
    if (child)
    {
        if (auto const flexibility = PropertyStore::GetValue<FlexLayoutChildMainAxisFlexibility>(*child, ViewPropertyIdMainAxisFlexibility))
        {
            return *flexibility;
        }
    }
    return FlexLayoutChildMainAxisFlexibility::Flexible;
}

///
/// @brief Set child main axis flexibility.
///
/// @param[in] child Direct child of FlexLayout.
/// @param[in] flexibility Main axis flexibility.
///
auto FlexLayout::SetChildMainAxisFlexibility(ReferenceArg<View> child, FlexLayoutChildMainAxisFlexibility const flexibility) -> void
{
    if (child)
    {
        PropertyStore::SetValue(*child, ViewPropertyIdMainAxisFlexibility, flexibility);
        SendInvalidateFromChild(*child);
    }
}

///
/// @brief Initialize.
///
auto FlexLayout::Initialize() -> void
{
    EventReceiver::Connect(*this, *this, &FlexLayout::ReceiveEvent);
}

///
/// @brief Measure.
///
auto FlexLayout::Measure(MeasureScope& scope) -> void
{
    auto const mainConstraints = GetMainAxisConstraints(scope);
    auto const crossConstraints = GetCrossAxisConstraints(scope);

    auto totalMain = Dp(0);
    auto maxCross = Dp(0);
    ForEachVisibleChild([&](View& view) {
        auto const childMain = AxisConstraints::MakeUnconstrained();
        auto const childCross = AxisConstraints::MakeMinMax(0, crossConstraints.GetMax());
        auto const childSize = MeasureChild(scope, view, childMain, childCross);
        auto const childMargin = GetChildMargin(view);
        totalMain += GetMainAxisLength(childSize) + GetMainAxisLength(childMargin);
        maxCross = Dp::Max(maxCross, GetCrossAxisLength(childSize) + GetCrossAxisLength(childMargin));
    });

    auto totalGrowFactor = FlexFactor(0);
    auto totalShrinkFactor = FlexFactor(0);
    ForEachVisibleChild([&](View& view) {
        totalGrowFactor += GetChildGrowFactor(view);
        totalShrinkFactor += GetChildShrinkFactor(view);
    });

    auto const mainSpaceTotal = MeasureMainAxisSize(mainConstraints, totalMain);
    auto const mainSpace = mainSpaceTotal - totalMain;

    totalMain = 0;
    maxCross = 0;
    ForEachVisibleChild([&](View& view) {
        auto const growFactor = GetChildGrowFactor(view);
        auto const shrinkFactor = GetChildShrinkFactor(view);

        auto const childSize = scope.GetMeasuredSize(view);
        auto const childMain = GetMainAxisLength(childSize);
        auto const childMargin = GetChildMargin(view);

        if (totalGrowFactor > 0 || totalShrinkFactor > 0)
        {
            auto diff = Dp(0);
            if (mainSpace > 0)
            {
                if (totalGrowFactor > 0 && growFactor > 0)
                {
                    diff = ViewLayoutFunction::AlignToPixelGridByFloor(mainSpace * Dp(growFactor / totalGrowFactor), *this);
                }
            }
            else if (mainSpace < 0)
            {
                if (totalShrinkFactor > 0 && shrinkFactor > 0)
                {
                    diff = ViewLayoutFunction::AlignToPixelGridByFloor(mainSpace * Dp(shrinkFactor / totalShrinkFactor), *this);
                }
            }

            auto const flexibility = GetChildMainAxisFlexibility(view);
            auto const max = childMain + diff;
            auto const min = (flexibility == FlexLayoutChildMainAxisFlexibility::Expand) ? max : childMain;
            auto const newChildMain = AxisConstraints::MakeMinMax(min, max);
            auto const newChildCross = AxisConstraints::MakeMinMax(0, crossConstraints.GetMax());
            MeasureChild(scope, view, newChildMain, newChildCross);
        }
        auto const newChildSize = scope.GetMeasuredSize(view);
        totalMain += GetMainAxisLength(newChildSize) + GetMainAxisLength(childMargin);
        maxCross = Dp::Max(maxCross, GetCrossAxisLength(newChildSize) + GetCrossAxisLength(childMargin));
    });

    auto const measuredMain = MeasureMainAxisSize(mainConstraints, totalMain);
    auto const measuredCross = MeasureCrossAxisSize(crossConstraints, maxCross);

    if (GetCrossAxisAlignment() == FlexLayoutCrossAxisAlignment::Stretch)
    {
        ForEachVisibleChild([&](View& view) {
            auto const childSize = scope.GetMeasuredSize(view);
            auto const childMain = GetMainAxisLength(childSize);
            auto const childMargin = GetChildMargin(view);
            auto const newChildMain = AxisConstraints::MakeExact(childMain);
            auto const newChildCross = AxisConstraints::MakeExact(measuredCross - GetCrossAxisLength(childMargin));
            MeasureChild(scope, view, newChildMain, newChildCross);
        });
    }

    SetMeasuredSize(scope, measuredMain, measuredCross);
}

///
/// @brief Arrange.
///
auto FlexLayout::Arrange(ArrangeScope& scope) -> void
{
    auto const& parameter = scope.GetParameter();

    auto const mainSize = GetMainAxisLength(parameter.GetNewFrameRect().GetSize());
    auto const crossSize = GetCrossAxisLength(parameter.GetNewFrameRect().GetSize());

    auto mainTotal = Dp(0);
    ForEachVisibleChild([&](View& view) { mainTotal += GetMainAxisLength(scope.GetMeasuredSize(view)) + GetMainAxisLength(GetChildMargin(view)); });

    auto mainOffset = Dp(0);
    ForEachVisibleChild([&](View& view) {
        auto const childSize = scope.GetMeasuredSize(view);
        auto const childMain = GetMainAxisLength(childSize);
        auto const childCross = GetCrossAxisLength(childSize);
        auto const childMargin = GetChildMargin(view);
        auto const main = ArrangeMainAxisOffset(mainOffset, childMain + GetMainAxisLength(childMargin), mainSize, mainTotal);
        auto const cross = ArrangeCrossAxisOffset(childCross + GetCrossAxisLength(childMargin), crossSize);
        ArrangeChild(scope, view, main + GetMainAxisOffset(childMargin), cross + GetCrossAxisOffset(childMargin));
    });
}

///
/// @brief Event handler.
///
auto FlexLayout::ReceiveEvent(Event& event) -> Async<Bool>
{
    if (event.Is<InvalidateFromChild>())
    {
        InvalidateLayout();
        co_return true;
    }
    co_return false;
}

///
/// @brief Get main axis constraints.
///
auto FlexLayout::GetMainAxisConstraints(MeasureScope const& scope) const -> AxisConstraints
{
    auto const& parameter = scope.GetParameter();
    auto const flexDirection = GetDirection();
    if (flexDirection == FlexLayoutDirection::Column || flexDirection == FlexLayoutDirection::ColumnReverse)
    {
        return parameter.GetHeightConstraints();
    }
    else
    {
        return parameter.GetWidthConstraints();
    }
}

///
/// @brief Get cross axis constraints.
///
auto FlexLayout::GetCrossAxisConstraints(MeasureScope const& scope) const -> AxisConstraints
{
    auto const& parameter = scope.GetParameter();
    auto const flexDirection = GetDirection();
    if (flexDirection == FlexLayoutDirection::Column || flexDirection == FlexLayoutDirection::ColumnReverse)
    {
        return parameter.GetWidthConstraints();
    }
    else
    {
        return parameter.GetHeightConstraints();
    }
}

///
/// @brief Measure child view.
///
/// @param[in] main Main axis offset of child.
/// @param[in] cross Cross axis offset of child.
///
auto FlexLayout::MeasureChild(MeasureScope& scope, View& child, AxisConstraints const& main, AxisConstraints const& cross) const -> Size<Dp>
{
    auto const flexDirection = GetDirection();
    if (flexDirection == FlexLayoutDirection::Column || flexDirection == FlexLayoutDirection::ColumnReverse)
    {
        return scope.MeasureChild(child, cross, main);
    }
    else
    {
        return scope.MeasureChild(child, main, cross);
    }
}

///
/// @brief Arrange child.
///
/// @param[in] main Main axis offset of child.
/// @param[in] cross Cross axis offset of child.
///
auto FlexLayout::ArrangeChild(ArrangeScope& scope, View& child, Dp const main, Dp const cross) -> void
{
    auto const flexDirection = GetDirection();
    auto const layoutDirection = GetLayoutDirection();

    auto const isRTL = layoutDirection == LayoutDirection::RightToLeft;

    auto const size = scope.GetMeasuredSize(child);
    auto const mainSize = GetMainAxisLength(size);
    auto const crossSize = GetCrossAxisLength(size);

    auto const right = scope.GetParameter().GetNewFrameRect().GetWidth();
    auto const bottom = scope.GetParameter().GetNewFrameRect().GetHeight();

    if (flexDirection == FlexLayoutDirection::Column || flexDirection == FlexLayoutDirection::ColumnReverse)
    {
        auto const x = isRTL ? right - cross - crossSize : cross;
        if (flexDirection == FlexLayoutDirection::ColumnReverse)
        {
            scope.ArrangeChild(child, {x, bottom - main - mainSize});
        }
        else
        {
            scope.ArrangeChild(child, {x, main});
        }
    }
    else
    {
        if ((flexDirection == FlexLayoutDirection::Row && isRTL) || (flexDirection == FlexLayoutDirection::RowReverse && !isRTL))
        {
            scope.ArrangeChild(child, {right - main - mainSize, cross});
        }
        else
        {
            scope.ArrangeChild(child, {main, cross});
        }
    }
}

///
/// @brief Get main axis length.
///
auto FlexLayout::GetMainAxisLength(Size<Dp> const& size) const -> Dp
{
    auto const flexDirection = GetDirection();
    if (flexDirection == FlexLayoutDirection::Column || flexDirection == FlexLayoutDirection::ColumnReverse)
    {
        return size.GetHeight();
    }
    else
    {
        return size.GetWidth();
    }
}

///
/// @brief Get cross axis length.
///
auto FlexLayout::GetCrossAxisLength(Size<Dp> const& size) const -> Dp
{
    auto const flexDirection = GetDirection();
    if (flexDirection == FlexLayoutDirection::Column || flexDirection == FlexLayoutDirection::ColumnReverse)
    {
        return size.GetWidth();
    }
    else
    {
        return size.GetHeight();
    }
}

///
/// @brief Get main axis length.
///
auto FlexLayout::GetMainAxisLength(EdgeInsets const& margin) const -> Dp
{
    auto const flexDirection = GetDirection();
    if (flexDirection == FlexLayoutDirection::Column || flexDirection == FlexLayoutDirection::ColumnReverse)
    {
        return margin.GetTop() + margin.GetBottom();
    }
    else
    {
        return margin.GetLeading() + margin.GetTrailing();
    }
}

///
/// @brief Get cross axis length.
///
auto FlexLayout::GetCrossAxisLength(EdgeInsets const& margin) const -> Dp
{
    auto const flexDirection = GetDirection();
    if (flexDirection == FlexLayoutDirection::Column || flexDirection == FlexLayoutDirection::ColumnReverse)
    {
        return ViewLayoutFunction::AlignToPixelGridByRound(margin.GetLeading(), *this) + ViewLayoutFunction::AlignToPixelGridByRound(margin.GetTrailing(), *this);
    }
    else
    {
        return ViewLayoutFunction::AlignToPixelGridByRound(margin.GetTop(), *this) + ViewLayoutFunction::AlignToPixelGridByRound(margin.GetBottom(), *this);
    }
}

///
/// @brief Get main axis offset.
///
auto FlexLayout::GetMainAxisOffset(EdgeInsets const& margin) const -> Dp
{
    auto const flexDirection = GetDirection();
    if (flexDirection == FlexLayoutDirection::Column || flexDirection == FlexLayoutDirection::ColumnReverse)
    {
        return ViewLayoutFunction::AlignToPixelGridByRound(margin.GetTop(), *this);
    }
    else
    {
        return ViewLayoutFunction::AlignToPixelGridByRound(margin.GetLeading(), *this);
    }
}

///
/// @brief Get cross axis offset.
///
auto FlexLayout::GetCrossAxisOffset(EdgeInsets const& margin) const -> Dp
{
    auto const flexDirection = GetDirection();
    if (flexDirection == FlexLayoutDirection::Column || flexDirection == FlexLayoutDirection::ColumnReverse)
    {
        return ViewLayoutFunction::AlignToPixelGridByRound(margin.GetLeading(), *this);
    }
    else
    {
        return ViewLayoutFunction::AlignToPixelGridByRound(margin.GetTop(), *this);
    }
}

///
/// @brief Measure main axis size.
///
/// @param[in] totalMain Total main axis length of children.
///
auto FlexLayout::MeasureMainAxisSize(AxisConstraints const& main, Dp const totalMain) const -> Dp
{
    if (GetMainAxisSize() == FlexLayoutMainAxisSize::Min)
    {
        return AxisConstraints::Constrain(main, totalMain);
    }
    else
    {
        if (main.IsBounded())
        {
            return main.GetMax();
        }
        return AxisConstraints::Constrain(main, totalMain);
    }
}

///
/// @brief Measure cross axis size.
///
/// @param[in] maxCross Maximum cross axis length of children.
///
auto FlexLayout::MeasureCrossAxisSize(AxisConstraints const& cross, Dp const maxCross) const -> Dp
{
    if (GetCrossAxisSize() == FlexLayoutCrossAxisSize::Min)
    {
        return AxisConstraints::Constrain(cross, maxCross);
    }
    else
    {
        if (cross.IsBounded())
        {
            return cross.GetMax();
        }
        return AxisConstraints::Constrain(cross, maxCross);
    }
}

///
/// @brief Compute main axis offset of child view.
///
/// @param[in, out] offset Current main axis offset.
/// @param[in] main Main axis size of child view.
/// @param[in] size Main axis size of this view.
/// @param[in] total Main axis total length of children.
///
/// @return Main axis offset of child view.
///
auto FlexLayout::ArrangeMainAxisOffset(Dp& offset, Dp const main, Dp const size, Dp const total) const -> Dp
{
    auto const alignment = GetMainAxisAlignment();
    if (alignment == FlexLayoutMainAxisAlignment::Start)
    {
        auto const currentOffset = offset;
        offset += main;
        return currentOffset;
    }
    else if (alignment == FlexLayoutMainAxisAlignment::End)
    {
        offset += main;
        return size - offset;
    }
    else if (alignment == FlexLayoutMainAxisAlignment::Center)
    {
        auto const currentOffset = offset;
        offset += main;
        return (size - total) / 2 + currentOffset;
    }
    else if (alignment == FlexLayoutMainAxisAlignment::SpaceBetween)
    {
        auto const space = (size - total) / Dp(GetAddedChildCount());
        auto const currentOffset = offset;
        offset += main + space;
        return currentOffset;
    }
    else if (alignment == FlexLayoutMainAxisAlignment::SpaceAround)
    {
        auto const space = (size - total) / Dp(GetAddedChildCount() + 1);
        auto const currentOffset = offset;
        offset += main + space;
        return currentOffset + space / 2;
    }
    FW_DEBUG_ASSERT(false);
    return 0;
}

///
/// @brief Compute cross axis offset of child view.
///
/// @param[in] cross Cross axis size of child view.
/// @param[in] total Cross axis size of this view.
///
/// @return Cross axis offset of child view.
///
auto FlexLayout::ArrangeCrossAxisOffset(Dp const cross, Dp const size) const -> Dp
{
    auto const alignment = GetCrossAxisAlignment();
    if (alignment == FlexLayoutCrossAxisAlignment::Start || alignment == FlexLayoutCrossAxisAlignment::Stretch)
    {
        return 0;
    }
    else if (alignment == FlexLayoutCrossAxisAlignment::End)
    {
        return size - cross;
    }
    else if (alignment == FlexLayoutCrossAxisAlignment::Center)
    {
        return (size - cross) / 2;
    }
    return 0;
}

///
/// @brief Set measured size of this view.
///
/// @param[in] main Main axis length of this view.
/// @param[in] cross Cross axis length of this view.
///
auto FlexLayout::SetMeasuredSize(MeasureScope& scope, Dp const main, Dp const cross) -> void
{
    auto const direction = GetDirection();
    if (direction == FlexLayoutDirection::Column || direction == FlexLayoutDirection::ColumnReverse)
    {
        scope.SetMeasuredSize(cross, main);
    }
    else
    {
        scope.SetMeasuredSize(main, cross);
    }
}
}

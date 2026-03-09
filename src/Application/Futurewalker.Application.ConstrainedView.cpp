// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ConstrainedView.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.ArrangeScope.hpp"

namespace FW_DETAIL_NS
{
auto ConstrainedView::MakeView() -> Shared<ConstrainedView>
{
    return View::MakeDerived<ConstrainedView>();
}

ConstrainedView::ConstrainedView(PassKey<View> key)
  : View(key)
{
}

auto ConstrainedView::GetContent() -> Shared<View>
{
    return GetChildAt(0);
}

auto ConstrainedView::GetContent() const -> Shared<View const>
{
    return GetChildAt(0);
}

auto ConstrainedView::SetContent(Shared<View> const& content) -> void
{
    auto const child = GetContent();
    if (child != content)
    {
        if (child)
        {
            child->RemoveFromParent();
        }
        AddChildBack(content);
    }
}

auto ConstrainedView::GetWidthConstraints() const -> AxisConstraints
{
    return _widthConstraints.GetValueOrDefault();
}

auto ConstrainedView::SetWidthConstraints(AttributeArg<AxisConstraints> const& constraints) -> void
{
    _widthConstraints.SetAttributeArg(constraints);
}

auto ConstrainedView::GetHeightConstraints() const -> AxisConstraints
{
    return _heightConstraints.GetValueOrDefault();
}

auto ConstrainedView::SetHeightConstraints(AttributeArg<AxisConstraints> const& constraints) -> void
{
    _heightConstraints.SetAttributeArg(constraints);
}

auto ConstrainedView::Initialize() -> void
{
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(AxisConstraints, AttributeWidthConstraints, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(AxisConstraints, AttributeHeightConstraints, {});

    _widthConstraints.BindAndConnectAttributeWithDefaultValue(*this, &ConstrainedView::ReceiveAttributeEvent, AttributeWidthConstraints, AxisConstraints::MakeUnconstrained());
    _heightConstraints.BindAndConnectAttributeWithDefaultValue(*this, &ConstrainedView::ReceiveAttributeEvent, AttributeHeightConstraints, AxisConstraints::MakeUnconstrained());
}

auto ConstrainedView::Measure(MeasureScope& scope) -> void
{
    auto const& parameter = scope.GetParameter();

    auto const childWidth = IntersectConstraints(parameter.GetWidthConstraints(), _widthConstraints.GetValueOrDefault());
    auto const childHeight = IntersectConstraints(parameter.GetWidthConstraints(), _widthConstraints.GetValueOrDefault());
    if (auto const content = GetContent())
    {
        auto const size = scope.MeasureChild(content, childWidth, childHeight);
        scope.SetMeasuredSize(size);
    }
    else
    {
        auto const measuredWidth = childWidth.GetMin();
        auto const measuredHeight = childHeight.GetMin();
        scope.SetMeasuredSize(measuredWidth, measuredHeight);
    }
}

auto ConstrainedView::ReceiveAttributeEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
        InvalidateLayout();
        co_return true;
    }
    co_return false;
}

auto ConstrainedView::IntersectConstraints(AxisConstraints const& source, AxisConstraints const& mask) -> AxisConstraints
{
    if (mask.GetMax() < source.GetMin())
    {
        return AxisConstraints::MakeExact(source.GetMin());
    }

    if (source.IsBounded() && (source.GetMax() < mask.GetMin()))
    {
        return AxisConstraints::MakeExact(source.GetMax());
    }

    auto const min = Dp::Max(source.GetMin(), mask.GetMin());
    auto const max = Dp::Min(source.GetMax(), mask.GetMax());
    return AxisConstraints::MakeMinMax(min, max);
}
}

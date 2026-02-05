// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ConstrainedView.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.ArrangeScope.hpp"
#include "Futurewalker.Application.ContainerView.hpp" 

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
    if (_container)
    {
        return _container->GetContent();
    }
    return {};
}

auto ConstrainedView::SetContent(Shared<View> const& content) -> void
{
    if (_container)
    {
        _container->SetContent(content);
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

    _container = ContainerView::Make();
    AddChildBack(_container);
}

auto ConstrainedView::Measure(MeasureScope& scope) -> void
{
    auto const& parameter = scope.GetParameter();

    auto IntersectConstraints = [](AxisConstraints const& source, AxisConstraints const& mask) {
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
    };

    auto maxSize = Size<Dp>();
    ForEachVisibleChild([&](View& child) {
        auto const childWidth = IntersectConstraints(parameter.GetWidthConstraints(), _widthConstraints.GetValueOrDefault());
        auto const childHeight = IntersectConstraints(parameter.GetWidthConstraints(), _widthConstraints.GetValueOrDefault());
        maxSize = Size<Dp>::Max(maxSize, scope.MeasureChild(child, childWidth, childHeight));
    });
    scope.SetMeasuredSize(maxSize);
}

auto ConstrainedView::Arrange(ArrangeScope& scope) -> void
{
    View::Arrange(scope);
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
}

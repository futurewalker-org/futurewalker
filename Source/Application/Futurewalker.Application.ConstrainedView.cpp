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
    return _widthConstraints;
}

auto ConstrainedView::SetWidthConstraints(AxisConstraints const& constraints) -> void
{
    if (_widthConstraints != constraints)
    {
        _widthConstraints = constraints;
        InvalidateLayout();
    }
}

auto ConstrainedView::GetHeightConstraints() const -> AxisConstraints
{
    return _heightConstraints;
}

auto ConstrainedView::SetHeightConstraints(AxisConstraints const& constraints) -> void
{
    if (_heightConstraints != constraints)
    {
        _heightConstraints = constraints;
        InvalidateLayout();
    }
}

auto ConstrainedView::Initialize() -> void
{
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
        auto const childWidth = IntersectConstraints(parameter.GetWidthConstraints(), _widthConstraints);
        auto const childHeight = IntersectConstraints(parameter.GetWidthConstraints(), _widthConstraints);
        maxSize = Size<Dp>::Max(maxSize, scope.MeasureChild(child, childWidth, childHeight));
    });
    scope.SetMeasuredSize(maxSize);
}

auto ConstrainedView::Arrange(ArrangeScope& scope) -> void
{
    View::Arrange(scope);
}
}

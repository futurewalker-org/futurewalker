// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PaddingView.hpp"
#include "Futurewalker.Application.ContainerView.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.ArrangeScope.hpp"

namespace FW_DETAIL_NS
{
auto PaddingView::Make() -> Shared<PaddingView>
{
    return View::MakeDerived<PaddingView>();
}

auto PaddingView::MakeWithPadding(EdgeInsets const& padding) -> Shared<PaddingView>
{
    auto view = Make();
    view->SetPadding(padding);
    return view;
}

auto PaddingView::MakeWithContent(Shared<View> const& content) -> Shared<PaddingView>
{
    auto view = Make();
    view->SetContent(content);
    return view;
}

auto PaddingView::MakeWithPaddingAndContent(EdgeInsets const& padding, Shared<View> const& content) -> Shared<PaddingView>
{
    auto view = Make();
    view->SetPadding(padding);
    view->SetContent(content);
    return view;
}

auto PaddingView::GetPadding() const -> EdgeInsets
{
    return _padding;
}

auto PaddingView::SetPadding(EdgeInsets const& padding) -> void
{
    auto const normalizedPadding = EdgeInsets::Max(padding, EdgeInsets());
    if (_padding != normalizedPadding)
    {
        _padding = normalizedPadding;
        InvalidateLayout();
    }
}

auto PaddingView::GetContent() -> Shared<View>
{
    return _container->GetContent();
}

auto PaddingView::GetContent() const -> Shared<View const>
{
    return _container->GetContent();
}

auto PaddingView::SetContent(Shared<View> const& content) -> void
{
    _container->SetContent(content);
}

PaddingView::PaddingView(PassKey<View> key)
  : View(key)
{
}

auto PaddingView::Initialize() -> void
{
    _container = ContainerView::Make();
    AddChildBack(_container);
}

auto PaddingView::Measure(MeasureScope& scope) -> void
{
    auto const& parameter = scope.GetParameter();
    auto const& widthConstraints = parameter.GetWidthConstraints();
    auto const& heightConstraints = parameter.GetHeightConstraints();

    auto const horizontal = _padding.GetLeading() + _padding.GetTrailing();
    auto const vertical = _padding.GetTop() + _padding.GetBottom();
    auto const adjustedWidth = AxisConstraints::Offset(widthConstraints, -horizontal);
    auto const adjustedHeight = AxisConstraints::Offset(heightConstraints, -vertical);
    auto const childSize = scope.MeasureChild(_container, adjustedWidth, adjustedHeight);

    auto const measuredWidth = AxisConstraints::Constrain(widthConstraints, childSize.GetWidth() + horizontal);
    auto const measuredHeight = AxisConstraints::Constrain(heightConstraints, childSize.GetHeight() + vertical);
    scope.SetMeasuredSize(measuredWidth, measuredHeight);
}

auto PaddingView::Arrange(ArrangeScope& scope) -> void
{
    auto const layoutDirection = GetLayoutDirection();
    auto const left = (layoutDirection == LayoutDirection::LeftToRight) ? _padding.GetLeading() : _padding.GetTrailing();
    auto const top = _padding.GetTop();
    scope.ArrangeChild(_container, {left, top});
}
}

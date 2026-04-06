// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PaddingView.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.ArrangeScope.hpp"

namespace FW_DETAIL_NS
{
auto PaddingView::Make() -> Shared<PaddingView>
{
    return View::MakeDerived<PaddingView>();
}

auto PaddingView::MakeWithPadding(AttributeArg<EdgeInsets> const& padding) -> Shared<PaddingView>
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

auto PaddingView::MakeWithPaddingAndContent(AttributeArg<EdgeInsets> const& padding, Shared<View> const& content) -> Shared<PaddingView>
{
    auto view = Make();
    view->SetPadding(padding);
    view->SetContent(content);
    return view;
}

auto PaddingView::GetPadding() const -> EdgeInsets
{
    return _padding.GetValueOrDefault();
}

auto PaddingView::SetPadding(AttributeArg<EdgeInsets> const& padding) -> void
{
    _padding.SetAttributeArg(padding);
}

auto PaddingView::GetContent() -> Shared<View>
{
    return GetChildAt(0);
}

auto PaddingView::GetContent() const -> Shared<View const>
{
    return GetChildAt(0);
}

auto PaddingView::SetContent(Shared<View> const& content) -> void
{
    auto child = GetChildAt(0);
    if (child != content)
    {
        if (child)
        {
            child->RemoveFromParent();
        }
        AddChildBack(content);
    }
}

PaddingView::PaddingView(PassKey<View> key)
  : View(key)
{
}

auto PaddingView::Initialize() -> void
{
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(EdgeInsets, PaddingAttribute, {});

    _padding.BindAndConnectAttributeWithDefaultValue(*this, &PaddingView::ReceiveAttributeEvent, PaddingAttribute, {});
}

auto PaddingView::Measure(MeasureScope& scope) -> void
{
    auto const& parameter = scope.GetParameter();
    auto const& widthConstraints = parameter.GetWidthConstraints();
    auto const& heightConstraints = parameter.GetHeightConstraints();

    auto const padding = GetNormalizedPadding();
    auto const horizontal = padding.GetLeading() + padding.GetTrailing();
    auto const vertical = padding.GetTop() + padding.GetBottom();
    auto const adjustedWidth = AxisConstraints::Offset(widthConstraints, -horizontal);
    auto const adjustedHeight = AxisConstraints::Offset(heightConstraints, -vertical);

    auto childSize = Size<Dp>();
    if (auto const content = GetContent())
    {
        childSize = scope.MeasureChild(content, adjustedWidth, adjustedHeight);
    }

    auto const measuredWidth = AxisConstraints::Constrain(widthConstraints, childSize.width + horizontal);
    auto const measuredHeight = AxisConstraints::Constrain(heightConstraints, childSize.height + vertical);
    scope.SetMeasuredSize(measuredWidth, measuredHeight);
}

auto PaddingView::Arrange(ArrangeScope& scope) -> void
{
    if (auto const content = GetContent())
    {
        auto const layoutDirection = GetLayoutDirection();
        auto const padding = GetNormalizedPadding();
        auto const left = (layoutDirection == LayoutDirection::LeftToRight) ? padding.GetLeading() : padding.GetTrailing();
        auto const top = padding.GetTop();
        scope.ArrangeChild(content, {left, top});
    }
}

auto PaddingView::ReceiveAttributeEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
        InvalidateLayout();
    }
    co_return false;
}

auto PaddingView::GetNormalizedPadding() const -> EdgeInsets
{
    return EdgeInsets::Max(_padding.GetValueOrDefault(), EdgeInsets());
}
}

// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.DisclosureView.hpp"

#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.ArrangeScope.hpp"
#include "Futurewalker.Application.DrawScope.hpp"

namespace FW_LAMP_DETAIL_NS
{
auto DisclosureView::Make() -> Shared<DisclosureView>
{
    return View::MakeDerived<DisclosureView>();
}

DisclosureView::DisclosureView(PassKey<View> key)
  : View(key)
{
}

auto DisclosureView::GetContent() -> Shared<View>
{
    return GetChildAt(0);
}

auto DisclosureView::GetContent() const -> Shared<View const>
{
    return GetChildAt(0);
}

auto DisclosureView::SetContent(Shared<View> content) -> void
{
    auto const child = GetChildAt(0);
    if (child != content)
    {
        if (child)
        {
            child->RemoveFromParent();
        }
        AddChildBack(content);
        UpdateContentVisibility();
    }
}

auto DisclosureView::SetOpenRatio(AttributeArg<Float64> const& openRatio) -> void
{
    _openRatio.SetAttributeArg(openRatio);
}

auto DisclosureView::Initialize() -> void
{
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Float64, OpenRatioAttribute, 0.0);

    _openRatio.BindAndConnectAttributeWithDefaultValue(*this, &DisclosureView::ReceiveAttributeEvent, OpenRatioAttribute, 0.0);
}

auto DisclosureView::Measure(MeasureScope& scope) -> void
{
    auto const child = GetChildAt(0);
    if (child && child->IsVisible())
    {
        auto const widthConstraints = scope.GetParameter().GetWidthConstraints();
        auto const heightConstraints = scope.GetParameter().GetHeightConstraints();
        auto const childSize = scope.MeasureChild(child, widthConstraints, AxisConstraints::MakeUnconstrained());
        auto const openRatio = _openRatio.GetValueOr(0);
        auto const height = AxisConstraints::Constrain(heightConstraints, childSize.height * static_cast<Dp>(openRatio));
        _measuredOpenRatio = openRatio;
        scope.SetMeasuredSize(childSize.width, height);
    }
    else
    {
        View::Measure(scope);
    }
}

auto DisclosureView::Arrange(ArrangeScope& scope) -> void
{
    auto const child = GetChildAt(0);
    if (child && child->IsVisible())
    {
        auto const childSize = scope.GetMeasuredSize(child);
        auto const y = childSize.height * static_cast<Dp>(1.0 - _measuredOpenRatio);
        scope.ArrangeChild(child, Point<Dp>(0, -y));
    }
    else
    {
        View::Arrange(scope);
    }
}

auto DisclosureView::Draw(DrawScope& scope) -> void
{
    scope.SetClipMode(ViewClipMode::Bounds);
}

auto DisclosureView::ReceiveAttributeEvent(Event<>& event) -> Bool
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
        UpdateContentVisibility();
        InvalidateLayout();
        return true;
    }
    return false;
}

auto DisclosureView::UpdateContentVisibility() -> void
{
    auto const openRatio = _openRatio.GetValueOr(0);
    auto const visibility = openRatio > 0.0;
    auto const child = GetChildAt(0);
    if (child && child->IsVisible() != visibility)
    {
        child->SetVisible(visibility);
    }
}
}

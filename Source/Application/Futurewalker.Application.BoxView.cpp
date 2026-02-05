// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.BoxView.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.DrawScope.hpp"
#include "Futurewalker.Application.ViewDrawFunction.hpp"

namespace FW_DETAIL_NS
{
auto BoxView::Make() -> Shared<BoxView>
{
    return View::MakeDerived<BoxView>();
}

BoxView::BoxView(PassKey<View> key)
  : View(key)
{
}

auto BoxView::SetBackgroundColor(AttributeArg<RGBAColor> color) -> void
{
    _backgroundColor.SetAttributeArg(color);
}

auto BoxView::SetBackgroundAlpha(AttributeArg<Channel> alpha) -> void
{
    _backgroundAlpha.SetAttributeArg(alpha);
}

auto BoxView::SetBorderColor(AttributeArg<RGBAColor> color) -> void
{
    _borderColor.SetAttributeArg(color);
}

auto BoxView::SetBorderAlpha(AttributeArg<Channel> alpha) -> void
{
    _borderAlpha.SetAttributeArg(alpha);
}

auto BoxView::SetBorderWidth(AttributeArg<Dp> width) -> void
{
    _borderWidth.SetAttributeArg(width);
}

auto BoxView::SetCornerRadius(AttributeArg<CornerRadius> radius) -> void
{
    _cornerRadius.SetAttributeArg(radius);
}

auto BoxView::Initialize() -> void
{
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeBackgroundColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeBackgroundAlpha, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeBorderColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeBorderAlpha, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Dp, AttributeBorderWidth, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(CornerRadius, AttributeCornerRadius, {});

    _backgroundColor.BindAndConnectAttributeWithDefaultValue(*this, &BoxView::ReceiveAttributeEvent, AttributeBackgroundColor, {});
    _backgroundAlpha.BindAndConnectAttributeWithDefaultValue(*this, &BoxView::ReceiveAttributeEvent, AttributeBackgroundAlpha, {1});
    _borderColor.BindAndConnectAttributeWithDefaultValue(*this, &BoxView::ReceiveAttributeEvent, AttributeBorderColor, {});
    _borderAlpha.BindAndConnectAttributeWithDefaultValue(*this, &BoxView::ReceiveAttributeEvent, AttributeBorderAlpha, {1});
    _borderWidth.BindAndConnectAttributeWithDefaultValue(*this, &BoxView::ReceiveAttributeEvent, AttributeBorderWidth, {0});
    _cornerRadius.BindAndConnectAttributeWithDefaultValue(*this, &BoxView::ReceiveAttributeEvent, AttributeCornerRadius, {});
}

auto BoxView::Measure(MeasureScope& scope) -> void
{
    auto const width = scope.GetParameter().GetWidthConstraints();
    auto const height = scope.GetParameter().GetHeightConstraints();

    auto const measureAxis = [&](auto const& constraints) -> Dp {
        if (constraints.IsBounded())
        {
            return constraints.GetMax();
        }
        else
        {
            return Dp(0);
        }
    };

    auto const measuredWidth = measureAxis(width);
    auto const measuredHeight = measureAxis(height);
    scope.SetMeasuredSize(measuredWidth, measuredHeight);
}

auto BoxView::Draw(DrawScope& scope) -> void
{
    auto& scene = scope.GetScene();

    auto const backgroundColor = _backgroundColor.GetValueOrDefault();
    auto const backgroundAlpha = _backgroundAlpha.GetValueOrDefault();
    auto const borderColor = _borderColor.GetValueOrDefault();
    auto const borderAlpha = _borderAlpha.GetValueOrDefault();
    auto const borderWidth = _borderWidth.GetValueOr(0);
    auto const cornerRadius = _cornerRadius.GetValueOrDefault();

    auto const contentRect = GetContentRect();
    auto const direction = GetLayoutDirection();

    ViewDrawFunction::DrawRoundRect(scene, contentRect, cornerRadius, backgroundColor.WithAlphaMultiplied(backgroundAlpha), direction);
    ViewDrawFunction::DrawRoundRectBorder(scene, contentRect, cornerRadius, borderColor.WithAlphaMultiplied(borderAlpha), borderWidth, direction);
}

auto BoxView::ReceiveAttributeEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<AttributeEvent>())
    {
        InvalidateVisual();
    }
    co_return false;
}
}

// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.AlignView.hpp"
#include "Futurewalker.Application.ContainerView.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.ArrangeScope.hpp"
#include "Futurewalker.Application.ViewLayoutFunction.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
auto AlignView::Make() -> Shared<AlignView>
{
    return View::MakeDerived<AlignView>();
}

///
/// @brief
///
/// @param content
///
auto AlignView::MakeWithContent(Shared<View> const& content) -> Shared<AlignView>
{
    auto view = Make();
    view->SetContent(content);
    return view;
}

///
/// @brief
///
auto AlignView::GetContent() const -> Shared<View>
{
    if (_container)
    {
        return _container->GetContent();
    }
    return {};
}

///
/// @brief
///
/// @param content
///
auto AlignView::SetContent(Shared<View> const& content) -> void
{
    if (_container)
    {
        _container->SetContent(content);
    }
}

///
/// @brief
///
auto AlignView::GetAlignment() const -> Alignment
{
    return _alignment;
}

///
/// @brief
///
/// @param alignment
///
auto AlignView::SetAlignment(Alignment const& alignment) -> void
{
    if (_alignment != alignment)
    {
        _alignment = alignment;
        InvalidateLayout();
    }
}

///
/// @brief
///
AlignView::AlignView(PassKey<View> key)
  : View(key)
{
}

///
/// @brief
///
auto AlignView::Initialize() -> void
{
    _container = ContainerView::Make();
    AddChildBack(_container);
}

///
/// @brief
///
/// @param scope
///
auto AlignView::Measure(MeasureScope& scope) -> void
{
    const auto& parameter = scope.GetParameter();
    const auto& width = parameter.GetWidthConstraints();
    const auto& height = parameter.GetHeightConstraints();

    Size<Dp> maxSize;
    ForEachVisibleChild([&](View& view) {
        scope.MeasureChild(view, AxisConstraints::MakeMinMax(0, width.GetMax()), AxisConstraints::MakeMinMax(0, height.GetMax()));
        maxSize = Size<Dp>::Max(maxSize, scope.GetMeasuredSize(view));
    });

    auto measureAxis = [](const auto& c, const auto& v) {
        if (c.IsBounded())
        {
            return c.GetMax();
        }
        return AxisConstraints::Constrain(c, v);
    };

    const auto measuredWidth = measureAxis(width, maxSize.GetWidth());
    const auto measuredHeight = measureAxis(height, maxSize.GetHeight());
    scope.SetMeasuredSize(measuredWidth, measuredHeight);
}

///
/// @brief
///
/// @param scope
///
auto AlignView::Arrange(ArrangeScope& scope) -> void
{
    ForEachVisibleChild([&](View& view) {
        const auto size = scope.GetMeasuredSize(view);
        const auto alignment = GetAlignment();
        const auto contentRect = GetContentRect();

        const auto reverse = GetLayoutDirection() == LayoutDirection::RightToLeft ? -1.0 : 1.0;
        const auto xRatio = (alignment.GetX() * reverse + 1) / 2;
        const auto yRatio = (alignment.GetY() + 1) / 2;

        const auto x = contentRect.GetLeft() + (contentRect.GetWidth() - size.GetWidth()) * Dp(xRatio);
        const auto y = contentRect.GetTop() + (contentRect.GetHeight() - size.GetHeight()) * Dp(yRatio);
        const auto pos = ViewLayoutFunction::AlignToPixelGridByRound(Point<Dp>(x, y), *this);
        scope.ArrangeChild(view, pos);
    });
}
}

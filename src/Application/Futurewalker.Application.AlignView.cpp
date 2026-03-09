// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.AlignView.hpp"
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
auto AlignView::GetContent() -> Shared<View>
{
    return GetChildAt(0);
}

///
/// @brief
///
auto AlignView::GetContent() const -> Shared<View const>
{
    return GetChildAt(0);
}

///
/// @brief
///
/// @param content
///
auto AlignView::SetContent(Shared<View> const& content) -> void
{
    auto const child = GetChildAt(0);
    if (child != content)
    {
        if (child)
        {
            child->RemoveFromParent();
        }
        AddChildBack(content);
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
/// @param scope
///
auto AlignView::Measure(MeasureScope& scope) -> void
{
    auto const& parameter = scope.GetParameter();
    auto const& width = parameter.GetWidthConstraints();
    auto const& height = parameter.GetHeightConstraints();

    auto maxSize = Size<Dp>();
    ForEachVisibleChild([&](View& view) {
        scope.MeasureChild(view, AxisConstraints::MakeMinMax(0, width.GetMax()), AxisConstraints::MakeMinMax(0, height.GetMax()));
        maxSize = Size<Dp>::Max(maxSize, scope.GetMeasuredSize(view));
    });

    auto measureAxis = [](auto const& c, auto const& v) {
        if (c.IsBounded())
        {
            return c.GetMax();
        }
        return AxisConstraints::Constrain(c, v);
    };

    auto const measuredWidth = measureAxis(width, maxSize.GetWidth());
    auto const measuredHeight = measureAxis(height, maxSize.GetHeight());
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
        auto const size = scope.GetMeasuredSize(view);
        auto const alignment = GetAlignment();
        auto const contentRect = GetContentRect();

        auto const reverse = GetLayoutDirection() == LayoutDirection::RightToLeft ? -1.0 : 1.0;
        auto const xRatio = (alignment.GetX() * reverse + 1) / 2;
        auto const yRatio = (alignment.GetY() + 1) / 2;

        auto const x = contentRect.GetLeft() + (contentRect.GetWidth() - size.GetWidth()) * Dp(xRatio);
        auto const y = contentRect.GetTop() + (contentRect.GetHeight() - size.GetHeight()) * Dp(yRatio);
        auto const pos = ViewLayoutFunction::AlignToPixelGridByRound(Point<Dp>(x, y), *this);
        scope.ArrangeChild(view, pos);
    });
}
}

// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.SizedView.hpp"
#include "Futurewalker.Application.ContainerView.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.ArrangeScope.hpp"
#include "Futurewalker.Application.ViewLayoutFunction.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Make instance.
///
auto SizedView::Make() -> Shared<SizedView>
{
    return View::MakeDerived<SizedView>();
}

///
/// @brief Make with width and height.
///
auto SizedView::Make(Dp const width, Dp const height) -> Shared<SizedView>
{
    auto view = Make();
    view->SetWidth(width);
    view->SetHeight(height);
    return view;
}

///
/// @brief Make with content view.
///
auto SizedView::MakeWithContent(Shared<View> const& content) -> Shared<SizedView>
{
    auto view = Make();
    view->SetContent(content);
    return view;
}

///
/// @brief Make with width/height and content view.
///
auto SizedView::MakeWithContent(Dp const width, Dp const height, Shared<View> const& content) -> Shared<SizedView>
{
    auto view = Make(width, height);
    view->SetContent(content);
    return view;
}

///
/// @brief Set width.
///
auto SizedView::SetWidth(Dp const width) -> void
{
    if (Dp::IsNaN(width))
    {
        return;
    }

    const auto actual = Dp::Max(0, width);
    if (_width != actual)
    {
        _width = actual;
        InvalidateLayout();
    }
}

///
/// @brief Get width.
///
auto SizedView::GetWidth() const -> Dp
{
    return _width;
}

///
/// @brief Set height.
///
auto SizedView::SetHeight(Dp const height) -> void
{
    if (Dp::IsNaN(height))
    {
        return;
    }

    const auto actual = Dp::Max(0, height);
    if (_height != actual)
    {
        _height = actual;
        InvalidateLayout();
    }
}

///
/// @brief Get height.
///
auto SizedView::GetHeight() const -> Dp
{
    return _height;
}

///
/// @brief Set content view.
///
auto SizedView::SetContent(Shared<View> const& content) -> void
{
    if (_container)
    {
        _container->SetContent(content);
    }
}

///
/// @brief Get content.
///
auto SizedView::GetContent() -> Shared<View>
{
    if (_container)
    {
        _container->GetContent();
    }
    return {};
}

///
/// @brief
///
SizedView::SizedView(PassKey<View> key)
  : View(key)
{
}

///
/// @brief Initialize.
///
auto SizedView::Initialize() -> void
{
    _container = ContainerView::Make();
    AddChildBack(_container);
}

///
/// @brief Measure.
///
auto SizedView::Measure(MeasureScope& scope) -> void
{
    const auto& parameter = scope.GetParameter();
    const auto& widthConstr = parameter.GetWidthConstraints();
    const auto& heightConstr = parameter.GetHeightConstraints();

    const auto measuredWidth = MeasureAxis(widthConstr, _width);
    const auto measuredHeight = MeasureAxis(heightConstr, _height);
    scope.SetMeasuredSize(measuredWidth, measuredHeight);

    ForEachVisibleChild([&](View& view) { scope.MeasureChild(view, AxisConstraints::MakeExact(measuredWidth), AxisConstraints::MakeExact(measuredHeight)); });
}

///
/// @brief Measure axis.
///
auto SizedView::MeasureAxis(const AxisConstraints& c, Dp const v) const -> Dp
{
    if (Dp::IsFinite(v))
    {
        return AxisConstraints::Constrain(c, ViewLayoutFunction::AlignToPixelGridByRound(v, *this));
    }
    return c.IsBounded() ? c.GetMax() : c.GetMin();
}
}

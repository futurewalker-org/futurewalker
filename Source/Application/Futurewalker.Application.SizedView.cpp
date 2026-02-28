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
auto SizedView::Make(AttributeArg<Dp> const& width, AttributeArg<Dp> const& height) -> Shared<SizedView>
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
auto SizedView::MakeWithContent(AttributeArg<Dp> const& width, AttributeArg<Dp> const& height, Shared<View> const& content) -> Shared<SizedView>
{
    auto view = Make(width, height);
    view->SetContent(content);
    return view;
}

///
/// @brief Set width.
///
auto SizedView::SetWidth(AttributeArg<Dp> const& width) -> void
{
    _width.SetAttributeArg(width);
}

///
/// @brief Get width.
///
auto SizedView::GetWidth() const -> Dp
{
    return _width.GetValueOr(0);
}

///
/// @brief Set height.
///
auto SizedView::SetHeight(AttributeArg<Dp> const& height) -> void
{
    _height.SetAttributeArg(height);
}

///
/// @brief Get height.
///
auto SizedView::GetHeight() const -> Dp
{
    return _height.GetValueOr(0);
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

    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Dp, AttributeWidth, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Dp, AttributeHeight, {0});

    _width.BindAndConnectAttributeWithDefaultValue(*this, &SizedView::ReceiveAttributeEvent, AttributeWidth, -1);
    _height.BindAndConnectAttributeWithDefaultValue(*this, &SizedView::ReceiveAttributeEvent, AttributeHeight, -1);
}

///
/// @brief Measure.
///
auto SizedView::Measure(MeasureScope& scope) -> void
{
    auto const& parameter = scope.GetParameter();
    auto const& widthConstr = parameter.GetWidthConstraints();
    auto const& heightConstr = parameter.GetHeightConstraints();

    auto const measuringWidth = MeasureAxis(widthConstr, GetLayoutWidth());
    auto const measuringHeight = MeasureAxis(heightConstr, GetLayoutHeight());

    auto maxSize = Size<Dp>();
    ForEachVisibleChild([&](View& view) {
        auto const measuringWidthConstr = measuringWidth ? AxisConstraints::MakeExact(*measuringWidth) : widthConstr;
        auto const measuringHeightConstr = measuringHeight ? AxisConstraints::MakeExact(*measuringHeight) : heightConstr;
        auto const size = scope.MeasureChild(view, measuringWidthConstr, measuringHeightConstr);
        maxSize = Size<Dp>::Max(maxSize, size);
    });
    scope.SetMeasuredSize(maxSize);
}

auto SizedView::ReceiveAttributeEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
        InvalidateLayout();
    }
    co_return false;
}

///
/// @brief Measure axis.
///
auto SizedView::MeasureAxis(const AxisConstraints& c, Optional<Dp> const& v) const -> Optional<Dp>
{
    if (v)
    {
        if (Dp::IsFinite(*v))
        {
            return AxisConstraints::Constrain(c, ViewLayoutFunction::AlignToPixelGridByRound(*v, *this));
        }
        return c.IsBounded() ? c.GetMax() : c.GetMin();
    }
    return {};
}

///
/// @brief 
///
auto SizedView::GetLayoutWidth() const -> Optional<Dp>
{
    if (auto width = _width.GetValue())
    {
        if (!Dp::IsNaN(*width) && 0 <= *width)
        {
            return *width;
        }
    }
    return {};
}

///
/// @brief 
///
auto SizedView::GetLayoutHeight() const -> Optional<Dp>
{
    if (auto height = _height.GetValue())
    {
        if (!Dp::IsNaN(*height) && 0 <= *height)
        {
            return *height;
        }
    }
    return {};
}
}

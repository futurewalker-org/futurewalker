// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ContainerView.hpp"
#include "Futurewalker.Application.ViewEvent.hpp"
#include "Futurewalker.Application.ViewDrawFunction.hpp"
#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.ArrangeScope.hpp"
#include "Futurewalker.Application.DrawScope.hpp"
#include "Futurewalker.Application.ClipView.hpp"
#include "Futurewalker.Application.BoxView.hpp"

#include "Futurewalker.Graphics.Scene.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Make ContainerView instance.
///
auto ContainerView::Make() -> Shared<ContainerView>
{
    return View::MakeDerived<ContainerView>();
}

///
/// @brief Make ContainerView instance with specified content.
///
/// @param[in] content Content of new instance.
///
auto ContainerView::MakeWithContent(Shared<View> const& content) -> Shared<ContainerView>
{
    auto view = Make();
    view->SetContent(content);
    return view;
}

///
/// @brief Constructor.
///
ContainerView::ContainerView(PassKey<View> key)
  : View(key)
{
}

///
/// @brief Get content view.
///
auto ContainerView::GetContent() -> Shared<View>
{
    return _clipView->GetContent();
}

///
/// @brief Get content view.
///
auto ContainerView::GetContent() const -> Shared<View const>
{
    return _clipView->GetContent();
}

///
/// @brief Set content view.
///
/// @param[in] view New content view.
///
auto ContainerView::SetContent(Shared<View> const& view) -> void
{
    _clipView->SetContent(view);
}

///
/// @brief Set background color.
///
/// @param[in] color Background color.
///
auto ContainerView::SetBackgroundColor(AttributeArg<RGBAColor> color) -> void
{
    _backgroundColor.SetAttributeArg(color);
}

///
/// @brief Set background alpha.
///
/// @param[in] alpha Background alpha.
///
auto ContainerView::SetBackgroundAlpha(AttributeArg<Channel> alpha) -> void
{
    _backgroundAlpha.SetAttributeArg(alpha);
}

///
/// @brief Set border color.
///
/// @param[in] color Border color.
///
auto ContainerView::SetBorderColor(AttributeArg<RGBAColor> color) -> void
{
    _borderColor.SetAttributeArg(color);
}

///
/// @brief Set border alpha.
///
/// @param[in] alpha Border alpha.
///
auto ContainerView::SetBorderAlpha(AttributeArg<Channel> alpha) -> void
{
    _borderAlpha.SetAttributeArg(alpha);
}

///
/// @brief Set border width.
///
/// @param[in] width Border width.
///
auto ContainerView::SetBorderWidth(AttributeArg<Dp> width) -> void
{
    _borderWidth.SetAttributeArg(width);
}

///
/// @brief Set corner radius.
///
/// @param[in] radius Corner radius.
///
auto ContainerView::SetCornerRadius(AttributeArg<CornerRadius> radius) -> void
{
    _cornerRadius.SetAttributeArg(radius);
}

///
/// @brief Initialize.
///
auto ContainerView::Initialize() -> void
{
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeBackgroundColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(CornerRadius, AttributeCornerRadius, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeBorderColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Dp, AttributeBorderWidth, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeBackgroundAlpha, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeBorderAlpha, {0});

    _backgroundColor.BindAttributeWithDefaultValue(*this, AttributeBackgroundColor, {});
    _backgroundAlpha.BindAttributeWithDefaultValue(*this, AttributeBackgroundAlpha, {1});
    _borderColor.BindAttributeWithDefaultValue(*this, AttributeBorderColor, {});
    _borderAlpha.BindAttributeWithDefaultValue(*this, AttributeBorderAlpha, {1});
    _borderWidth.BindAttributeWithDefaultValue(*this, AttributeBorderWidth, {0});
    _cornerRadius.BindAttributeWithDefaultValue(*this, AttributeCornerRadius, {});

    _backgroundBox = BoxView::Make();
    _backgroundBox->SetBackgroundColor(AttributeBackgroundColor);
    _backgroundBox->SetBackgroundAlpha(AttributeBackgroundAlpha);
    _backgroundBox->SetCornerRadius(AttributeCornerRadius);
    AddChildBack(_backgroundBox);

    _clipView = ClipView::Make();
    _clipView->SetCornerRadius(AttributeCornerRadius);
    AddChildBack(_clipView);

    _borderBox = BoxView::Make();
    _borderBox->SetBorderColor(AttributeBorderColor);
    _borderBox->SetBorderAlpha(AttributeBorderAlpha);
    _borderBox->SetBorderWidth(AttributeBorderWidth);
    _borderBox->SetCornerRadius(AttributeCornerRadius);
    AddChildBack(_borderBox);
}

///
/// @brief Measure.
///
auto ContainerView::Measure(MeasureScope& scope) -> void
{
    auto const contentSize = scope.MeasureChild(_clipView);
    scope.MeasureChild(_borderBox, BoxConstraints::MakeExact(contentSize));
    scope.MeasureChild(_backgroundBox, BoxConstraints::MakeExact(contentSize));
    scope.SetMeasuredSize(contentSize);
}
}

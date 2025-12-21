// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.ButtonRenderView.hpp"
#include "Futurewalker.Component.Lamp.ButtonViewStyle.hpp"

#include "Futurewalker.Application.ContainerView.hpp"
#include "Futurewalker.Application.DrawScope.hpp"
#include "Futurewalker.Application.ViewDrawFunction.hpp"

namespace FW_LAMP_DETAIL_NS
{
auto ButtonRenderView::Make() -> Shared<ButtonRenderView>
{
    return View::MakeDerived<ButtonRenderView>();
}

auto ButtonRenderView::MakeWithContent(Shared<View> const& content) -> Shared<ButtonRenderView>
{
    auto view = ButtonRenderView::Make();
    view->SetContent(content);
    return view;
}

ButtonRenderView::ButtonRenderView(PassKey<View> key)
  : View(key)
{
}

auto ButtonRenderView::GetContent() -> Shared<View>
{
    if (_container)
    {
        return _container->GetContent();
    }
    return {};
}

auto ButtonRenderView::SetContent(Shared<View> const& content) -> void
{
    if (_container)
    {
        _container->SetContent(content);
    }
}

auto ButtonRenderView::SetDown(Bool const down) -> void
{
    if (_down != down)
    {
        _down = down;
        InvalidateVisual();
    }
}

auto ButtonRenderView::SetEnter(Bool const enter) -> void
{
    if (_enter != enter)
    {
        _enter = enter;
        InvalidateVisual();
    }
}

auto ButtonRenderView::SetBackgroundColor(AttributeArg<RGBAColor> const& color) -> void
{
    _backgroundColor.SetAttributeArg(color);
}

auto ButtonRenderView::SetBackgroundAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _backgroundAlpha.SetAttributeArg(alpha);
}

auto ButtonRenderView::SetDisabledBackgroundColor(AttributeArg<RGBAColor> const& color) -> void
{
    _disabledBackgroundColor.SetAttributeArg(color);
}

auto ButtonRenderView::SetDisabledBackgroundAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _disabledBackgroundAlpha.SetAttributeArg(alpha);
}

auto ButtonRenderView::SetBorderColor(AttributeArg<RGBAColor> const& color) -> void
{
    _borderColor.SetAttributeArg(color);
}

auto ButtonRenderView::SetBorderAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _borderAlpha.SetAttributeArg(alpha);
}

auto ButtonRenderView::SetDisabledBorderColor(AttributeArg<RGBAColor> const& color) -> void
{
    _disabledBorderColor.SetAttributeArg(color);
}

auto ButtonRenderView::SetDisabledBorderAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _disabledBorderAlpha.SetAttributeArg(alpha);
}

auto ButtonRenderView::SetHighlightColor(AttributeArg<RGBAColor> const& color) -> void
{
    _highlightColor.SetAttributeArg(color);
}

auto ButtonRenderView::SetHoverHighlightAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _hoverHighlightAlpha.SetAttributeArg(alpha);
}

auto ButtonRenderView::SetPressHighlightAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _pressHighlightAlpha.SetAttributeArg(alpha);
}

auto ButtonRenderView::SetCornerRadius(AttributeArg<CornerRadius> const& radius) -> void
{
    _cornerRadius.SetAttributeArg(radius);
}

auto ButtonRenderView::SetBorderWidth(AttributeArg<Dp> const& width) -> void
{
    _borderWidth.SetAttributeArg(width);
}

void ButtonRenderView::Initialize()
{
    _container = ContainerView::Make();
    AddChildBack(_container);

    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeBackgroundColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeBackgroundAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeDisabledBackgroundColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeDisabledBackgroundAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeBorderColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeBorderAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeDisabledBorderColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeDisabledBorderAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeHighlightColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeHoverHighlightAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributePressHighlightAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(CornerRadius, AttributeCornerRadius, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Dp, AttributeBorderWidth, {0});

    _backgroundColor.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeBackgroundColor, ButtonViewStyle::BackgroundColor);
    _backgroundAlpha.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeBackgroundAlpha, ButtonViewStyle::BackgroundAlpha);
    _disabledBackgroundColor.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeDisabledBackgroundColor, ButtonViewStyle::DisabledBackgroundColor);
    _disabledBackgroundAlpha.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeDisabledBackgroundAlpha, ButtonViewStyle::DisabledBackgroundAlpha);
    _borderColor.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeBorderColor, ButtonViewStyle::BorderColor);
    _borderAlpha.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeBorderAlpha, ButtonViewStyle::BorderAlpha);
    _disabledBorderColor.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeDisabledBorderColor, ButtonViewStyle::DisabledBorderColor);
    _disabledBorderAlpha.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeDisabledBorderAlpha, ButtonViewStyle::DisabledBorderAlpha);
    _highlightColor.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeHighlightColor, ButtonViewStyle::HighlightColor);
    _hoverHighlightAlpha.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeHoverHighlightAlpha, ButtonViewStyle::HoverHighlightAlpha);
    _pressHighlightAlpha.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributePressHighlightAlpha, ButtonViewStyle::PressHighlightAlpha);
    _cornerRadius.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeCornerRadius, ButtonViewStyle::CornerRadius);
    _borderWidth.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeBorderWidth, ButtonViewStyle::BorderWidth);

    EventReceiver::Connect(*this, *this, &ButtonRenderView::ReceiveEvent);
}

///
/// @brief
///
/// @param scope
///
auto ButtonRenderView::Draw(DrawScope& scope) -> void
{
    auto const rect = GetContentRect();
    auto const backgroundColor = _backgroundColor.GetValueOrDefault();
    auto const highlightColor = _highlightColor.GetValueOrDefault();
    auto const borderColor = _borderColor.GetValueOrDefault();
    auto const cornerRadius = _cornerRadius.GetValueOrDefault();
    auto const borderWidth = _borderWidth.GetValueOr(0);
    auto const layoutDirection = GetLayoutDirection();

    auto& scene = scope.GetScene();

    ViewDrawFunction::DrawRoundRect(scene, rect, cornerRadius, backgroundColor, layoutDirection);
    ViewDrawFunction::DrawRoundRectBorder(scene, rect, cornerRadius, borderColor, borderWidth, layoutDirection);

    if (IsEnabledFromRoot())
    {
        auto highlightAlpha = Channel();
        if (_down)
        {
            highlightAlpha = _pressHighlightAlpha.GetValueOrDefault();
        }
        else if (_enter)
        {
            highlightAlpha = _hoverHighlightAlpha.GetValueOrDefault();
        }
        ViewDrawFunction::DrawRoundRect(scene, rect, cornerRadius, RGBAColor(highlightColor.GetRGBColor(), highlightAlpha), layoutDirection);
    }
}

///
/// @brief
///
/// @param event
///
auto ButtonRenderView::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<ViewEvent::EnabledChanged>())
    {
        InvalidateVisual();
    }
    co_return false;
}

///
/// @brief
///
/// @param event
///
auto ButtonRenderView::ReceiveAttributeEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
        InvalidateVisual();
    }
    co_return false;
}
}

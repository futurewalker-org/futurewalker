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
    return GetChildAt(0);
}

auto ButtonRenderView::GetContent() const -> Shared<View const>
{
    return GetChildAt(0);
}

auto ButtonRenderView::SetContent(Shared<View> const& content) -> void
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

auto ButtonRenderView::SetFocused(Bool const focused, FocusReason const focusReason) -> void
{
    if (_focused != focused || _focusReason != focusReason)
    {
        _focused = focused;
        _focusReason = focusReason;
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

auto ButtonRenderView::SetBorderWidth(AttributeArg<Dp> const& width) -> void
{
    _borderWidth.SetAttributeArg(width);
}

auto ButtonRenderView::SetDisabledBorderColor(AttributeArg<RGBAColor> const& color) -> void
{
    _disabledBorderColor.SetAttributeArg(color);
}

auto ButtonRenderView::SetDisabledBorderAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _disabledBorderAlpha.SetAttributeArg(alpha);
}

auto ButtonRenderView::SetDisabledBorderWidth(AttributeArg<Dp> const& width) -> void
{
    _disabledBorderWidth.SetAttributeArg(width);
}

auto ButtonRenderView::SetFocusedBorderColor(AttributeArg<RGBAColor> const& color) -> void
{
    _focusedBorderColor.SetAttributeArg(color);
}

auto ButtonRenderView::SetFocusedBorderAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _focusedBorderAlpha.SetAttributeArg(alpha);
}

auto ButtonRenderView::SetFocusedBorderWidth(AttributeArg<Dp> const& width) -> void
{
    _focusedBorderWidth.SetAttributeArg(width);
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

void ButtonRenderView::Initialize()
{
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeBackgroundColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeBackgroundAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeDisabledBackgroundColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeDisabledBackgroundAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeBorderColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeBorderAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Dp, AttributeBorderWidth, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeDisabledBorderColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeDisabledBorderAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Dp, AttributeDisabledBorderWidth, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeFocusedBorderColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeFocusedBorderAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Dp, AttributeFocusedBorderWidth, {0});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(RGBAColor, AttributeHighlightColor, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributeHoverHighlightAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(Channel, AttributePressHighlightAlpha, {});
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_VALUE(CornerRadius, AttributeCornerRadius, {});

    _backgroundColor.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeBackgroundColor, ButtonViewStyle::BackgroundColor);
    _backgroundAlpha.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeBackgroundAlpha, ButtonViewStyle::BackgroundAlpha);
    _disabledBackgroundColor.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeDisabledBackgroundColor, ButtonViewStyle::DisabledBackgroundColor);
    _disabledBackgroundAlpha.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeDisabledBackgroundAlpha, ButtonViewStyle::DisabledBackgroundAlpha);
    _borderColor.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeBorderColor, ButtonViewStyle::BorderColor);
    _borderAlpha.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeBorderAlpha, ButtonViewStyle::BorderAlpha);
    _borderWidth.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeBorderWidth, ButtonViewStyle::BorderWidth);
    _disabledBorderColor.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeDisabledBorderColor, ButtonViewStyle::DisabledBorderColor);
    _disabledBorderAlpha.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeDisabledBorderAlpha, ButtonViewStyle::DisabledBorderAlpha);
    _disabledBorderWidth.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeDisabledBorderWidth, ButtonViewStyle::DisabledBorderWidth);
    _focusedBorderColor.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeFocusedBorderColor, ButtonViewStyle::FocusedBorderColor);
    _focusedBorderAlpha.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeFocusedBorderAlpha, ButtonViewStyle::FocusedBorderAlpha);
    _focusedBorderWidth.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeFocusedBorderWidth, ButtonViewStyle::FocusedBorderWidth);
    _highlightColor.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeHighlightColor, ButtonViewStyle::HighlightColor);
    _hoverHighlightAlpha.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeHoverHighlightAlpha, ButtonViewStyle::HoverHighlightAlpha);
    _pressHighlightAlpha.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributePressHighlightAlpha, ButtonViewStyle::PressHighlightAlpha);
    _cornerRadius.BindAndConnectAttributeWithDefaultReference(*this, &ButtonRenderView::ReceiveAttributeEvent, AttributeCornerRadius, ButtonViewStyle::CornerRadius);

    EventReceiver::Connect(*this, *this, &ButtonRenderView::ReceiveEvent);
}

///
/// @brief
///
/// @param scope
///
auto ButtonRenderView::Draw(DrawScope& scope) -> void
{
    auto colorWithAlpha = [](RGBAColor const& color, Channel const& alpha) { return RGBAColor(color.GetRGBColor(), alpha.GetF64() * color.GetAlpha().GetF64()); };

    auto selectValue = [](auto const enabled, auto const focused, auto const& normalValue, auto const& focusedValue, auto const& disabledValue) {
        return enabled ? (focused ? focusedValue.GetValueOrDefault() : normalValue.GetValueOrDefault()) : disabledValue.GetValueOrDefault();
    };

    auto const focusVisible = _focused && _focusReason == FocusReason::Keyboard;

    auto const rect = GetContentRect();
    auto const enabled = IsEnabledFromRoot();
    auto const backgroundColor = selectValue(enabled, _focused, _backgroundColor, _backgroundColor, _disabledBackgroundColor);
    auto const backgroundAlpha = selectValue(enabled, _focused, _backgroundAlpha, _backgroundAlpha, _disabledBackgroundAlpha);
    auto const borderColor = selectValue(enabled, focusVisible, _borderColor, _focusedBorderColor, _disabledBorderColor);
    auto const borderAlpha = selectValue(enabled, focusVisible, _borderAlpha, _focusedBorderAlpha, _disabledBorderAlpha);
    auto const borderWidth = focusVisible ? _focusedBorderWidth.GetValueOr(0) : _borderWidth.GetValueOr(0);
    auto const highlightColor = _highlightColor.GetValueOrDefault();
    auto const cornerRadius = _cornerRadius.GetValueOrDefault();
    auto const layoutDirection = GetLayoutDirection();

    auto& scene = scope.GetScene();

    ViewDrawFunction::DrawRoundRect(scene, rect, cornerRadius, colorWithAlpha(backgroundColor, backgroundAlpha), layoutDirection);
    ViewDrawFunction::DrawRoundRectBorder(scene, rect, cornerRadius, colorWithAlpha(borderColor, borderAlpha), borderWidth, layoutDirection);

    if (enabled)
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
        ViewDrawFunction::DrawRoundRect(scene, rect, cornerRadius, colorWithAlpha(highlightColor, highlightAlpha), layoutDirection);
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

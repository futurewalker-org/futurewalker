// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.ButtonView.hpp"

#include "Futurewalker.Application.ButtonView.hpp"
#include "Futurewalker.Application.ButtonViewEvent.hpp"
#include "Futurewalker.Application.ViewDrawFunction.hpp"
#include "Futurewalker.Application.DrawScope.hpp"

#include "Futurewalker.Graphics.Scene.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
auto ButtonView::Make() -> Shared<ButtonView>
{
    return View::MakeDerived<ButtonView>();
}

///
/// @brief
///
/// @param contentView
///
auto ButtonView::MakeWithContent(Shared<View> contentView) -> Shared<ButtonView>
{
    auto view = Make();
    view->SetContent(contentView);
    return view;
}

///
/// @brief
///
auto ButtonView::GetContent() -> Shared<View>
{
    if (_buttonView)
    {
        return _buttonView->GetContent();
    }
    return {};
}

///
/// @brief
///
/// @param content
///
auto ButtonView::SetContent(Shared<View> content) -> void
{
    if (_buttonView)
    {
        _buttonView->SetContent(content);
    }
}

///
/// @brief
///
/// @param flags
///
auto ButtonView::SetActionFlags(ButtonViewActionFlags const flags) -> void
{
    if (_buttonView)
    {
        _buttonView->SetActionFlags(flags);
    }
}

///
/// @brief
///
/// @return
///
auto ButtonView::GetActionFlags() const -> ButtonViewActionFlags
{
    if (_buttonView)
    {
        return _buttonView->GetActionFlags();
    }
    return {};
}

///
/// @brief
///
/// @param color
///
auto ButtonView::SetBackgroundColor(AttributeArg<RGBAColor> const& color) -> void
{
    _backgroundColor.SetAttributeArg(color);
}

///
/// @brief
///
/// @param alpha
///
auto ButtonView::SetBackgroundAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _backgroundAlpha.SetAttributeArg(alpha);
}

///
/// @brief
///
/// @param color
///
auto ButtonView::SetDisabledBackgroundColor(AttributeArg<RGBAColor> const& color) -> void
{
    _disabledBackgroundColor.SetAttributeArg(color);
}

///
/// @brief
///
/// @param alpha
///
auto ButtonView::SetDisabledBackgroundAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _disabledBackgroundAlpha.SetAttributeArg(alpha);
}

///
/// @brief
///
/// @param color
///
auto ButtonView::SetBorderColor(AttributeArg<RGBAColor> const& color) -> void
{
    _borderColor.SetAttributeArg(color);
}

///
/// @brief
///
/// @param alpha
///
auto ButtonView::SetBorderAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _borderAlpha.SetAttributeArg(alpha);
}

///
/// @brief
///
/// @param color
///
auto ButtonView::SetDisabledBorderColor(AttributeArg<RGBAColor> const& color) -> void
{
    _disabledBorderColor.SetAttributeArg(color);
}

///
/// @brief
///
/// @param alpha
///
auto ButtonView::SetDisabledBorderAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _disabledBorderAlpha.SetAttributeArg(alpha);
}

///
/// @brief
///
/// @param color
///
auto ButtonView::SetHighlightColor(AttributeArg<RGBAColor> const& color) -> void
{
    _highlightColor.SetAttributeArg(color);
}

///
/// @brief
///
/// @param alpha
///
auto ButtonView::SetHoverHighlightAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _hoverHighlightAlpha.SetAttributeArg(alpha);
}

///
/// @brief
///
/// @param alpha
///
auto ButtonView::SetPressHighlightAlpha(AttributeArg<Channel> const& alpha) -> void
{
    _pressHighlightAlpha.SetAttributeArg(alpha);
}

///
/// @brief
///
/// @param radius
///
auto ButtonView::SetCornerRadius(AttributeArg<CornerRadius> const& radius) -> void
{
    _cornerRadius.SetAttributeArg(radius);
}

///
/// @brief
///
/// @param width
///
auto ButtonView::SetBorderWidth(AttributeArg<Dp> const& width) -> void
{
    _borderWidth.SetAttributeArg(width);
}

///
/// @brief
///
ButtonView::ButtonView(PassKey<View> key)
  : View(key)
{
}

///
/// @brief
///
auto ButtonView::Initialize() -> void
{
    _buttonView = ::FW_NS::ButtonView::Make();
    AddChildBack(_buttonView);

    EventReceiver::Connect(*_buttonView, *this, &ButtonView::ReceiveEvent);

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

    _backgroundColor.BindAndConnectAttributeWithDefaultReference(*this, &ButtonView::ReceiveAttributeEvent, AttributeBackgroundColor, ButtonViewStyle::BackgroundColor);
    _backgroundAlpha.BindAndConnectAttributeWithDefaultReference(*this, &ButtonView::ReceiveAttributeEvent, AttributeBackgroundAlpha, ButtonViewStyle::BackgroundAlpha);
    _disabledBackgroundColor.BindAndConnectAttributeWithDefaultReference(*this, &ButtonView::ReceiveAttributeEvent, AttributeDisabledBackgroundColor, ButtonViewStyle::DisabledBackgroundColor);
    _disabledBackgroundAlpha.BindAndConnectAttributeWithDefaultReference(*this, &ButtonView::ReceiveAttributeEvent, AttributeDisabledBackgroundAlpha, ButtonViewStyle::DisabledBackgroundAlpha);
    _borderColor.BindAndConnectAttributeWithDefaultReference(*this, &ButtonView::ReceiveAttributeEvent, AttributeBorderColor, ButtonViewStyle::BorderColor);
    _borderAlpha.BindAndConnectAttributeWithDefaultReference(*this, &ButtonView::ReceiveAttributeEvent, AttributeBorderAlpha, ButtonViewStyle::BorderAlpha);
    _disabledBorderColor.BindAndConnectAttributeWithDefaultReference(*this, &ButtonView::ReceiveAttributeEvent, AttributeDisabledBorderColor, ButtonViewStyle::DisabledBorderColor);
    _disabledBorderAlpha.BindAndConnectAttributeWithDefaultReference(*this, &ButtonView::ReceiveAttributeEvent, AttributeDisabledBorderAlpha, ButtonViewStyle::DisabledBorderAlpha);
    _highlightColor.BindAndConnectAttributeWithDefaultReference(*this, &ButtonView::ReceiveAttributeEvent, AttributeHighlightColor, ButtonViewStyle::HighlightColor);
    _hoverHighlightAlpha.BindAndConnectAttributeWithDefaultReference(*this, &ButtonView::ReceiveAttributeEvent, AttributeHoverHighlightAlpha, ButtonViewStyle::HoverHighlightAlpha);
    _pressHighlightAlpha.BindAndConnectAttributeWithDefaultReference(*this, &ButtonView::ReceiveAttributeEvent, AttributePressHighlightAlpha, ButtonViewStyle::PressHighlightAlpha);
    _cornerRadius.BindAndConnectAttributeWithDefaultReference(*this, &ButtonView::ReceiveAttributeEvent, AttributeCornerRadius, ButtonViewStyle::CornerRadius);
    _borderWidth.BindAndConnectAttributeWithDefaultReference(*this, &ButtonView::ReceiveAttributeEvent, AttributeBorderWidth, ButtonViewStyle::BorderWidth);
}

///
/// @brief
///
/// @param scope
///
auto ButtonView::Draw(DrawScope& scope) -> void
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

///
/// @brief
///
/// @param event
///
auto ButtonView::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<ButtonViewEvent>())
    {
        if (event.Is<ButtonViewEvent::Enter>())
        {
            SetEnter(true);
        }
        else if (event.Is<ButtonViewEvent::Leave>())
        {
            SetEnter(false);
        }
        else if (event.Is<ButtonViewEvent::Down>())
        {
            SetDown(true);
        }
        else if (event.Is<ButtonViewEvent::Up>())
        {
            SetDown(false);
        }
        co_await SendEvent(event);
        co_return true;
    }
    co_return false;
}

///
/// @brief
///
/// @param event
///
auto ButtonView::ReceiveAttributeEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
        InvalidateVisual();
    }
    co_return false;
}

///
/// @brief
///
/// @param down
///
auto ButtonView::SetDown(const Bool down) -> void
{
    if (_down != down)
    {
        _down = down;
        InvalidateVisual();
    }
}

///
/// @brief
///
/// @param enter
///
auto ButtonView::SetEnter(const Bool enter) -> void
{
    if (_enter != enter)
    {
        _enter = enter;
        InvalidateVisual();
    }
}
}
}

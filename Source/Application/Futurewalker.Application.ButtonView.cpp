// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ButtonView.hpp"
#include "Futurewalker.Application.TapGestureView.hpp"
#include "Futurewalker.Application.HoverGestureView.hpp"
#include "Futurewalker.Application.DrawScope.hpp"
#include "Futurewalker.Application.ViewDrawFunction.hpp"

#include "Futurewalker.Graphics.Scene.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Make instance.
///
auto ButtonView::Make() -> Shared<ButtonView>
{
    return View::MakeDerived<ButtonView>();
}

///
/// @brief Make instance with content view.
///
/// @param contentView 
///
auto ButtonView::MakeWithContent(Shared<View> contentView) -> Shared<ButtonView>
{
    auto const view = Make();
    view->SetContent(contentView);
    return view;
}

///
/// @brief Constructor.
///
ButtonView::ButtonView(PassKey<View> key)
  : View(key)
{
}

///
/// @brief Set content view.
///
/// @param content 
///
auto ButtonView::SetContent(Shared<View> content) -> void
{
    if (_tapGestureView)
    {
        _tapGestureView->SetContent(content);
    }
}

///
/// @brief Get content view.
///
auto ButtonView::GetContent() -> Shared<View>
{
    return _tapGestureView->GetContent();
}

///
/// @brief 
///
/// @param flags 
///
auto ButtonView::SetActionFlags(ButtonViewActionFlags const flags) -> void
{
    _actionFlags = flags;
}

///
/// @brief
///
auto ButtonView::GetActionFlags() const -> ButtonViewActionFlags
{
    return _actionFlags;
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
/// @brief Initialize.
///
auto ButtonView::Initialize() -> void
{
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

    _tapGestureView = TapGestureView::Make();
    _hoverGestureView = HoverGestureView::MakeWithContent(_tapGestureView);

    auto bindAndConnectAttribute = [this]<class Accessor>(Accessor& accessor, StaticAttributeRef<typename Accessor::ValueType> attribute) {
        accessor.BindAttribute(*this, attribute);
        EventReceiver::Connect(accessor, *this, &ButtonView::ReceiveAttributeEvent);
    };
    bindAndConnectAttribute(_backgroundColor, AttributeBackgroundColor);
    bindAndConnectAttribute(_backgroundAlpha, AttributeBackgroundAlpha);
    bindAndConnectAttribute(_disabledBackgroundColor, AttributeDisabledBackgroundColor);
    bindAndConnectAttribute(_disabledBackgroundAlpha, AttributeDisabledBackgroundAlpha);
    bindAndConnectAttribute(_borderColor, AttributeBorderColor);
    bindAndConnectAttribute(_borderAlpha, AttributeBorderAlpha);
    bindAndConnectAttribute(_disabledBorderColor, AttributeDisabledBorderColor);
    bindAndConnectAttribute(_disabledBorderAlpha, AttributeDisabledBorderAlpha);
    bindAndConnectAttribute(_highlightColor, AttributeHighlightColor);
    bindAndConnectAttribute(_hoverHighlightAlpha, AttributeHoverHighlightAlpha);
    bindAndConnectAttribute(_pressHighlightAlpha, AttributePressHighlightAlpha);
    bindAndConnectAttribute(_cornerRadius, AttributeCornerRadius);
    bindAndConnectAttribute(_borderWidth, AttributeBorderWidth);

    AddChildBack(_hoverGestureView);

    EventReceiver::Connect(*this, *this, &ButtonView::ReceiveEvent);
}

///
/// @brief Draw.
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
/// @brief Receive view events.
///
auto ButtonView::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<ViewEvent::Notify>())
    {
        auto const sender = event.As<ViewEvent::Notify>()->GetSender();
        if (sender == _tapGestureView || sender == _hoverGestureView)
        {
            if (event.Is<TapGestureViewEvent>())
            {
                if (event.Is<TapGestureViewEvent::Down>())
                {
                    SetDown(true);
                }
                else if (event.Is<TapGestureViewEvent::Up>() || event.Is<TapGestureViewEvent::Cancel>())
                {
                    SetDown(false);
                }
                else if (event.Is<TapGestureViewEvent::Tap>())
                {
                    auto buttonEvent = Event<>(Event<ButtonViewEvent::Press>::Make());
                    SendEventDetached(buttonEvent);
                }
                co_return true;
            }
            else if (event.Is<HoverGestureViewEvent>())
            {
                if (event.Is<HoverGestureViewEvent::Enter>())
                {
                    SetEnter(true);
                }
                else if (event.Is<HoverGestureViewEvent::Leave>())
                {
                    SetEnter(false);
                }
                co_return true;
            }
        }
    }
    co_return false;
}

///
/// @brief Receive attribute events.
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
/// @brief Set down state.
///
auto ButtonView::SetDown(const Bool down) -> void
{
    if (_down != down)
    {
        _down = down;

        InvalidateVisual();

        if (_down)
        {
            auto event = Event<>(Event<ButtonViewEvent::Down>::Make());
            SendEventDetached(event);
        }
        else
        {
            auto event = Event<>(Event<ButtonViewEvent::Up>::Make());
            SendEventDetached(event);
        }
    }
}

///
/// @brief Set hover state.
///
auto ButtonView::SetEnter(const Bool enter) -> void
{
    if (_enter != enter)
    {
        _enter = enter;

        InvalidateVisual();

        if (_enter)
        {
            auto event = Event<>(Event<ButtonViewEvent::Enter>::Make());
            SendEventDetached(event);
        }
        else
        {
            auto event = Event<>(Event<ButtonViewEvent::Leave>::Make());
            SendEventDetached(event);
        }
    }
}
}

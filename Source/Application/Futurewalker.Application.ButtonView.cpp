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
/// @brief Constructor.
///
ButtonView::ButtonView(PassKey<View> key)
  : View(key)
{
}

///
/// @brief Initialize.
///
auto ButtonView::Initialize() -> void
{
    _tapGestureView = TapGestureView::Make();
    _hoverGestureView = HoverGestureView::MakeWithContent(_tapGestureView);
    _backgroundColor.BindAttribute(*this, ButtonViewStyle::BackgroundColor);
    _highlightColor.BindAttribute(*this, ButtonViewStyle::HighlightColor);
    _borderColor.BindAttribute(*this, ButtonViewStyle::BorderColor);
    _hoverHighlightAlpha.BindAttribute(*this, ButtonViewStyle::HoverHighlightAlpha);
    _pressHighlightAlpha.BindAttribute(*this, ButtonViewStyle::PressHighlightAlpha);
    _cornerRadius.BindAttribute(*this, ButtonViewStyle::CornerRadius);
    _borderWidth.BindAttribute(*this, ButtonViewStyle::BorderWidth);

    AddChildBack(_hoverGestureView);

    EventReceiver::Connect(*this, *this, &ButtonView::ReceiveEvent);
    EventReceiver::Connect(_backgroundColor, *this, &ButtonView::ReceiveAttributeEvent);
    EventReceiver::Connect(_highlightColor, *this, &ButtonView::ReceiveAttributeEvent);
    EventReceiver::Connect(_borderColor, *this, &ButtonView::ReceiveAttributeEvent);
    EventReceiver::Connect(_hoverHighlightAlpha, *this, &ButtonView::ReceiveAttributeEvent);
    EventReceiver::Connect(_pressHighlightAlpha, *this, &ButtonView::ReceiveAttributeEvent);
    EventReceiver::Connect(_cornerRadius, *this, &ButtonView::ReceiveAttributeEvent);
    EventReceiver::Connect(_borderWidth, *this, &ButtonView::ReceiveAttributeEvent);
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

    auto highlightAlpha = AlphaChannel();
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

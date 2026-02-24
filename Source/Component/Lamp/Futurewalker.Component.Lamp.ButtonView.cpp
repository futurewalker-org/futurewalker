// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.ButtonView.hpp"
#include "Futurewalker.Component.Lamp.ButtonRenderView.hpp"

#include "Futurewalker.Application.TapGestureView.hpp"
#include "Futurewalker.Application.HoverGestureView.hpp"
#include "Futurewalker.Application.ContainerView.hpp"
#include "Futurewalker.Application.FocusEvent.hpp"
#include "Futurewalker.Application.KeyEvent.hpp"
#include "Futurewalker.Application.Key.hpp"

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
    if (_containerView)
    {
        return _containerView->GetContent();
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
    if (_containerView)
    {
        _containerView->SetContent(content);
    }
}

///
/// @brief
///
/// @param flags
///
auto ButtonView::SetActionFlags(Flags<ButtonViewActionFlag> const flags) -> void
{
    _actionFlags = flags;
}

///
/// @brief
///
/// @return
///
auto ButtonView::GetActionFlags() const -> Flags<ButtonViewActionFlag>
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
    if (_renderView)
    {
        _renderView->SetBackgroundColor(color);
    }
}

///
/// @brief
///
/// @param alpha
///
auto ButtonView::SetBackgroundAlpha(AttributeArg<Channel> const& alpha) -> void
{
    if (_renderView)
    {
        _renderView->SetBackgroundAlpha(alpha);
    }
}

///
/// @brief
///
/// @param color
///
auto ButtonView::SetDisabledBackgroundColor(AttributeArg<RGBAColor> const& color) -> void
{
    if (_renderView)
    {
        _renderView->SetDisabledBackgroundColor(color);
    }
}

///
/// @brief
///
/// @param alpha
///
auto ButtonView::SetDisabledBackgroundAlpha(AttributeArg<Channel> const& alpha) -> void
{
    if (_renderView)
    {
        _renderView->SetDisabledBackgroundAlpha(alpha);
    }
}

///
/// @brief
///
/// @param color
///
auto ButtonView::SetBorderColor(AttributeArg<RGBAColor> const& color) -> void
{
    if (_renderView)
    {
        _renderView->SetBorderColor(color);
    }
}

///
/// @brief
///
/// @param alpha
///
auto ButtonView::SetBorderAlpha(AttributeArg<Channel> const& alpha) -> void
{
    if (_renderView)
    {
        _renderView->SetBorderAlpha(alpha);
    }
}

///
/// @brief
///
/// @param width
///
auto ButtonView::SetBorderWidth(AttributeArg<Dp> const& width) -> void
{
    if (_renderView)
    {
        _renderView->SetBorderWidth(width);
    }
}

///
/// @brief
///
/// @param color
///
auto ButtonView::SetDisabledBorderColor(AttributeArg<RGBAColor> const& color) -> void
{
    if (_renderView)
    {
        _renderView->SetDisabledBorderColor(color);
    }
}

///
/// @brief
///
/// @param alpha
///
auto ButtonView::SetDisabledBorderAlpha(AttributeArg<Channel> const& alpha) -> void
{
    if (_renderView)
    {
        _renderView->SetDisabledBorderAlpha(alpha);
    }
}

///
/// @brief 
///
auto ButtonView::SetDisabledBorderWidth(AttributeArg<Dp> const& width) -> void
{
    if (_renderView)
    {
        _renderView->SetDisabledBorderWidth(width);
    }
}

///
/// @brief 
///
auto ButtonView::SetFocusedBorderColor(AttributeArg<RGBAColor> const& color) -> void
{
    if (_renderView)
    {
        _renderView->SetFocusedBorderColor(color);
    }
}

///
/// @brief 
///
auto ButtonView::SetFocusedBorderAlpha(AttributeArg<Channel> const& alpha) -> void
{
    if (_renderView)
    {
        _renderView->SetFocusedBorderAlpha(alpha);
    }
}

///
/// @brief 
///
auto ButtonView::SetFocusedBorderWidth(AttributeArg<Dp> const& width) -> void
{
    if (_renderView)
    {
        _renderView->SetFocusedBorderWidth(width);
    }
}

///
/// @brief
///
/// @param color
///
auto ButtonView::SetHighlightColor(AttributeArg<RGBAColor> const& color) -> void
{
    if (_renderView)
    {
        _renderView->SetHighlightColor(color);
    }
}

///
/// @brief
///
/// @param alpha
///
auto ButtonView::SetHoverHighlightAlpha(AttributeArg<Channel> const& alpha) -> void
{
    if (_renderView)
    {
        _renderView->SetHoverHighlightAlpha(alpha);
    }
}

///
/// @brief
///
/// @param alpha
///
auto ButtonView::SetPressHighlightAlpha(AttributeArg<Channel> const& alpha) -> void
{
    if (_renderView)
    {
        _renderView->SetPressHighlightAlpha(alpha);
    }
}

///
/// @brief
///
/// @param radius
///
auto ButtonView::SetCornerRadius(AttributeArg<CornerRadius> const& radius) -> void
{
    if (_renderView)
    {
        _renderView->SetCornerRadius(radius);
    }
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
    _containerView = ContainerView::Make();
    _tapGestureView = TapGestureView::MakeWithContent(_containerView);
    _hoverGestureView = HoverGestureView::MakeWithContent(_tapGestureView);
    _renderView = ButtonRenderView::MakeWithContent(_hoverGestureView);
    AddChildBack(_renderView);

    SetFocusTrackingFlags(ViewFocusTrackingFlags::All);

    EventReceiver::Connect(*this, *this, &ButtonView::ReceiveEvent);
}

///
/// @brief
///
/// @param event
///
auto ButtonView::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<FocusEvent>())
    {
        if (event.Is<FocusEvent::FocusIn>())
        {
            if (_renderView)
            {
                _renderView->SetFocused(true, event.As<FocusEvent::FocusIn>()->GetReason());
            }
        }
        else if (event.Is<FocusEvent::FocusOut>())
        {
            _keyDown = false;

            if (_renderView)
            {
                _renderView->SetFocused(false, {});
            }
        }
    }
    else if (event.Is<KeyEvent>())
    {
        if (event.Is<KeyEvent::Down>())
        {
            auto const key = event.As<KeyEvent::Down>()->GetKey();
            auto const unmodifiedKey = event.As<KeyEvent::Down>()->GetUnmodifiedKey();
            if ((key != Key::Process) && (unmodifiedKey == Key::Enter || unmodifiedKey == Key::Space))
            {
                if (!_keyDown && !_tapDown)
                {
                    _keyDown = true;
                    _renderView->SetDown(true);
                    _renderView->SetFocused(IsFocused(), FocusReason::Keyboard);
                    auto buttonEvent = Event<>(Event<ButtonViewEvent::Down>());
                    SendEventDetached(buttonEvent);
                }
                co_return true;
            }
        }
        else if (event.Is<KeyEvent::Up>())
        {
            auto const key = event.As<KeyEvent::Up>()->GetKey();
            auto const unmodifiedKey = event.As<KeyEvent::Up>()->GetUnmodifiedKey();
            if ((key != Key::Process) && (unmodifiedKey == Key::Enter || unmodifiedKey == Key::Space))
            {
                if (_keyDown)
                {
                    _keyDown = false;
                    _renderView->SetDown(false);
                    auto buttonEvent = Event<>(Event<ButtonViewEvent::Press>());
                    SendEventDetached(buttonEvent);
                }
                co_return true;
            }
        }
    }
    else if (event.Is<ViewEvent::Notify>())
    {
        auto const sender = event.As<ViewEvent::Notify>()->GetSender();
        if (sender == _tapGestureView || sender == _hoverGestureView)
        {
            if (event.Is<TapGestureViewEvent>())
            {
                if (event.Is<TapGestureViewEvent::Begin>())
                {
                    if (!_tapDown && !_keyDown)
                    {
                        _tapDown = true;
                        _renderView->SetDown(true);
                    }
                    RequestFocus(FocusReason::Pointer);
                    auto buttonEvent = Event<>(Event<ButtonViewEvent::Down>());
                    SendEventDetached(buttonEvent);
                }
                else if (event.Is<TapGestureViewEvent::Cancel>())
                {
                    _tapDown = false;
                    _renderView->SetDown(false);
                    auto buttonEvent = Event<>(Event<ButtonViewEvent::Cancel>());
                    SendEventDetached(buttonEvent);
                }
                else if (event.Is<TapGestureViewEvent::Tap>())
                {
                    if (_tapDown)
                    {
                        _tapDown = false;
                        _renderView->SetDown(false);
                        auto buttonEvent = Event<>(Event<ButtonViewEvent::Press>());
                        SendEventDetached(buttonEvent);
                    }
                }
                co_return true;
            }
            else if (event.Is<HoverGestureViewEvent>())
            {
                if (event.Is<HoverGestureViewEvent::Enter>())
                {
                    _hoverEnter = true;
                    _renderView->SetEnter(true);
                    auto buttonEvent = Event<>(Event<ButtonViewEvent::Enter>());
                    SendEventDetached(buttonEvent);
                }
                else if (event.Is<HoverGestureViewEvent::Leave>())
                {
                    _hoverEnter = false;
                    _renderView->SetEnter(false);
                    auto buttonEvent = Event<>(Event<ButtonViewEvent::Leave>());
                    SendEventDetached(buttonEvent);
                }
                co_return true;
            }
        }
    }
    co_return false;
}
}
}

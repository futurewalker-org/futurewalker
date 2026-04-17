// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.MenuButtonView.hpp"
#include "Futurewalker.Component.Lamp.ButtonRenderView.hpp"

#include "Futurewalker.Application.ContainerView.hpp"
#include "Futurewalker.Application.PointerEvent.hpp"
#include "Futurewalker.Application.FocusEvent.hpp"
#include "Futurewalker.Application.KeyEvent.hpp"
#include "Futurewalker.Application.Key.hpp"

namespace FW_LAMP_DETAIL_NS
{
auto MenuButtonView::Make() -> Shared<MenuButtonView>
{
    return View::MakeDerived<MenuButtonView>();
}

auto MenuButtonView::MakeWithContent(Shared<View> const& content) -> Shared<MenuButtonView>
{
    auto button = Make();
    button->SetContent(content);
    return button;
}

MenuButtonView::MenuButtonView(PassKey<View> key)
  : View(key)
{
}

auto MenuButtonView::Initialize() -> void
{
    SetPointerTrackingFlags(ViewPointerTrackingFlag::All);
    SetFocusTrackingFlags(ViewFocusTrackingFlag::All);

    _container = ContainerView::Make();
    _buttonView = ButtonRenderView::MakeWithContent(_container);
    AddChildBack(_buttonView);

    EventReceiver::Connect(*this, *this, &MenuButtonView::ReceiveEvent);
}


auto MenuButtonView::GetContent() -> Shared<View>
{
    if (_container)
    {
        return _container->GetContent();
    }
    return {};
}

auto MenuButtonView::SetContent(Shared<View> const& content) -> void
{
    if (_container)
    {
        _container->SetContent(content);
    }
}

auto MenuButtonView::SetBackgroundColor(AttributeArg<RGBAColor> const& color) -> void
{
    if (_buttonView)
    {
        _buttonView->SetBackgroundColor(color);
    }
}

auto MenuButtonView::SetBackgroundAlpha(AttributeArg<Channel> const& alpha) -> void
{
    if (_buttonView)
    {
        _buttonView->SetBackgroundAlpha(alpha);
    }
}

auto MenuButtonView::SetDisabledBackgroundColor(AttributeArg<RGBAColor> const& color) -> void
{
    if (_buttonView)
    {
        _buttonView->SetDisabledBackgroundColor(color);
    }
}

auto MenuButtonView::SetDisabledBackgroundAlpha(AttributeArg<Channel> const& alpha) -> void
{
    if (_buttonView)
    {
        _buttonView->SetDisabledBackgroundAlpha(alpha);
    }
}

auto MenuButtonView::SetBorderColor(AttributeArg<RGBAColor> const& color) -> void
{
    if (_buttonView)
    {
        _buttonView->SetBorderColor(color);
    }
}

auto MenuButtonView::SetBorderAlpha(AttributeArg<Channel> const& alpha) -> void
{
    if (_buttonView)
    {
        _buttonView->SetBorderAlpha(alpha);
    }
}

auto MenuButtonView::SetBorderWidth(AttributeArg<Dp> const& width) -> void
{
    if (_buttonView)
    {
        _buttonView->SetBorderWidth(width);
    }
}

auto MenuButtonView::SetDisabledBorderColor(AttributeArg<RGBAColor> const& color) -> void
{
    if (_buttonView)
    {
        _buttonView->SetDisabledBorderColor(color);
    }
}

auto MenuButtonView::SetDisabledBorderAlpha(AttributeArg<Channel> const& alpha) -> void
{
    if (_buttonView)
    {
        _buttonView->SetDisabledBorderAlpha(alpha);
    }
}

auto MenuButtonView::SetDisabledBorderWidth(AttributeArg<Dp> const& width) -> void
{
    if (_buttonView)
    {
        _buttonView->SetDisabledBorderWidth(width);
    }
}

auto MenuButtonView::SetFocusedBorderColor(AttributeArg<RGBAColor> const& color) -> void
{
    if (_buttonView)
    {
        _buttonView->SetFocusedBorderColor(color);
    }
}

auto MenuButtonView::SetFocusedBorderAlpha(AttributeArg<Channel> const& alpha) -> void
{
    if (_buttonView)
    {
        _buttonView->SetFocusedBorderAlpha(alpha);
    }
}

auto MenuButtonView::SetFocusedBorderWidth(AttributeArg<Dp> const& width) -> void
{
    if (_buttonView)
    {
        _buttonView->SetFocusedBorderWidth(width);
    }
}

auto MenuButtonView::SetHighlightColor(AttributeArg<RGBAColor> const& color) -> void
{
    if (_buttonView)
    {
        _buttonView->SetHighlightColor(color);
    }
}

auto MenuButtonView::SetHoverHighlightAlpha(AttributeArg<Channel> const& alpha) -> void
{
    if (_buttonView)
    {
        _buttonView->SetHoverHighlightAlpha(alpha);
    }
}

auto MenuButtonView::SetPressHighlightAlpha(AttributeArg<Channel> const& alpha) -> void
{
    if (_buttonView)
    {
        _buttonView->SetPressHighlightAlpha(alpha);
    }
}

auto MenuButtonView::SetCornerRadius(AttributeArg<CornerRadius> const& radius) -> void
{
    if (_buttonView)
    {
        _buttonView->SetCornerRadius(radius);
    }
}

auto MenuButtonView::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<PointerEvent>())
    {
        if (IsEnabledFromRoot())
        {
            if (event.Is<PointerEvent::Motion::Down>())
            {
                if (!_pointerDown)
                {
                    _pointerDown = true;
                    _buttonView->SetDown(true);
                    RequestFocus(FocusReason::Pointer);
                    auto buttonEvent = Event<>(Event<MenuButtonViewEvent::Down>());
                    co_await SendEvent(buttonEvent);
                }
            }
            else if (event.Is<PointerEvent::Motion::Up>())
            {
                if (_pointerDown)
                {
                    _pointerDown = false;
                    _buttonView->SetDown(false);
                    auto buttonEvent = Event<>(Event<MenuButtonViewEvent::Up>());
                    co_await SendEvent(buttonEvent);
                }
            }
            else if (event.Is<PointerEvent::Motion::Enter>())
            {
                if (!_pointerEnter)
                {
                    _pointerEnter = true;
                    _buttonView->SetEnter(true);
                    auto buttonEvent = Event<>(Event<MenuButtonViewEvent::Enter>());
                    co_await SendEvent(buttonEvent);
                }
            }
            else if (event.Is<PointerEvent::Motion::Leave>())
            {
                if (_pointerEnter)
                {
                    _pointerDown = false;
                    _pointerEnter = false;
                    _buttonView->SetDown(false);
                    _buttonView->SetEnter(false);
                    auto buttonEvent = Event<>(Event<MenuButtonViewEvent::Leave>());
                    co_await SendEvent(buttonEvent);
                }
            }
            else if (event.Is<PointerEvent::Motion::Move>())
            {
                if (event.As<PointerEvent::Motion::Move>()->GetButtons() != PointerButtonFlag::None)
                {
                    if (!_pointerDown)
                    {
                        _pointerDown = true;
                        _buttonView->SetDown(true);
                    }
                }
            }
            else if (event.Is<PointerEvent::Motion::Cancel>())
            {
                auto const down = std::exchange(_pointerDown, false);
                auto const enter = std::exchange(_pointerEnter, false);

                _buttonView->SetDown(false);
                _buttonView->SetEnter(false);

                if (down)
                {
                    auto buttonEvent = Event<>(Event<MenuButtonViewEvent::Up>());
                    co_await SendEvent(buttonEvent);
                }
                if (enter)
                {
                    auto buttonEvent = Event<>(Event<MenuButtonViewEvent::Leave>());
                    co_await SendEvent(buttonEvent);
                }
            }
            co_return true;
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
                if (!_keyDown)
                {
                    _keyDown = true;
                    _buttonView->SetDown(true);
                    auto buttonEvent = Event<>(Event<MenuButtonViewEvent::Down>());
                    co_await SendEvent(buttonEvent);
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
                    _buttonView->SetDown(false);

                    auto buttonEvent = Event<>(Event<MenuButtonViewEvent::Up>());
                    co_await SendEvent(buttonEvent);
                }
                co_return true;
            }
        }
    }
    else if (event.Is<FocusEvent>())
    {
        if (event.Is<FocusEvent::FocusIn>())
        {
            _buttonView->SetFocused(true, event.As<FocusEvent::FocusIn>()->GetReason());
        }
        else if (event.Is<FocusEvent::FocusOut>())
        {
            _keyDown = false;
            _buttonView->SetDown(false);
            _buttonView->SetFocused(false, {});
        }
    }
    co_return false;
}
}

// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.PopupMenuButton.hpp"
#include "Futurewalker.Component.Lamp.PopupMenu.hpp"
#include "Futurewalker.Component.Lamp.ButtonRenderView.hpp"

#include "Futurewalker.Application.PaddingView.hpp"

#include "Futurewalker.Base.Debug.hpp" 

namespace FW_LAMP_DETAIL_NS
{
auto PopupMenuButton::Make() -> Shared<PopupMenuButton>
{
    return View::MakeDerived<PopupMenuButton>();
}

auto PopupMenuButton::MakeWithContent(Shared<View> const& content) -> Shared<PopupMenuButton>
{
    auto button = PopupMenuButton::Make();
    button->SetContent(content);
    return button;
}

auto PopupMenuButton::GetContent() -> Shared<View>
{
    if (_paddingView)
    {
        return _paddingView->GetContent();
    }
    return {};
}

auto PopupMenuButton::SetContent(Shared<View> const& content) -> void
{
    if (_paddingView)
    {
        _paddingView->SetContent(content);
    }
}

auto PopupMenuButton::GetMenu() const -> Menu
{
    return _menu;
}

auto PopupMenuButton::SetMenu(Menu const& menu) -> void
{
    _menu = menu;
    UpdatePopup();
}

auto PopupMenuButton::SetAnchorEdge(PopupMenuAnchorEdge edge) -> void
{
    if (_anchorEdge != edge)
    {
        _anchorEdge = edge;
        UpdatePopup();
    }
}

auto PopupMenuButton::SetAnchorAlignment(PopupMenuAnchorAlignment alignment) -> void
{
    if (_anchorAlignment != alignment)
    {
        _anchorAlignment = alignment;
        UpdatePopup();
    }
}

PopupMenuButton::PopupMenuButton(PassKey<View> key)
  : View(key)
{
}

auto PopupMenuButton::Initialize() -> void
{
    SetPointerTrackingFlags(ViewPointerTrackingFlags::All);

    _paddingView = PaddingView::Make();
    _paddingView->SetPadding(PopupMenuButtonStyle::Padding);

    _buttonRenderView = ButtonRenderView::MakeWithContent(_paddingView);
    _buttonRenderView->SetBackgroundColor(PopupMenuButtonStyle::BackgroundColor);
    _buttonRenderView->SetBackgroundAlpha(PopupMenuButtonStyle::BackgroundAlpha);
    _buttonRenderView->SetDisabledBackgroundColor(PopupMenuButtonStyle::DisabledBackgroundColor);
    _buttonRenderView->SetDisabledBackgroundAlpha(PopupMenuButtonStyle::DisabledBackgroundAlpha);
    _buttonRenderView->SetBorderColor(PopupMenuButtonStyle::BorderColor);
    _buttonRenderView->SetBorderAlpha(PopupMenuButtonStyle::BorderAlpha);
    _buttonRenderView->SetDisabledBorderColor(PopupMenuButtonStyle::DisabledBorderColor);
    _buttonRenderView->SetDisabledBorderAlpha(PopupMenuButtonStyle::DisabledBorderAlpha);
    _buttonRenderView->SetHighlightColor(PopupMenuButtonStyle::HighlightColor);
    _buttonRenderView->SetHoverHighlightAlpha(PopupMenuButtonStyle::HoverHighlightAlpha);
    _buttonRenderView->SetPressHighlightAlpha(PopupMenuButtonStyle::PressHighlightAlpha);
    _buttonRenderView->SetCornerRadius(PopupMenuButtonStyle::CornerRadius);
    _buttonRenderView->SetBorderWidth(PopupMenuButtonStyle::BorderWidth);
    AddChildBack(_buttonRenderView);

    EventReceiver::Connect(*this, *this, &PopupMenuButton::ReceiveEvent);
}

auto PopupMenuButton::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (IsEnabledFromRoot() && event.Is<PointerEvent>())
    {
        if (event.Is<PointerEvent::Motion::Down>())
        {
            SetDown(true);
            if (!_menu.IsEmpty())
            {
                CreatePopup();
                UpdatePopup();
            }
            auto downEvent = Event<>(Event<PopupMenuButtonEvent::Down>());
            co_await SendEvent(downEvent);
        }
        else if (event.Is<PointerEvent::Motion::Up>())
        {
            SetDown(false);
            auto upEvent = Event<>(Event<PopupMenuButtonEvent::Up>());
            co_await SendEvent(upEvent);
        }
        else if (event.Is<PointerEvent::Motion::Enter>())
        {
            SetEnter(true);
            auto enterEvent = Event<>(Event<PopupMenuButtonEvent::Enter>());
            co_await SendEvent(enterEvent);
        }
        else if (event.Is<PointerEvent::Motion::Leave>())
        {
            SetDown(false);
            SetEnter(false);
            auto leaveEvent = Event<>(Event<PopupMenuButtonEvent::Leave>());
            co_await SendEvent(leaveEvent);
        }
        else if (event.Is<PointerEvent::Motion::Cancel>())
        {
            auto const down = IsDown();
            auto const enter = IsEnter();
            SetDown(false);
            SetEnter(false);

            if (down)
            {
                auto upEvent = Event<>(Event<PopupMenuButtonEvent::Up>());
                co_await SendEvent(upEvent);
            }
            if (enter)
            {
                auto leaveEvent = Event<>(Event<PopupMenuButtonEvent::Leave>());
                co_await SendEvent(leaveEvent);
            }
        }
        co_return true;
    }
    else if (event.Is<PopupMenuEvent>())
    {
        if (event.Is<PopupMenuEvent::Closed>())
        {
            DestroyPopup();
        }
        co_return true;
    }
    co_return false;
}

auto PopupMenuButton::CreatePopup() -> void
{
    if (!_popupMenu)
    {
        _popupMenu = PopupMenu::Make();
        EventReceiver::Connect(*_popupMenu, *this, &PopupMenuButton::ReceiveEvent);
    }
}

auto PopupMenuButton::DestroyPopup() -> void
{
    _popupMenu = nullptr;
}

auto PopupMenuButton::UpdatePopup() -> void
{
    if (_popupMenu)
    {
        _popupMenu->SetMenu(_menu);
        _popupMenu->SetAnchorRect(GetContentRect());
        _popupMenu->SetAnchorEdge(_anchorEdge);
        _popupMenu->SetAnchorAlignment(_anchorAlignment);
        _popupMenu->Show(GetSelf());
    }
}

auto PopupMenuButton::IsDown() const -> Bool
{
    if (_buttonRenderView)
    {
        return _buttonRenderView->IsDown();
    }
    return false;
}

auto PopupMenuButton::IsEnter() const -> Bool
{
    if (_buttonRenderView)
    {
        return _buttonRenderView->IsEnter();
    }
    return false;
}

auto PopupMenuButton::SetDown(Bool const down) -> void
{
    if (_buttonRenderView)
    {
        _buttonRenderView->SetDown(down);
    }
}

auto PopupMenuButton::SetEnter(Bool const enter) -> void
{
    if (_buttonRenderView)
    {
        _buttonRenderView->SetEnter(enter);
    }
}
}

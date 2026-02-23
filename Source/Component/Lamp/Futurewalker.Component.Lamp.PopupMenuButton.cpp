// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.PopupMenuButton.hpp"
#include "Futurewalker.Component.Lamp.PopupMenu.hpp"
#include "Futurewalker.Component.Lamp.ButtonRenderView.hpp"
#include "Futurewalker.Component.Lamp.MenuButtonView.hpp"

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

    _buttonView = MenuButtonView::MakeWithContent(_paddingView);
    _buttonView->SetBackgroundColor(PopupMenuButtonStyle::BackgroundColor);
    _buttonView->SetBackgroundAlpha(PopupMenuButtonStyle::BackgroundAlpha);
    _buttonView->SetDisabledBackgroundColor(PopupMenuButtonStyle::DisabledBackgroundColor);
    _buttonView->SetDisabledBackgroundAlpha(PopupMenuButtonStyle::DisabledBackgroundAlpha);
    _buttonView->SetBorderColor(PopupMenuButtonStyle::BorderColor);
    _buttonView->SetBorderAlpha(PopupMenuButtonStyle::BorderAlpha);
    _buttonView->SetBorderWidth(PopupMenuButtonStyle::BorderWidth);
    _buttonView->SetDisabledBorderColor(PopupMenuButtonStyle::DisabledBorderColor);
    _buttonView->SetDisabledBorderAlpha(PopupMenuButtonStyle::DisabledBorderAlpha);
    _buttonView->SetDisabledBorderWidth(PopupMenuButtonStyle::DisabledBorderWidth);
    _buttonView->SetFocusedBorderColor(PopupMenuButtonStyle::FocusedBorderColor);
    _buttonView->SetFocusedBorderAlpha(PopupMenuButtonStyle::FocusedBorderAlpha);
    _buttonView->SetFocusedBorderWidth(PopupMenuButtonStyle::FocusedBorderWidth);
    _buttonView->SetHighlightColor(PopupMenuButtonStyle::HighlightColor);
    _buttonView->SetHoverHighlightAlpha(PopupMenuButtonStyle::HoverHighlightAlpha);
    _buttonView->SetPressHighlightAlpha(PopupMenuButtonStyle::PressHighlightAlpha);
    _buttonView->SetCornerRadius(PopupMenuButtonStyle::CornerRadius);
    AddChildBack(_buttonView);

    EventReceiver::Connect(*this, *this, &PopupMenuButton::ReceiveEvent);
}

auto PopupMenuButton::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<MenuButtonViewEvent>())
    {
        auto const sender = event.As<MenuButtonViewEvent>()->GetSender();
        if (sender == _buttonView)
        {
            if (event.Is<MenuButtonViewEvent::Down>())
            {
                if (!_menu.IsEmpty())
                {
                    CreatePopup();
                    UpdatePopup();
                }
                auto downEvent = Event<>(Event<PopupMenuButtonEvent::Down>());
                co_await SendEvent(downEvent);
            }
            else if (event.Is<MenuButtonViewEvent::Up>())
            {
                auto upEvent = Event<>(Event<PopupMenuButtonEvent::Up>());
                co_await SendEvent(upEvent);
            }
            else if (event.Is<MenuButtonViewEvent::Enter>())
            {
                auto enterEvent = Event<>(Event<PopupMenuButtonEvent::Enter>());
                co_await SendEvent(enterEvent);
            }
            else if (event.Is<MenuButtonViewEvent::Leave>())
            {
                auto leaveEvent = Event<>(Event<PopupMenuButtonEvent::Leave>());
                co_await SendEvent(leaveEvent);
            }
            co_return true;
        }
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
}

// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.MenuItemButton.hpp"
#include "Futurewalker.Component.Lamp.MenuButtonView.hpp"
#include "Futurewalker.Component.Lamp.MenuViewStyle.hpp"
#include "Futurewalker.Component.Lamp.IconViewStyle.hpp"
#include "Futurewalker.Component.Lamp.TextViewStyle.hpp"

#include "Futurewalker.Application.ContainerView.hpp"
#include "Futurewalker.Application.FlexLayout.hpp"
#include "Futurewalker.Application.PointerEvent.hpp"
#include "Futurewalker.Application.FocusEvent.hpp"
#include "Futurewalker.Application.KeyEvent.hpp"
#include "Futurewalker.Application.Key.hpp"

namespace FW_LAMP_DETAIL_NS
{
auto MenuItemButton::Make() -> Shared<MenuItemButton>
{
    return View::MakeDerived<MenuItemButton>();
}

auto MenuItemButton::MakeWithContent(Shared<View> const& content) -> Shared<MenuItemButton>
{
    auto button = Make();
    button->SetContent(content);
    return button;
}

MenuItemButton::MenuItemButton(PassKey<View> key)
  : View(key)
{
}

auto MenuItemButton::Initialize() -> void
{
    _container = ContainerView::Make();

    _buttonView = MenuButtonView::MakeWithContent(_container);
    _buttonView->SetBackgroundColor(MenuItemButtonStyle::BackgroundColor);
    _buttonView->SetBackgroundAlpha(MenuItemButtonStyle::BackgroundAlpha);
    _buttonView->SetDisabledBackgroundColor(MenuItemButtonStyle::DisabledBackgroundColor);
    _buttonView->SetDisabledBackgroundAlpha(MenuItemButtonStyle::DisabledBackgroundAlpha);
    _buttonView->SetBorderColor(MenuItemButtonStyle::BorderColor);
    _buttonView->SetBorderAlpha(MenuItemButtonStyle::BorderAlpha);
    _buttonView->SetBorderWidth(MenuItemButtonStyle::BorderWidth);
    _buttonView->SetDisabledBorderColor(MenuItemButtonStyle::DisabledBorderColor);
    _buttonView->SetDisabledBorderAlpha(MenuItemButtonStyle::DisabledBorderAlpha);
    _buttonView->SetDisabledBorderWidth(MenuItemButtonStyle::DisabledBorderWidth);
    _buttonView->SetFocusedBorderColor(MenuItemButtonStyle::FocusedBorderColor);
    _buttonView->SetFocusedBorderAlpha(MenuItemButtonStyle::FocusedBorderAlpha);
    _buttonView->SetFocusedBorderWidth(MenuItemButtonStyle::FocusedBorderWidth);
    _buttonView->SetHighlightColor(MenuItemButtonStyle::HighlightColor);
    _buttonView->SetHoverHighlightAlpha(MenuItemButtonStyle::HoverHighlightAlpha);
    _buttonView->SetPressHighlightAlpha(MenuItemButtonStyle::PressHighlightAlpha);
    _buttonView->SetCornerRadius(MenuItemButtonStyle::CornerRadius);
    AddChildBack(_buttonView);

    _textColor.BindAndConnectAttribute(*this, &MenuItemButton::ReceiveAttributeEvent, MenuItemButtonStyle::TextColor);
    _textAlpha.BindAndConnectAttribute(*this, &MenuItemButton::ReceiveAttributeEvent, MenuItemButtonStyle::TextAlpha);
    _textDisabledColor.BindAndConnectAttribute(*this, &MenuItemButton::ReceiveAttributeEvent, MenuItemButtonStyle::DisabledTextColor);
    _textDisabledAlpha.BindAndConnectAttribute(*this, &MenuItemButton::ReceiveAttributeEvent, MenuItemButtonStyle::DisabledTextAlpha);
    _iconColor.BindAndConnectAttribute(*this, &MenuItemButton::ReceiveAttributeEvent, MenuItemButtonStyle::IconColor);
    _iconAlpha.BindAndConnectAttribute(*this, &MenuItemButton::ReceiveAttributeEvent, MenuItemButtonStyle::IconAlpha);
    _iconDisabledColor.BindAndConnectAttribute(*this, &MenuItemButton::ReceiveAttributeEvent, MenuItemButtonStyle::DisabledIconColor);
    _iconDisabledAlpha.BindAndConnectAttribute(*this, &MenuItemButton::ReceiveAttributeEvent, MenuItemButtonStyle::DisabledIconAlpha);

    EventReceiver::Connect(*this, *this, &MenuItemButton::ReceiveEvent);
}

auto MenuItemButton::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<MenuButtonViewEvent>())
    {
        auto const sender = event.As<MenuButtonViewEvent>()->GetSender();
        if (sender == _buttonView)
        {
            if (event.Is<MenuButtonViewEvent::Down>())
            {
                auto buttonEvent = Event<>(Event<MenuItemButtonEvent::Down>());
                co_await SendEvent(buttonEvent);
            }
            else if (event.Is<MenuButtonViewEvent::Up>())
            {
                auto buttonEvent = Event<>(Event<MenuItemButtonEvent::Up>());
                co_await SendEvent(buttonEvent);
            }
            else if (event.Is<MenuButtonViewEvent::Enter>())
            {
                auto buttonEvent = Event<>(Event<MenuItemButtonEvent::Enter>());
                co_await SendEvent(buttonEvent);
            }
            else if (event.Is<MenuButtonViewEvent::Leave>())
            {
                auto buttonEvent = Event<>(Event<MenuItemButtonEvent::Leave>());
                co_await SendEvent(buttonEvent);
            }
            co_return true;
        }
    }
    co_return false;
}

auto MenuItemButton::ReceiveAttributeEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
        // Text view styles
        AttributeNode::SetValue<&TextViewStyle::Color>(*_buttonView, _textColor.GetValueOrDefault());
        AttributeNode::SetValue<&TextViewStyle::Alpha>(*_buttonView, _textAlpha.GetValueOrDefault());
        AttributeNode::SetValue<&TextViewStyle::DisabledColor>(*_buttonView, _textDisabledColor.GetValueOrDefault());
        AttributeNode::SetValue<&TextViewStyle::DisabledAlpha>(*_buttonView, _textDisabledAlpha.GetValueOrDefault());

        // Icon view styles
        AttributeNode::SetValue<&IconViewStyle::Color>(*_buttonView, _iconColor.GetValueOrDefault());
        AttributeNode::SetValue<&IconViewStyle::Alpha>(*_buttonView, _iconAlpha.GetValueOrDefault());
        AttributeNode::SetValue<&IconViewStyle::DisabledColor>(*_buttonView, _iconDisabledColor.GetValueOrDefault());
        AttributeNode::SetValue<&IconViewStyle::DisabledAlpha>(*_buttonView, _iconDisabledAlpha.GetValueOrDefault());
    }
    co_return false;
}

auto MenuItemButton::GetContent() -> Shared<View>
{
    if (_container)
    {
        return _container->GetContent();
    }
    return {};
}

auto MenuItemButton::SetContent(Shared<View> const& content) -> void
{
    if (_container)
    {
        _container->SetContent(content);
    }
}
}

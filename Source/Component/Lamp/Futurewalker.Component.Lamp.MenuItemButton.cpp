// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.MenuItemButton.hpp"
#include "Futurewalker.Component.Lamp.ButtonRenderView.hpp"
#include "Futurewalker.Component.Lamp.MenuViewStyle.hpp"
#include "Futurewalker.Component.Lamp.IconViewStyle.hpp"
#include "Futurewalker.Component.Lamp.TextViewStyle.hpp"

#include "Futurewalker.Application.ContainerView.hpp"
#include "Futurewalker.Application.FlexLayout.hpp"

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
    SetPointerTrackingFlags(ViewPointerTrackingFlags::All);

    _container = ContainerView::Make();

    _buttonView = ButtonRenderView::MakeWithContent(_container);
    _buttonView->SetBackgroundColor(MenuItemButtonStyle::BackgroundColor);
    _buttonView->SetBackgroundAlpha(MenuItemButtonStyle::BackgroundAlpha);
    _buttonView->SetDisabledBackgroundColor(MenuItemButtonStyle::DisabledBackgroundColor);
    _buttonView->SetDisabledBackgroundAlpha(MenuItemButtonStyle::DisabledBackgroundAlpha);
    _buttonView->SetBorderColor(MenuItemButtonStyle::BorderColor);
    _buttonView->SetBorderAlpha(MenuItemButtonStyle::BorderAlpha);
    _buttonView->SetDisabledBorderColor(MenuItemButtonStyle::DisabledBorderColor);
    _buttonView->SetDisabledBorderAlpha(MenuItemButtonStyle::DisabledBorderAlpha);
    _buttonView->SetHighlightColor(MenuItemButtonStyle::HighlightColor);
    _buttonView->SetHoverHighlightAlpha(MenuItemButtonStyle::HoverHighlightAlpha);
    _buttonView->SetPressHighlightAlpha(MenuItemButtonStyle::PressHighlightAlpha);
    _buttonView->SetCornerRadius(MenuItemButtonStyle::CornerRadius);
    _buttonView->SetBorderWidth(MenuItemButtonStyle::BorderWidth);
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
    if (IsEnabledFromRoot() && event.Is<PointerEvent>())
    {
        if (event.Is<PointerEvent::Motion::Down>())
        {
            SetDown(true);
            auto buttonEvent = Event<>(Event<MenuItemButtonEvent::Down>());
            co_await SendEvent(buttonEvent);
        }
        else if (event.Is<PointerEvent::Motion::Up>())
        {
            SetDown(false);
            auto buttonEvent = Event<>(Event<MenuItemButtonEvent::Up>());
            co_await SendEvent(buttonEvent);
        }
        else if (event.Is<PointerEvent::Motion::Enter>())
        {
            SetEnter(true);
            auto buttonEvent = Event<>(Event<MenuItemButtonEvent::Enter>());
            co_await SendEvent(buttonEvent);
        }
        else if (event.Is<PointerEvent::Motion::Leave>())
        {
            SetDown(false);
            SetEnter(false);
            auto buttonEvent = Event<>(Event<MenuItemButtonEvent::Leave>());
            co_await SendEvent(buttonEvent);
        }
        else if (event.Is<PointerEvent::Motion::Move>())
        {
            if (event.As<PointerEvent::Motion::Move>()->GetButtons() != PointerButtonFlags::None)
            {
                SetDown(true);
            }
        }
        else if (event.Is<PointerEvent::Motion::Cancel>())
        {
            auto const down = IsDown();
            auto const enter = IsEnter();
            SetDown(false);
            SetEnter(false);

            if (down)
            {
                auto buttonEvent = Event<>(Event<MenuItemButtonEvent::Up>());
                co_await SendEvent(buttonEvent);
            }
            if (enter)
            {
                auto buttonEvent = Event<>(Event<MenuItemButtonEvent::Leave>());
                co_await SendEvent(buttonEvent);
            }
        }
        co_return true;
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

auto MenuItemButton::IsDown() const -> Bool
{
    if (_buttonView)
    {
        return _buttonView->IsDown();
    }
    return false;
}

auto MenuItemButton::IsEnter() const -> Bool
{
    if (_buttonView)
    {
        return _buttonView->IsEnter();
    }
    return false;
}

auto MenuItemButton::SetDown(Bool const down) -> void
{
    if (_buttonView)
    {
        _buttonView->SetDown(down);
    }
}

auto MenuItemButton::SetEnter(Bool const enter) -> void
{
    if (_buttonView)
    {
        _buttonView->SetEnter(enter);
    }
}
}

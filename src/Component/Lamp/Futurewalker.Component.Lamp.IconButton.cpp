// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.IconButton.hpp"
#include "Futurewalker.Component.Lamp.IconView.hpp"
#include "Futurewalker.Component.Lamp.ButtonView.hpp"

#include "Futurewalker.Application.PaddingView.hpp"

namespace FW_LAMP_DETAIL_NS
{
auto IconButton::Make() -> Shared<IconButton>
{
    return View::MakeDerived<IconButton>();
}

auto IconButton::MakeWithContent(Shared<View> content) -> Shared<IconButton>
{
    auto button = Make();
    button->SetContent(content);
    return button;
}

auto IconButton::MakeWithIcon(AttributeArg<Icon> const& icon) -> Shared<IconButton>
{
    auto iconView = IconView::MakeWithIcon(icon);
    return MakeWithContent(iconView);
}

auto IconButton::SetContent(Shared<View> content) -> void
{
    if (_paddingView)
    {
        _paddingView->SetContent(content);
    }
}

auto IconButton::GetContent() -> Shared<View>
{
    if (_paddingView)
    {
        return _paddingView->GetContent();
    }
    return {};
}

IconButton::IconButton(PassKey<View> key)
  : View(key)
{
}

auto IconButton::Initialize() -> void
{
    _paddingView = PaddingView::MakeWithPaddingAndContent(IconButtonStyle::Padding, _buttonView);
    _paddingView->SetPadding(IconButtonStyle::Padding);

    _buttonView = ButtonView::MakeWithContent(_paddingView);
    _buttonView->SetBackgroundColor(IconButtonStyle::BackgroundColor);
    _buttonView->SetBackgroundAlpha(IconButtonStyle::BackgroundAlpha);
    _buttonView->SetDisabledBackgroundColor(IconButtonStyle::DisabledBackgroundColor);
    _buttonView->SetDisabledBackgroundAlpha(IconButtonStyle::DisabledBackgroundAlpha);
    _buttonView->SetBorderColor(IconButtonStyle::BorderColor);
    _buttonView->SetBorderAlpha(IconButtonStyle::BorderAlpha);
    _buttonView->SetBorderWidth(IconButtonStyle::BorderWidth);
    _buttonView->SetDisabledBorderColor(IconButtonStyle::DisabledBorderColor);
    _buttonView->SetDisabledBorderAlpha(IconButtonStyle::DisabledBorderAlpha);
    _buttonView->SetDisabledBorderWidth(IconButtonStyle::DisabledBorderWidth);
    _buttonView->SetFocusedBorderColor(IconButtonStyle::FocusedBorderColor);
    _buttonView->SetFocusedBorderAlpha(IconButtonStyle::FocusedBorderAlpha);
    _buttonView->SetFocusedBorderWidth(IconButtonStyle::FocusedBorderWidth);
    _buttonView->SetHighlightColor(IconButtonStyle::HighlightColor);
    _buttonView->SetHoverHighlightAlpha(IconButtonStyle::HoverHighlightAlpha);
    _buttonView->SetPressHighlightAlpha(IconButtonStyle::PressHighlightAlpha);
    _buttonView->SetCornerRadius(IconButtonStyle::CornerRadius);

    _iconSize.BindAndConnectAttribute(*this, &IconButton::ReceiveAttributeEvent, IconButtonStyle::IconSize);
    _iconColor.BindAndConnectAttribute(*this, &IconButton::ReceiveAttributeEvent, IconButtonStyle::IconColor);
    _iconAlpha.BindAndConnectAttribute(*this, &IconButton::ReceiveAttributeEvent, IconButtonStyle::IconAlpha);
    _iconDisabledColor.BindAndConnectAttribute(*this, &IconButton::ReceiveAttributeEvent, IconButtonStyle::IconDisabledColor);
    _iconDisabledAlpha.BindAndConnectAttribute(*this, &IconButton::ReceiveAttributeEvent, IconButtonStyle::IconDisabledAlpha);

    AddChildBack(_buttonView);

    EventReceiver::Connect(*this, *this, &IconButton::ReceiveEvent);
}

auto IconButton::ReceiveEvent(Event<>& event) -> Bool
{
    if (event.Is<ButtonViewEvent>())
    {
        if (event.Is<ButtonViewEvent::Press>())
        {
            auto pressEvent = Event<>(Event<IconButtonEvent::Press>());
            SendEvent(pressEvent);
        }
        return true;
    }
    return false;
}

auto IconButton::ReceiveAttributeEvent(Event<>& event) -> Bool
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
        // TODO: Implement cached attribute values so we don't have to do this manually.
        AttributeNode::SetValue<IconViewStyle::Size>(*_buttonView, _iconSize.GetValueOr(0));
        AttributeNode::SetValue<IconViewStyle::Color>(*_buttonView, _iconColor.GetValueOrDefault());
        AttributeNode::SetValue<IconViewStyle::Alpha>(*_buttonView, _iconAlpha.GetValueOrDefault());
        AttributeNode::SetValue<IconViewStyle::DisabledColor>(*_buttonView, _iconDisabledColor.GetValueOrDefault());
        AttributeNode::SetValue<IconViewStyle::DisabledAlpha>(*_buttonView, _iconDisabledAlpha.GetValueOrDefault());
    }
    return false;
}
}

// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.TextButton.hpp"
#include "Futurewalker.Component.Lamp.TextView.hpp"
#include "Futurewalker.Component.Lamp.IconView.hpp"
#include "Futurewalker.Component.Lamp.ButtonView.hpp"

#include "Futurewalker.Application.IconView.hpp"
#include "Futurewalker.Application.FlexLayout.hpp"
#include "Futurewalker.Application.PaddingView.hpp"

namespace FW_LAMP_DETAIL_NS
{
auto TextButton::Make() -> Shared<TextButton>
{
    return View::MakeDerived<TextButton>();
}

auto TextButton::MakeWithContent(Shared<View> content) -> Shared<TextButton>
{
    auto button = Make();
    button->SetContent(content);
    return button;
}

auto TextButton::MakeWithText(AttributeArg<String> const& text) -> Shared<TextButton>
{
    auto textView = TextView::MakeWithText(text);
    return MakeWithContent(textView);
}

auto TextButton::MakeWithTextAndIcon(AttributeArg<String> const& text, AttributeArg<Icon> const& icon) -> Shared<TextButton>
{
    auto textView = TextView::MakeWithText(text);
    auto iconView = IconView::MakeWithIcon(icon);
    auto flex = FlexLayout::Make();
    flex->SetDirection(FlexLayoutDirection::Row);
    flex->SetMainAxisAlignment(FlexLayoutMainAxisAlignment::Start);
    flex->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Stretch);
    flex->SetMainAxisSize(FlexLayoutMainAxisSize::Min);
    flex->SetCrossAxisSize(FlexLayoutCrossAxisSize::Min);
    flex->AddChild(iconView);
    flex->AddChild(textView);
    FlexLayout::SetChildGrowFactor(textView, 1.0);
    FlexLayout::SetChildShrinkFactor(textView, 1.0);
    FlexLayout::SetChildMargin(iconView, EdgeInsets());
    return MakeWithContent(flex);
}

auto TextButton::SetContent(Shared<View> content) -> void
{
    if (_paddingView)
    {
        _paddingView->SetContent(content);
    }
}

auto TextButton::GetContent() -> Shared<View>
{
    if (_paddingView)
    {
        return _paddingView->GetContent();
    }
    return {};
}

TextButton::TextButton(PassKey<View> key)
  : View(key)
{
}

auto TextButton::Initialize() -> void
{
    _paddingView = PaddingView::MakeWithPaddingAndContent(TextButtonStyle::Padding, _buttonView);
    _paddingView->SetPadding(TextButtonStyle::Padding);

    _buttonView = ButtonView::MakeWithContent(_paddingView);
    _buttonView->SetBackgroundColor(TextButtonStyle::BackgroundColor);
    _buttonView->SetBackgroundAlpha(TextButtonStyle::BackgroundAlpha);
    _buttonView->SetDisabledBackgroundColor(TextButtonStyle::DisabledBackgroundColor);
    _buttonView->SetDisabledBackgroundAlpha(TextButtonStyle::DisabledBackgroundAlpha);
    _buttonView->SetBorderColor(TextButtonStyle::BorderColor);
    _buttonView->SetBorderAlpha(TextButtonStyle::BorderAlpha);
    _buttonView->SetBorderWidth(TextButtonStyle::BorderWidth);
    _buttonView->SetDisabledBorderColor(TextButtonStyle::DisabledBorderColor);
    _buttonView->SetDisabledBorderAlpha(TextButtonStyle::DisabledBorderAlpha);
    _buttonView->SetDisabledBorderWidth(TextButtonStyle::DisabledBorderWidth);
    _buttonView->SetFocusedBorderColor(TextButtonStyle::FocusedBorderColor);
    _buttonView->SetFocusedBorderAlpha(TextButtonStyle::FocusedBorderAlpha);
    _buttonView->SetFocusedBorderWidth(TextButtonStyle::FocusedBorderWidth);
    _buttonView->SetHighlightColor(TextButtonStyle::HighlightColor);
    _buttonView->SetHoverHighlightAlpha(TextButtonStyle::HoverHighlightAlpha);
    _buttonView->SetPressHighlightAlpha(TextButtonStyle::PressHighlightAlpha);
    _buttonView->SetCornerRadius(TextButtonStyle::CornerRadius);

    _textColor.BindAndConnectAttribute(*this, &TextButton::ReceiveAttributeEvent, TextButtonStyle::TextColor);
    _disabledTextColor.BindAndConnectAttribute(*this, &TextButton::ReceiveAttributeEvent, TextButtonStyle::DisabledTextColor);
    _textFontSize.BindAndConnectAttribute(*this, &TextButton::ReceiveAttributeEvent, TextButtonStyle::TextFontSize);
    _textFontWeight.BindAndConnectAttribute(*this, &TextButton::ReceiveAttributeEvent, TextButtonStyle::TextFontWeight);
    _textFontWidth.BindAndConnectAttribute(*this, &TextButton::ReceiveAttributeEvent, TextButtonStyle::TextFontWidth);
    _textFontSlant.BindAndConnectAttribute(*this, &TextButton::ReceiveAttributeEvent, TextButtonStyle::TextFontSlant);
    _textFontFamily.BindAndConnectAttribute(*this, &TextButton::ReceiveAttributeEvent, TextButtonStyle::TextFontFamily);
    _textHorizontalAlignment.BindAndConnectAttribute(*this, &TextButton::ReceiveAttributeEvent, TextButtonStyle::TextHorizontalAlignment);
    _textVerticalAlignment.BindAndConnectAttribute(*this, &TextButton::ReceiveAttributeEvent, TextButtonStyle::TextVerticalAlignment);

    AddChildBack(_buttonView);

    EventReceiver::Connect(*this, *this, &TextButton::ReceiveEvent);
}

auto TextButton::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<ButtonViewEvent>())
    {
        if (event.Is<ButtonViewEvent::Press>())
        {
            auto pressEvent = Event<>(Event<TextButtonEvent::Press>());
            co_await SendEvent(pressEvent);
        }
        co_return true;
    }
    co_return false;
}

auto TextButton::ReceiveAttributeEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
        // TODO: Implement cached attribute values so we don't have to do this manually.
        AttributeNode::SetValue<&TextViewStyle::Color>(*_buttonView, _textColor.GetValueOrDefault());
        AttributeNode::SetValue<&TextViewStyle::DisabledColor>(*_buttonView, _disabledTextColor.GetValueOrDefault());
        AttributeNode::SetValue<&TextViewStyle::FontSize>(*_buttonView, _textFontSize.GetValueOr(16));
        AttributeNode::SetValue<&TextViewStyle::FontWeight>(*_buttonView, _textFontWeight.GetValueOr(Graphics::FontWeight::Normal()));
        AttributeNode::SetValue<&TextViewStyle::FontWidth>(*_buttonView, _textFontWidth.GetValueOr(Graphics::FontWidth::Normal()));
        AttributeNode::SetValue<&TextViewStyle::FontSlant>(*_buttonView, _textFontSlant.GetValueOr(Graphics::FontSlant::Upright));
        AttributeNode::SetValue<&TextViewStyle::FontFamily>(*_buttonView, _textFontFamily.GetValueOrDefault());
        AttributeNode::SetValue<&TextViewStyle::HorizontalAlignment>(*_buttonView, _textHorizontalAlignment.GetValueOr(TextViewHorizontalAlignment::Center));
        AttributeNode::SetValue<&TextViewStyle::VerticalAlignment>(*_buttonView, _textVerticalAlignment.GetValueOr(TextViewVerticalAlignment::Middle));
    }
    co_return false;
}
}

// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.TextButton.hpp"
#include "Futurewalker.Application.TextView.hpp"
#include "Futurewalker.Application.ButtonView.hpp"

namespace FW_DETAIL_NS
{
auto TextButton::Make() -> Shared<TextButton>
{
    return View::MakeDerived<TextButton>();
}

auto TextButton::MakeWithText(AttributeArg<String> const& arg) -> Shared<TextButton>
{
    auto button = Make();
    auto text = TextView::Make();
    text->SetText(arg);
    button->SetContent(text);
    return button;
}

auto TextButton::SetContent(Shared<View> content) -> void
{
    _buttonView->SetContent(content);
}

auto TextButton::GetContent() -> Shared<View>
{
    return _buttonView->GetContent();
}

TextButton::TextButton(PassKey<View> key)
  : View(key)
{
}

auto TextButton::Initialize() -> void
{
    _buttonView = ButtonView::Make();
    _backgroundColor.BindAttribute(*this, TextButtonStyle::BackgroundColor);
    _highlightColor.BindAttribute(*this, TextButtonStyle::HighlightColor);
    _borderColor.BindAttribute(*this, TextButtonStyle::BorderColor);
    _hoverHighlightAlpha.BindAttribute(*this, TextButtonStyle::HoverHighlightAlpha);
    _pressHighlightAlpha.BindAttribute(*this, TextButtonStyle::PressHighlightAlpha);
    _cornerRadius.BindAttribute(*this, TextButtonStyle::CornerRadius);
    _borderWidth.BindAttribute(*this, TextButtonStyle::BorderWidth);
    _textColor.BindAttribute(*this, TextButtonStyle::TextColor);
    _textFontSize.BindAttribute(*this, TextButtonStyle::TextFontSize);
    _textFontWeight.BindAttribute(*this, TextButtonStyle::TextFontWeight);
    _textFontWidth.BindAttribute(*this, TextButtonStyle::TextFontWidth);
    _textFontSlant.BindAttribute(*this, TextButtonStyle::TextFontSlant);
    _textFontFamily.BindAttribute(*this, TextButtonStyle::TextFontFamily);
    _textHorizontalAlignment.BindAttribute(*this, TextButtonStyle::TextHorizontalAlignment);
    _textVerticalAlignment.BindAttribute(*this, TextButtonStyle::TextVerticalAlignment);

    EventReceiver::Connect(_backgroundColor, *this, &TextButton::ReceiveAttributeEvent);
    EventReceiver::Connect(_highlightColor, *this, &TextButton::ReceiveAttributeEvent);
    EventReceiver::Connect(_borderColor, *this, &TextButton::ReceiveAttributeEvent);
    EventReceiver::Connect(_hoverHighlightAlpha, *this, &TextButton::ReceiveAttributeEvent);
    EventReceiver::Connect(_pressHighlightAlpha, *this, &TextButton::ReceiveAttributeEvent);
    EventReceiver::Connect(_cornerRadius, *this, &TextButton::ReceiveAttributeEvent);
    EventReceiver::Connect(_borderWidth, *this, &TextButton::ReceiveAttributeEvent);
    EventReceiver::Connect(_textColor, *this, &TextButton::ReceiveAttributeEvent);
    EventReceiver::Connect(_textFontSize, *this, &TextButton::ReceiveAttributeEvent);
    EventReceiver::Connect(_textFontWeight, *this, &TextButton::ReceiveAttributeEvent);
    EventReceiver::Connect(_textFontWidth, *this, &TextButton::ReceiveAttributeEvent);
    EventReceiver::Connect(_textFontSlant, *this, &TextButton::ReceiveAttributeEvent);
    EventReceiver::Connect(_textFontFamily, *this, &TextButton::ReceiveAttributeEvent);
    EventReceiver::Connect(_textHorizontalAlignment, *this, &TextButton::ReceiveAttributeEvent);
    EventReceiver::Connect(_textVerticalAlignment, *this, &TextButton::ReceiveAttributeEvent);

    AddChildBack(_buttonView);
}

auto TextButton::ReceiveAttributeEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<AttributeEvent::ValueChanged>())
    {
        // TODO: Implement cached attribute values so we don't have to do this manually.
        AttributeNode::SetValue<&ButtonViewStyle::BackgroundColor>(*_buttonView, _backgroundColor.GetValueOrDefault());
        AttributeNode::SetValue<&ButtonViewStyle::HighlightColor>(*_buttonView, _highlightColor.GetValueOrDefault());
        AttributeNode::SetValue<&ButtonViewStyle::BorderColor>(*_buttonView, _borderColor.GetValueOrDefault());
        AttributeNode::SetValue<&ButtonViewStyle::HoverHighlightAlpha>(*_buttonView, _hoverHighlightAlpha.GetValueOrDefault());
        AttributeNode::SetValue<&ButtonViewStyle::PressHighlightAlpha>(*_buttonView, _pressHighlightAlpha.GetValueOrDefault());
        AttributeNode::SetValue<&ButtonViewStyle::BorderWidth>(*_buttonView, _borderWidth.GetValueOr(0));
        AttributeNode::SetValue<&ButtonViewStyle::CornerRadius>(*_buttonView, _cornerRadius.GetValueOrDefault());
        AttributeNode::SetValue<&TextViewStyle::Color>(*_buttonView, _textColor.GetValueOrDefault());
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

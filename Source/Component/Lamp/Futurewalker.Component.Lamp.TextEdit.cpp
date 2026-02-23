// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.TextEdit.hpp"

#include "Futurewalker.Application.TextEdit.hpp"

namespace FW_LAMP_DETAIL_NS
{
auto TextEdit::Make() -> Shared<TextEdit>
{
    return View::MakeDerived<TextEdit>();
}

auto TextEdit::GetText() const -> String
{
    if (_textEdit)
    {
        return _textEdit->GetText();
    }
    return {};
}

auto TextEdit::SetText(String const& text) -> void
{
    if (_textEdit)
    {
        _textEdit->SetText(text);
    }
}

TextEdit::TextEdit(PassKey<View> key)
  : View(key)
{
}

auto TextEdit::Initialize() -> void
{
    _textEdit = ::FW_NS::TextEdit::Make();
    AddChildBack(_textEdit);

    UpdateStyle();

    EventReceiver::Connect(*this, *this, &TextEdit::ReceiveEvent);
}

auto TextEdit::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<ViewEvent::EnabledChanged>())
    {
        UpdateStyle();
        co_return false;
    }
    else if (event.Is<TextEditEvent>())
    {
        if (event.Is<TextEditEvent::FocusChanged>())
        {
            UpdateStyle();
        }
        co_return true;
    }
    co_return false;
}

auto TextEdit::UpdateStyle() -> void
{
    if (IsEnabledFromRoot())
    {
        _textEdit->SetBackgroundColor(TextEditStyle::BackgroundColor);
        _textEdit->SetBackgroundAlpha(TextEditStyle::BackgroundAlpha);
        _textEdit->SetTextColor(TextEditStyle::TextColor);
        _textEdit->SetTextAlpha(TextEditStyle::TextAlpha);

        if (_textEdit->IsFocused())
        {
            _textEdit->SetBorderColor(TextEditStyle::FocusedBorderColor);
            _textEdit->SetBorderAlpha(TextEditStyle::FocusedBorderAlpha);
            _textEdit->SetBorderWidth(TextEditStyle::FocusedBorderWidth);
        }
        else
        {
            _textEdit->SetBorderColor(TextEditStyle::BorderColor);
            _textEdit->SetBorderAlpha(TextEditStyle::BorderAlpha);
            _textEdit->SetBorderWidth(TextEditStyle::BorderWidth);
        }
    }
    else
    {
        _textEdit->SetBackgroundColor(TextEditStyle::DisabledBackgroundColor);
        _textEdit->SetBackgroundAlpha(TextEditStyle::DisabledBackgroundAlpha);
        _textEdit->SetTextColor(TextEditStyle::DisabledTextColor);
        _textEdit->SetTextAlpha(TextEditStyle::DisabledTextAlpha);
        _textEdit->SetBorderColor(TextEditStyle::DisabledBorderColor);
        _textEdit->SetBorderAlpha(TextEditStyle::DisabledBorderAlpha);
        _textEdit->SetBorderWidth(TextEditStyle::DisabledBorderWidth);
    }
    _textEdit->SetCornerRadius(TextEditStyle::BorderRadius);
    _textEdit->SetPadding(TextEditStyle::Padding);
    _textEdit->SetFontSize(TextEditStyle::FontSize);
    _textEdit->SetFontWeight(TextEditStyle::FontWeight);
    _textEdit->SetFontWidth(TextEditStyle::FontWidth);
    _textEdit->SetFontSlant(TextEditStyle::FontSlant);
    _textEdit->SetFontFamily(StaticAttributeRef(TextEditStyle::FontFamily)); // FIXME: MSVC Bug
}
}

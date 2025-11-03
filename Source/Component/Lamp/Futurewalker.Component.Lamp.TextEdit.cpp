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
    _textEdit->SetBackgroundColor(TextEditStyle::BackgroundColor);
    _textEdit->SetBackgroundAlpha(TextEditStyle::BackgroundAlpha);
    _textEdit->SetDisabledBackgroundColor(TextEditStyle::DisabledBackgroundColor);
    _textEdit->SetDisabledBackgroundAlpha(TextEditStyle::DisabledBackgroundAlpha);
    _textEdit->SetTextColor(TextEditStyle::TextColor);
    _textEdit->SetTextAlpha(TextEditStyle::TextAlpha);
    _textEdit->SetDisabledTextColor(TextEditStyle::DisabledTextColor);
    _textEdit->SetDisabledTextAlpha(TextEditStyle::DisabledTextAlpha);
    _textEdit->SetBorderColor(TextEditStyle::BorderColor);
    _textEdit->SetBorderAlpha(TextEditStyle::BorderAlpha);
    _textEdit->SetDisabledBorderColor(TextEditStyle::DisabledBorderColor);
    _textEdit->SetDisabledBorderAlpha(TextEditStyle::DisabledBorderAlpha);
    _textEdit->SetBorderWidth(TextEditStyle::BorderWidth);
    _textEdit->SetCornerRadius(TextEditStyle::BorderRadius);
    _textEdit->SetFontSize(TextEditStyle::FontSize);
    _textEdit->SetFontWeight(TextEditStyle::FontWeight);
    _textEdit->SetFontWidth(TextEditStyle::FontWidth);
    _textEdit->SetFontSlant(TextEditStyle::FontSlant);
    _textEdit->SetFontFamily(TextEditStyle::FontFamily);
    AddChildBack(_textEdit);
}
}

// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.TextView.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
auto TextView::Make() -> Shared<TextView>
{
    return View::MakeDerived<TextView>();
}

auto TextView::MakeWithText(AttributeArg<String> const& text) -> Shared<TextView>
{
    auto textView = TextView::Make();
    textView->SetText(text);
    return textView;
}

auto TextView::GetText() const -> String
{
    if (_textView)
    {
        return _textView->GetText();
    }
    return {};
}

auto TextView::SetText(AttributeArg<String> const& text) -> void
{
    if (_textView)
    {
        _textView->SetText(text);
    }
}

TextView::TextView(PassKey<View> key)
  : View(key)
{
}

auto TextView::Initialize() -> void
{
    _textView = ::FW_NS::TextView::Make();
    _textView->SetColor(TextViewStyle::Color);
    _textView->SetAlpha(TextViewStyle::Alpha);
    _textView->SetDisabledColor(TextViewStyle::DisabledColor);
    _textView->SetDisabledAlpha(TextViewStyle::DisabledAlpha);
    _textView->SetFontSize(TextViewStyle::FontSize);
    _textView->SetFontWeight(TextViewStyle::FontWeight);
    _textView->SetFontWidth(TextViewStyle::FontWidth);
    _textView->SetFontSlant(TextViewStyle::FontSlant);
    _textView->SetFontFamily(TextViewStyle::FontFamily);
    _textView->SetHorizontalAlignment(TextViewStyle::HorizontalAlignment);
    _textView->SetVerticalAlignment(TextViewStyle::VerticalAlignment);
    AddChildBack(_textView);
}
}
}

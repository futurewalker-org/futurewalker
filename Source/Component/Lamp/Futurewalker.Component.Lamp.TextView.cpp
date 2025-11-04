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
    AddChildBack(_textView);

    UpdateStyle();

    EventReceiver::Connect(*this, *this, &TextView::ReceiveEvent);
}

auto TextView::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<ViewEvent::EnabledChanged>())
    {
        UpdateStyle();
    }
    co_return false;
}

auto TextView::UpdateStyle() -> void
{
    if (IsEnabledFromRoot())
    {
        _textView->SetColor(TextViewStyle::Color);
        _textView->SetAlpha(TextViewStyle::Alpha);
    }
    else
    {
        _textView->SetColor(TextViewStyle::DisabledColor);
        _textView->SetAlpha(TextViewStyle::DisabledAlpha);
    }
    _textView->SetFontSize(TextViewStyle::FontSize);
    _textView->SetFontWeight(TextViewStyle::FontWeight);
    _textView->SetFontWidth(TextViewStyle::FontWidth);
    _textView->SetFontSlant(TextViewStyle::FontSlant);
    _textView->SetFontFamily(TextViewStyle::FontFamily);
    _textView->SetHorizontalAlignment(TextViewStyle::HorizontalAlignment);
    _textView->SetVerticalAlignment(TextViewStyle::VerticalAlignment);
}
}
}

// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformInputMethodEditableWin.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodTextStoreWin.hpp"
#include "Futurewalker.Application.PlatformInputEvent.hpp"

#include "Futurewalker.Core.StringFunction.hpp"

#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
auto PlatformInputMethodEditableWin::Make(Delegate const& delegate) -> Shared<PlatformInputMethodEditableWin>
{
    return PlatformInputMethodEditable::MakeDerived<PlatformInputMethodEditableWin>(delegate);
}

PlatformInputMethodEditableWin::PlatformInputMethodEditableWin(PassKey<PlatformInputMethodEditable> key, Delegate const& delegate)
  : PlatformInputMethodEditable(key, delegate)
  , _state {{
      .beforeInsertText = [&](String const& text) { return InternalBeforeInsertText(text); },
      .beforeDeleteSurroundingText = [&](CodePoint before, CodePoint after) { return InternalBeforeDeleteSurroundingText(before, after); },
      .onTextChange = [&](CodeUnit u16OldBegin, CodeUnit u16OldEnd, CodeUnit u16NewEnd) { return InternalOnTextChange(u16OldBegin, u16OldEnd, u16NewEnd); },
      .onSelectionChange = [&]() { return InternalOnSelectionChange(); },
    }}
{
}

auto PlatformInputMethodEditableWin::SetTextStore(Shared<PlatformInputMethodTextStoreWin> textStore) -> void
{
    _textStore = textStore;
}

auto PlatformInputMethodEditableWin::GetText() const -> String
{
    return _state.GetText();
}

auto PlatformInputMethodEditableWin::GetText(Range<CodePoint> range) const -> String
{
    return _state.GetText(range);
}

auto PlatformInputMethodEditableWin::GetTextRange() const -> Range<CodePoint>
{
    return _state.GetTextRange();
}

auto PlatformInputMethodEditableWin::SetText(String const& text) -> void
{
    _state.SetText(text);
}

auto PlatformInputMethodEditableWin::GetSelectedRange() const -> Range<CodePoint>
{
    return _state.GetSelectedRange();
}

auto PlatformInputMethodEditableWin::SetSelectedRange(Range<CodePoint> const& range) -> void
{
    _state.SetSelectedRange(range);
}

auto PlatformInputMethodEditableWin::GetComposingRange() const -> Range<CodePoint>
{
    return _state.GetSelectedRange();
}

auto PlatformInputMethodEditableWin::SetComposingRange(Range<CodePoint> const& range) -> void
{
    _state.SetSelectedRange(range);
}

auto PlatformInputMethodEditableWin::GetLayoutRect() const -> Rect<Dp>
{
    return _layoutRect;
}

auto PlatformInputMethodEditableWin::SetLayoutRect(Rect<Dp> const& layoutRect) -> void
{
    if (_layoutRect != layoutRect)
    {
        _layoutRect = layoutRect;

        if (auto textStore = GetTextStore())
        {
            textStore->NotifyLayoutChange();
        }
    }
}

auto PlatformInputMethodEditableWin::GetLayoutInfo() const -> Graphics::TextLayoutInfo
{
    return _layoutInfo;
}

auto PlatformInputMethodEditableWin::SetLayoutInfo(Graphics::TextLayoutInfo const& layoutInfo) -> void
{
    _layoutInfo = layoutInfo;

    if (auto textStore = GetTextStore())
    {
        textStore->NotifyLayoutChange();
    }
}

auto PlatformInputMethodEditableWin::InsertText(String const& text, CodePoint caretPosition) -> void
{
    _state.InsertText(text, caretPosition, false);
}

auto PlatformInputMethodEditableWin::DeleteSurroundingText(CodePoint before, CodePoint after) -> void
{
    _state.DeleteSurroundingText(before, after, false);
}

auto PlatformInputMethodEditableWin::GetU16Text() const -> std::u16string_view
{
    return _state.GetU16Text();
}

auto PlatformInputMethodEditableWin::GetU16Text(Range<CodeUnit> range) const -> std::u16string_view
{
    return _state.GetU16Text(range);
}

auto PlatformInputMethodEditableWin::GetU16TextRange() const -> Range<CodeUnit>
{
    return _state.GetU16TextRange();
}

auto PlatformInputMethodEditableWin::GetU16SelectedRange() const -> Range<CodeUnit>
{
    return _state.GetU16SelectedRange();
}

auto PlatformInputMethodEditableWin::SetU16SelectedRange(Range<CodeUnit> range, Bool anticipated) -> void
{
    _state.SetU16SelectedRange(range, anticipated);
}

auto PlatformInputMethodEditableWin::GetU16ComposingRange() const -> Range<CodeUnit>
{
    return _state.GetU16ComposingRange();
}

auto PlatformInputMethodEditableWin::SetU16ComposingRange(Range<CodeUnit> range) -> void
{
    _state.SetU16ComposingRange(range);
}

auto PlatformInputMethodEditableWin::InsertU16Text(std::u16string_view const text, CodePoint caretPosition, Bool anticipated) -> void
{
    _state.InsertU16Text(text, caretPosition, anticipated);
}

auto PlatformInputMethodEditableWin::GetRangeFromU16Range(Range<CodeUnit> range) const -> Range<CodePoint>
{
    return _state.GetRangeFromU16Range(range);
}

auto PlatformInputMethodEditableWin::InternalBeforeInsertText(String const& text) -> Bool
{
    auto parameter = Event<PlatformInputEvent::InsertText>();
    parameter->SetText(text);
    auto event = Event<>(parameter);
    if (SendInputEventDetached(event))
    {
        if (event.Is<PlatformInputEvent::InsertText>())
        {
            parameter = event.As<PlatformInputEvent::InsertText>();
            return parameter->GetCancel();
        }
        return false;
    }
    return true;
}

auto PlatformInputMethodEditableWin::InternalBeforeDeleteSurroundingText(CodePoint before, CodePoint after) -> Bool
{
    auto parameter = Event<PlatformInputEvent::DeleteSurroundingText>();
    parameter->SetBefore(before);
    parameter->SetAfter(after);
    auto event = Event<>(parameter);
    if (SendInputEventDetached(event))
    {
        if (event.Is<PlatformInputEvent::DeleteSurroundingText>())
        {
            parameter = event.As<PlatformInputEvent::DeleteSurroundingText>();
            return parameter->GetCancel();
        }
        return false;
    }
    return true;
}

auto PlatformInputMethodEditableWin::InternalOnTextChange(CodeUnit u16OldBegin, CodeUnit u16OldEnd, CodeUnit u16NewEnd) -> void
{
    if (auto textStore = GetTextStore())
    {
        textStore->NotifyTextChange(u16OldBegin, u16OldEnd, u16NewEnd);
    }
}

auto PlatformInputMethodEditableWin::InternalOnSelectionChange() -> void
{
    if (auto textStore = GetTextStore())
    {
        textStore->NotifySelectionChange();
    }
}

auto PlatformInputMethodEditableWin::GetTextStore() -> Shared<PlatformInputMethodTextStoreWin>
{
    return _textStore.Lock();
}
}

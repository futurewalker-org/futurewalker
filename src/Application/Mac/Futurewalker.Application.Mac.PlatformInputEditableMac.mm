// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Mac.PlatformInputEditableMac.hpp"

#include "Futurewalker.Application.PlatformInputEvent.hpp"

namespace FW_DETAIL_NS
{
auto PlatformInputEditableMac::Make(Delegate const& delegate) -> Shared<PlatformInputEditableMac>
{
    return PlatformInputEditable::MakeDerived<PlatformInputEditableMac>(delegate);
}

PlatformInputEditableMac::PlatformInputEditableMac(PassKey<PlatformInputEditable> key, Delegate const& delegate)
  : PlatformInputEditable(key, delegate)
  , _inputState {{
      .beforeInsertText = [&](String const& text, Bool cancellable) { return InternalBeforeInsertText(text, cancellable); },
      .insertText = [&](String const& text) { InternalInsertText(text); },
      .beforeInsertLineBreak = [&](Bool cancellable) { return InternalBeforeInsertLineBreak(cancellable); },
      .insertLineBreak = [&]() { InternalInsertLineBreak(); },
      .beforeDeleteSurroundingText = [&](CodePoint before, CodePoint after, Bool cancellable) { return InternalBeforeDeleteSurroundingText(before, after, cancellable); },
      .deleteSurroundingText = [&](CodePoint before, CodePoint after) { InternalDeleteSurroundingText(before, after); },
      .onTextChange = [&](CodeUnit u16OldBegin, CodeUnit u16OldEnd, CodeUnit u16NewEnd) { return InternalOnTextChange(u16OldBegin, u16OldEnd, u16NewEnd); },
      .onSelectionChange = [&]() { return InternalOnSelectionChange(); },
    }}
{
}

auto PlatformInputEditableMac::SetTextInputContext(NSTextInputContext* textInputContext) -> void
{
    _textInputContext = textInputContext;
}

auto PlatformInputEditableMac::GetText() const -> Text
{
    return _inputState.GetText();
}

auto PlatformInputEditableMac::SetText(Text const& text) -> void
{
    _inputState.SetText(text);
}

auto PlatformInputEditableMac::GetString(Range<CodePoint> const& range) const -> String
{
    return _inputState.GetString(range);
}

auto PlatformInputEditableMac::GetStringRange() const -> Range<CodePoint>
{
    return _inputState.GetStringRange();
}

auto PlatformInputEditableMac::GetSelectionDirection() const -> TextSelectionDirection
{
    return _inputState.GetSelectionDirection();
}

auto PlatformInputEditableMac::GetSelectionRange() const -> Range<CodePoint>
{
    return _inputState.GetSelectionRange();
}

auto PlatformInputEditableMac::SetSelectionRange(Range<CodePoint> const& range, TextSelectionDirection direction) -> void
{
    _inputState.SetSelectionRange(range, direction);
}

auto PlatformInputEditableMac::GetComposingRange() const -> Range<CodePoint>
{
    return _inputState.GetComposingRange();
}

auto PlatformInputEditableMac::SetComposingRange(Range<CodePoint> const& range) -> void
{
    _inputState.SetComposingRange(range);
}

auto PlatformInputEditableMac::GetLayoutRect() const -> Rect<Dp>
{
    return _layoutRect;
}

auto PlatformInputEditableMac::SetLayoutRect(Rect<Dp> const& rect) -> void
{
    _layoutRect = rect;
}

auto PlatformInputEditableMac::GetLayoutInfo() const -> Graphics::TextLayoutInfo
{
    return _layoutInfo;
}

auto PlatformInputEditableMac::SetLayoutInfo(Graphics::TextLayoutInfo const& layoutInfo) -> void
{
    _layoutInfo = layoutInfo;
}

auto PlatformInputEditableMac::InsertText(String const& text, CodePoint caretPosition) -> void
{
    _inputState.InsertText(text, caretPosition, false, true);
}

auto PlatformInputEditableMac::InsertLineBreak() -> void
{
    _inputState.InsertLineBreak(false, true);
}

auto PlatformInputEditableMac::DeleteSurroundingText(CodePoint before, CodePoint after) -> void
{
    _inputState.DeleteSurroundingText(before, after, false, true);
}

auto PlatformInputEditableMac::GetU16Text() const -> U16String
{
    return _inputState.GetU16String();
}

auto PlatformInputEditableMac::GetU16Text(Range<CodeUnit> range) const -> U16String
{
    return _inputState.GetU16String(range);
}

auto PlatformInputEditableMac::GetU16TextRange() const -> Range<CodeUnit>
{
    return _inputState.GetU16StringRange();
}

auto PlatformInputEditableMac::GetU16SelectionRange() const -> Range<CodeUnit>
{
    return _inputState.GetU16SelectionRange();
}

auto PlatformInputEditableMac::SetU16SelectionRange(Range<CodeUnit> range, TextSelectionDirection direction, Bool anticipated) -> void
{
    _inputState.SetU16SelectionRange(range, direction, anticipated);
}

auto PlatformInputEditableMac::GetU16ComposingRange() const -> Range<CodeUnit>
{
    return _inputState.GetU16ComposingRange();
}

auto PlatformInputEditableMac::SetU16ComposingRange(Range<CodeUnit> range) -> void
{
    _inputState.SetU16ComposingRange(range);
}

auto PlatformInputEditableMac::GetRangeFromU16Range(Range<CodeUnit> range) const -> Range<CodePoint>
{
    return _inputState.GetRangeFromU16Range(range);
}

auto PlatformInputEditableMac::GetU16RangeFromRange(Range<CodePoint> range) const -> Range<CodeUnit>
{
    return _inputState.GetU16RangeFromRange(range);
}

auto PlatformInputEditableMac::InsertU16Text(U16StringView const text, CodePoint caretPosition, Bool anticipated, Bool cancellable) -> void
{
    _inputState.InsertU16Text(text, caretPosition, anticipated, cancellable);
}

auto PlatformInputEditableMac::InternalBeforeInsertText(String const& text, Bool cancellable) -> Bool
{
    if (cancellable)
    {
        auto parameter = Event<PlatformInputEvent::BeforeInsertText>();
        parameter->SetText(text);
        auto event = Event<>(parameter);
        if (SendInputEventDetached(event))
        {
            if (event.Is<PlatformInputEvent::BeforeInsertText>())
            {
                parameter = event.As<PlatformInputEvent::BeforeInsertText>();
                return !parameter->GetCancel();
            }
            return false;
        }
    }
    else
    {
        auto parameter = Event<PlatformInputEvent::BeforeInsertCompositionText>();
        parameter->SetText(text);
        auto event = Event<>(std::move(parameter));
        SendInputEventDetached(event);
    }
    return true;
}

auto PlatformInputEditableMac::InternalInsertText(String const& text) -> void
{
    auto parameter = Event<PlatformInputEvent::InsertText>();
    parameter->SetText(text);
    auto event = Event<>(std::move(parameter));
    SendInputEventDetached(event);
}

auto PlatformInputEditableMac::InternalBeforeInsertLineBreak(Bool cancellable) -> Bool
{
    auto parameter = Event<PlatformInputEvent::BeforeInsertLineBreak>();
    auto event = Event<>(parameter);
    if (SendInputEventDetached(event))
    {
        if (cancellable)
        {
            if (event.Is<PlatformInputEvent::BeforeInsertLineBreak>())
            {
                parameter = event.As<PlatformInputEvent::BeforeInsertLineBreak>();
                return !parameter->GetCancel();
            }
            return false;
        }
    }
    return true;
}

auto PlatformInputEditableMac::InternalInsertLineBreak() -> void
{
    auto parameter = Event<PlatformInputEvent::InsertLineBreak>();
    auto event = Event<>(std::move(parameter));
    SendInputEventDetached(event);
}

auto PlatformInputEditableMac::InternalBeforeDeleteSurroundingText(CodePoint before, CodePoint after, Bool cancellable) -> Bool
{
    auto parameter = Event<PlatformInputEvent::BeforeDeleteSurroundingText>();
    parameter->SetBefore(before);
    parameter->SetAfter(after);
    auto event = Event<>(parameter);
    if (SendInputEventDetached(event))
    {
        if (cancellable)
        {
            if (event.Is<PlatformInputEvent::BeforeDeleteSurroundingText>())
            {
                parameter = event.As<PlatformInputEvent::BeforeDeleteSurroundingText>();
                return !parameter->GetCancel();
            }
            return false;
        }
    }
    return true;
}

auto PlatformInputEditableMac::InternalDeleteSurroundingText(CodePoint before, CodePoint after) -> void
{
    auto parameter = Event<PlatformInputEvent::DeleteSurroundingText>();
    parameter->SetBefore(before);
    parameter->SetAfter(after);
    auto event = Event<>(std::move(parameter));
    SendInputEventDetached(event);
}

auto PlatformInputEditableMac::InternalOnTextChange(CodeUnit u16OldBegin, CodeUnit u16OldEnd, CodeUnit u16NewEnd) -> void
{
    (void)u16OldBegin;
    (void)u16OldEnd;
    (void)u16NewEnd;
}

auto PlatformInputEditableMac::InternalOnSelectionChange() -> void
{
    if (@available(macOS 15.4, *))
    {
        [_textInputContext textInputClientDidUpdateSelection];
    }
}
}

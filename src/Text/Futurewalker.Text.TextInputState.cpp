// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Text.TextInputState.hpp"

namespace FW_DETAIL_NS
{
TextInputState::TextInputState(Delegate const& delegate)
  : _delegate {delegate}
{
}

auto TextInputState::GetText() const -> Text
{
    return _text;
}

auto TextInputState::SetText(Text const& text) -> void
{
    auto const textRange = GetU16StringRange();
    auto const selectionRange = GetSelectionRange();

    _text = text;
    _selectionRange = {_text.GetCodePointCount(), _text.GetCodePointCount()}; // FIXME: Should we preserve current selection??
    auto const newTextRange = GetU16StringRange();
    auto const newSelectionRange = GetSelectionRange();

    OnTextChange(false, textRange.begin, textRange.end, newTextRange.end);

    if (selectionRange != newSelectionRange)
    {
        OnSelectionChange(false);
    }
}

auto TextInputState::GetString() const -> String
{
    return _text.GetString();
}

auto TextInputState::GetString(Range<CodePoint> const& range) const -> String
{
    return _text.GetString(range);
}

auto TextInputState::GetStringRange() const -> Range<CodePoint>
{
    return {0, _text.GetCodePointCount()};
}

auto TextInputState::GetSelectionRange() const -> Range<CodePoint>
{
    return _selectionRange;
}

auto TextInputState::GetSelectionDirection() const -> TextSelectionDirection
{
    return _selectionDirection;
}

auto TextInputState::SetSelectionRange(Range<CodePoint> const& range, TextSelectionDirection const direction) -> Bool
{
    auto const textRange = GetStringRange();
    auto const newBegin = Range<CodePoint>::Clamp(range.begin, textRange);
    auto const newEnd = Range<CodePoint>::Clamp(range.end, textRange);
    auto const newSelectionRange = Range<CodePoint>::Normalize(Range<CodePoint>(newBegin, newEnd));
    auto const newSelectionDirection = direction == TextSelectionDirection::None ? TextSelectionDirection::Forward : direction;

    auto changed = False;
    if (_selectionRange != newSelectionRange)
    {
        _selectionRange = newSelectionRange;
        changed = true;
    }

    if (_selectionDirection != newSelectionDirection)
    {
        _selectionDirection = newSelectionDirection;
        changed = true;
    }

    if (changed)
    {
        OnSelectionChange(false);
    }

    if (range != newSelectionRange)
    {
        return false;
    }
    return true;
}

auto TextInputState::GetComposingRange() const -> Range<CodePoint>
{
    return _composingRange;
}

auto TextInputState::SetComposingRange(Range<CodePoint> const& range) -> Bool
{
    auto const textRange = GetStringRange();
    auto const newBegin = Range<CodePoint>::Clamp(range.begin, textRange);
    auto const newEnd = Range<CodePoint>::Clamp(range.end, textRange);
    auto const newComposingRange = Range<CodePoint>::Normalize(Range<CodePoint>(newBegin, newEnd));

    if (_composingRange != newComposingRange)
    {
        _composingRange = newComposingRange;
    }

    if (range != newComposingRange)
    {
        return false;
    }
    return true;
}

auto TextInputState::InsertText(String const& text, CodePoint caretPosition, Bool anticipated, Bool cancellable) -> void
{
    InsertTextCore(caretPosition, anticipated, cancellable, Text(text));
}

auto TextInputState::InsertLineBreak(Bool anticipated, Bool cancellable) -> void
{
    if (BeforeInsertLineBreak(cancellable))
    {
        auto const text = Text(String(u8"\n"));
        auto const selectionRange = GetSelectionRange();
        auto const u16SelectionRange = GetU16SelectionRange();
        _text.Replace(selectionRange, text);

        auto const caretPosition = 1;
        auto const newPosition = selectionRange.begin + ((caretPosition > 0) ? text.GetCodePointCount() + caretPosition - 1 : caretPosition);
        _selectionRange = {newPosition, newPosition};

        OnTextChange(anticipated, u16SelectionRange.begin, u16SelectionRange.end, u16SelectionRange.begin + text.GetU16CodeUnitCount());
        OnSelectionChange(anticipated);

        InsertLineBreak();
    }
    else
    {
        auto const text = Text(String(u8"\n"));
        auto const u16SelectionRange = GetU16SelectionRange();
        OnTextChange(anticipated, u16SelectionRange.begin, u16SelectionRange.begin + text.GetU16CodeUnitCount(), u16SelectionRange.end);
        OnSelectionChange(anticipated);
    }
}

auto TextInputState::DeleteSurroundingText(CodePoint before, CodePoint after, Bool anticipated, Bool cancellable) -> void
{
    if (BeforeDeleteSurroundingText(before, after, cancellable))
    {
        auto const textRange = GetStringRange();
        auto const selectionRange = GetSelectionRange();

        auto const postBegin = selectionRange.end;
        auto const postEnd = Range<CodePoint>::Clamp(selectionRange.end + after, textRange);
        auto const postEnd16 = FindU16IndexFromCodePoint(postEnd);
        _text.Replace({postBegin, postEnd}, Text());

        auto const preBegin = Range<CodePoint>::Clamp(selectionRange.begin - before, textRange);
        auto const preBegin16 = FindU16IndexFromCodePoint(preBegin);
        auto const preEnd = selectionRange.begin;
        _text.Replace({preBegin, preEnd}, Text());

        auto const offset = preEnd - preBegin;
        auto const newBegin = selectionRange.begin - offset;
        auto const newEnd = selectionRange.end - offset;
        if (_selectionRange.GetLength() > 0)
        {
            _selectionRange = {newBegin, newEnd};
        }
        else
        {
            _selectionRange = {newEnd, newBegin};
        }

        OnTextChange(anticipated, preBegin16, postEnd16, FindU16IndexFromCodePoint(newEnd));

        if (before != 0)
        {
            OnSelectionChange(anticipated);
        }

        DeleteSurroundingText(before, after);
    }
    else
    {
        auto const textRange = GetStringRange();
        auto const selectionRange = GetSelectionRange();
        auto const oldBegin = Range<CodePoint>::Clamp(selectionRange.begin - before, textRange);
        auto const oldEnd = Range<CodePoint>::Clamp(oldBegin + selectionRange.GetLength(), textRange);
        auto const oldRange = FindU16RangeFromCodePointRange({oldBegin, oldEnd});
        auto const newRange = GetU16SelectionRange();
        OnTextChange(anticipated, oldRange.begin, oldRange.end, newRange.end);

        if (before != 0)
        {
            OnSelectionChange(anticipated);
        }
    }
}

auto TextInputState::GetU16String() const -> U16String
{
    return _text.GetU16String();
}

auto TextInputState::GetU16String(Range<CodeUnit> range) const -> U16String
{
    auto const b = _text.GetCodePointIndexByU16Index(range.begin);
    auto const e = _text.GetCodePointIndexByU16Index(range.end);
    return _text.GetU16String({b, e});
}

auto TextInputState::GetU16StringRange() const -> Range<CodeUnit>
{
    return {0, _text.GetU16CodeUnitCount()};
}

auto TextInputState::GetU16SelectionRange() const -> Range<CodeUnit>
{
    auto const range = GetSelectionRange();
    auto const b = _text.GetU16IndexByCodePointIndex(range.begin);
    auto const e = _text.GetU16IndexByCodePointIndex(range.end);
    return {b, e};
}

auto TextInputState::SetU16SelectionRange(Range<CodeUnit> const& range, TextSelectionDirection const direction, Bool anticipated) -> void
{
    auto const b = FindCodePointFromU16Index(range.begin);
    auto const e = FindCodePointFromU16Index(range.end);
    auto const newSelectedRange = Range<CodePoint>::Normalize(Range<CodePoint>(b, e));
    auto const newSelectionDirection = direction == TextSelectionDirection::None ? TextSelectionDirection::Forward : direction;

    auto changed = False;

    if (_selectionRange != newSelectedRange)
    {
        _selectionRange = newSelectedRange;
        changed = true;
    }

    if (_selectionDirection != newSelectionDirection)
    {
        _selectionDirection = newSelectionDirection;
        changed = true;
    }

    if (changed)
    {
        OnSelectionChange(anticipated);
    }
}

auto TextInputState::GetU16ComposingRange() const -> Range<CodeUnit>
{
    auto const range = GetComposingRange();
    auto const b = _text.GetU16IndexByCodePointIndex(range.begin);
    auto const e = _text.GetU16IndexByCodePointIndex(range.end);
    return {b, e};
}

auto TextInputState::SetU16ComposingRange(Range<CodeUnit> const& range) -> void
{
    auto const b = FindCodePointFromU16Index(range.begin);
    auto const e = FindCodePointFromU16Index(range.end);
    auto const newComposingRange = Range<CodePoint>::Normalize(Range<CodePoint>(b, e));

    if (_composingRange != newComposingRange)
    {
        _composingRange = newComposingRange;
    }
}

auto TextInputState::InsertU16Text(U16StringView text, CodePoint caretPosition, Bool anticipated, Bool cancellable) -> void
{
    InsertTextCore(caretPosition, anticipated, cancellable, Text(text));
}

auto TextInputState::GetRangeFromU16Range(Range<CodeUnit> range) const -> Range<CodePoint>
{
    auto const begin = FindCodePointFromU16Index(range.begin);
    auto const end = FindCodePointFromU16Index(range.end);
    return {begin, end};
}

auto TextInputState::GetRangeFromU8Range(Range<CodeUnit> range) const -> Range<CodePoint>
{
    auto const begin = FindCodePointFromU8Index(range.begin);
    auto const end = FindCodePointFromU8Index(range.end);
    return {begin, end};
}

auto TextInputState::GetU16RangeFromRange(Range<CodePoint> range) const -> Range<CodeUnit>
{
    return FindU16RangeFromCodePointRange(range);
}

auto TextInputState::GetU8RangeFromRange(Range<CodePoint> range) const -> Range<CodeUnit>
{
    return FindU8RangeFromCodePointRange(range);
}

auto TextInputState::FindCodePointFromU8Index(CodeUnit index) const -> CodePoint
{
    return _text.GetCodePointIndexByU8Index(index);
}

auto TextInputState::FindCodePointFromU16Index(CodeUnit index) const -> CodePoint
{
    return _text.GetCodePointIndexByU16Index(index);
}

auto TextInputState::FindU8IndexFromCodePoint(CodePoint codePoint) const -> CodeUnit
{
    return _text.GetU8IndexByCodePointIndex(codePoint);
}

auto TextInputState::FindU8RangeFromCodePointRange(Range<CodePoint> range) const -> Range<CodeUnit>
{
    auto const b = _text.GetU8IndexByCodePointIndex(range.begin);
    auto const e = _text.GetU8IndexByCodePointIndex(range.end);
    return Range<CodeUnit>(b, e);
}

auto TextInputState::FindU16IndexFromCodePoint(CodePoint codePoint) const -> CodeUnit
{
    return _text.GetU16IndexByCodePointIndex(codePoint);
}

auto TextInputState::FindU16RangeFromCodePointRange(Range<CodePoint> range) const -> Range<CodeUnit>
{
    auto const b = _text.GetU16IndexByCodePointIndex(range.begin);
    auto const e = _text.GetU16IndexByCodePointIndex(range.end);
    return Range<CodeUnit>(b, e);
}

auto TextInputState::InsertTextCore(CodePoint caretPosition, Bool anticipated, Bool cancellable, Text const& text) -> void
{
    if (BeforeInsertText(text.GetString(), cancellable))
    {
        auto const oldSelection = GetSelectionRange();
        auto const oldU16Selection = GetU16SelectionRange();
        _text.Replace(oldSelection, text);

        auto const newPosition = oldSelection.begin + ((caretPosition > 0) ? text.GetCodePointCount() + caretPosition - 1 : caretPosition);
        _selectionRange = {newPosition, newPosition};

        OnTextChange(anticipated, oldU16Selection.begin, oldU16Selection.end, oldU16Selection.begin + text.GetU16CodeUnitCount());

        if (_selectionRange != oldSelection)
        {
            OnSelectionChange(anticipated);
        }

        InsertText(text.GetString());
    }
    else
    {
        auto const u16SelectedRange = GetU16SelectionRange();
        OnTextChange(anticipated, u16SelectedRange.begin, u16SelectedRange.begin + text.GetU16CodeUnitCount(), u16SelectedRange.end);
        OnSelectionChange(anticipated);
    }
}

auto TextInputState::BeforeInsertText(String const& text, Bool cancellable) -> Bool
{
    if (_delegate.beforeInsertText)
    {
        return _delegate.beforeInsertText(text, cancellable);
    }
    return true;
}

auto TextInputState::InsertText(String const& text) -> void
{
    if (_delegate.insertText)
    {
        _delegate.insertText(text);
    }
}

auto TextInputState::BeforeInsertLineBreak(Bool cancellable) -> Bool
{
    if (_delegate.beforeInsertLineBreak)
    {
        return _delegate.beforeInsertLineBreak(cancellable);
    }
    return true;
}

auto TextInputState::InsertLineBreak() -> void
{
    if (_delegate.insertLineBreak)
    {
        _delegate.insertLineBreak();
    }
}

auto TextInputState::BeforeDeleteSurroundingText(CodePoint before, CodePoint after, Bool cancellable) -> Bool
{
    if (_delegate.beforeDeleteSurroundingText)
    {
        return _delegate.beforeDeleteSurroundingText(before, after, cancellable);
    }
    return true;
}

auto TextInputState::DeleteSurroundingText(CodePoint before, CodePoint after) -> void
{
    if (_delegate.deleteSurroundingText)
    {
        _delegate.deleteSurroundingText(before, after);
    }
}

auto TextInputState::OnTextChange(Bool anticipated, CodeUnit u16Begin, CodeUnit oldU16End, CodeUnit newU16End) -> void
{
    if (_delegate.onTextChange)
    {
        _delegate.onTextChange(anticipated, u16Begin, oldU16End, newU16End);
    }
}

auto TextInputState::OnSelectionChange(Bool anticipated) -> void
{
    if (_delegate.onSelectionChange)
    {
        _delegate.onSelectionChange(anticipated);
    }
}
}

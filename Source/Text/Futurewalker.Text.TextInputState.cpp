// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Text.TextInputState.hpp"

#include "Futurewalker.Core.StringFunction.hpp"

#include <unicode/utf8.h>
#include <unicode/utf16.h>

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
    auto const selectedRange = GetSelectedRange();

    _text = text;
    _selectedRange = {_text.GetCodePointCount(), _text.GetCodePointCount()}; // FIXME: Should we preserve current selection??
    auto const newTextRange = GetU16StringRange();
    auto const newSelectedRange = GetSelectedRange();

    OnTextChange(textRange.GetBegin(), textRange.GetEnd(), newTextRange.GetEnd());

    if (selectedRange != newSelectedRange)
    {
        OnSelectionChange();
    }
}

auto TextInputState::GetString() const -> String
{
    return _text.GetString();
}

auto TextInputState::GetString(Range<CodePoint> range) const -> String
{
    return _text.GetString(range);
}

auto TextInputState::GetStringRange() const -> Range<CodePoint>
{
    return {0, _text.GetCodePointCount()};
}

auto TextInputState::GetSelectedRange() const -> Range<CodePoint>
{
    return _selectedRange;
}

auto TextInputState::GetNormalizedSelectedRange() const -> Range<CodePoint>
{
    return Range<CodePoint>::Normalize(_selectedRange);
}

auto TextInputState::SetSelectedRange(Range<CodePoint> const& range) -> Bool
{
    auto const textRange = GetStringRange();
    auto const selectedRange = _selectedRange;

    auto const newBegin = Range<CodePoint>::Clamp(range.GetBegin(), textRange);
    auto const newEnd = Range<CodePoint>::Clamp(range.GetEnd(), textRange);
    auto const newSelectedRange = Range<CodePoint>(newBegin, newEnd);

    auto const selectionChanged = (newSelectedRange != selectedRange);
    auto const rangeChanged = range != newSelectedRange;

    if (selectionChanged)
    {
        OnSelectionChange();
    }

    if (rangeChanged)
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
    _composingRange.SetBegin(Range<CodePoint>::Clamp(range.GetBegin(), textRange));
    _composingRange.SetEnd(Range<CodePoint>::Clamp(range.GetEnd(), textRange));

    if (range != _selectedRange)
    {
        return false;
    }
    return true;
}

auto TextInputState::InsertText(String const& text, CodePoint caretPosition, Bool anticipated) -> void
{
    InsertTextCore(caretPosition, anticipated, Text(text));
}

auto TextInputState::DeleteSurroundingText(CodePoint before, CodePoint after, Bool anticipated) -> void
{
    if (BeforeDeleteSurroundingText(before, after))
    {
        auto const textRange = GetStringRange();
        auto const selectedRange = GetNormalizedSelectedRange();

        auto const postBegin = selectedRange.GetEnd();
        auto const postEnd = Range<CodePoint>::Clamp(selectedRange.GetEnd() + after, textRange);
        auto const postEnd16 = FindU16IndexFromCodePoint(postEnd);
        _text.Replace({postBegin, postEnd}, Text());

        auto const preBegin = Range<CodePoint>::Clamp(selectedRange.GetBegin() - before, textRange);
        auto const preBegin16 = FindU16IndexFromCodePoint(preBegin);
        auto const preEnd = selectedRange.GetBegin();
        _text.Replace({preBegin, preEnd}, Text());

        auto const offset = preEnd - preBegin;
        auto const newBegin = selectedRange.GetBegin() - offset;
        auto const newEnd = selectedRange.GetEnd() - offset;
        if (_selectedRange.GetLength() > 0)
        {
            _selectedRange = {newBegin, newEnd};
        }
        else
        {
            _selectedRange = {newEnd, newBegin};
        }

        if (!anticipated)
        {
            OnTextChange(preBegin16, postEnd16, FindU16IndexFromCodePoint(newEnd));

            if (before != 0)
            {
                OnSelectionChange();
            }
        }
    }
    else
    {
        if (anticipated)
        {
            auto const textRange = GetStringRange();
            auto const selectedRange = GetNormalizedSelectedRange();
            auto const oldBegin = Range<CodePoint>::Clamp(selectedRange.GetBegin() - before, textRange);
            auto const oldEnd = Range<CodePoint>::Clamp(oldBegin + selectedRange.GetLength(), textRange);
            auto const oldRange = FindU16RangeFromCodePointRange({oldBegin, oldEnd});
            auto const newRange = GetU16NormalizedSelectedRange();
            OnTextChange(oldRange.GetBegin(), oldRange.GetEnd(), newRange.GetEnd());

            if (before != 0)
            {
                OnSelectionChange();
            }
        }
    }
}

auto TextInputState::GetU16String() const -> U16String
{
    return _text.GetU16String();
}

auto TextInputState::GetU16String(Range<CodeUnit> range) const -> U16String
{
    auto const b = _text.GetCodePointIndexByU16Index(range.GetBegin());
    auto const e = _text.GetCodePointIndexByU16Index(range.GetEnd());
    return _text.GetU16String({b, e});
}

auto TextInputState::GetU16StringRange() const -> Range<CodeUnit>
{
    return {0, _text.GetU16CodeUnitCount()};
}

auto TextInputState::GetU16SelectedRange() const -> Range<CodeUnit>
{
    auto const range = GetSelectedRange();
    auto const b = _text.GetU16IndexByCodePointIndex(range.GetBegin());
    auto const e = _text.GetU16IndexByCodePointIndex(range.GetEnd());
    return {b, e};
}

auto TextInputState::GetU16NormalizedSelectedRange() const -> Range<CodeUnit>
{
    auto const selectedRange = GetU16SelectedRange();
    return Range<CodeUnit>::Normalize(selectedRange);
}

auto TextInputState::SetU16SelectedRange(Range<CodeUnit> range, Bool anticipated) -> void
{
    auto const selectedRange = _selectedRange;
    auto const b = FindCodePointFromU16Index(range.GetBegin());
    auto const e = FindCodePointFromU16Index(range.GetEnd());
    auto const newSelectedRange = Range<CodePoint>(b, e);

    if (newSelectedRange != selectedRange)
    {
        _selectedRange = newSelectedRange;

        if (!anticipated || range != GetU16SelectedRange())
        {
            OnSelectionChange();
        }
    }
}

auto TextInputState::GetU16ComposingRange() const -> Range<CodeUnit>
{
    auto const range = GetComposingRange();
    auto const b = _text.GetU16IndexByCodePointIndex(range.GetBegin());
    auto const e = _text.GetU16IndexByCodePointIndex(range.GetEnd());
    return {b, e};
}

auto TextInputState::SetU16ComposingRange(Range<CodeUnit> range) -> void
{
    auto const b = FindCodePointFromU16Index(range.GetBegin());
    auto const e = FindCodePointFromU16Index(range.GetEnd());
    _composingRange = {b, e};
}

auto TextInputState::InsertU16Text(U16StringView text, CodePoint caretPosition, Bool anticipated) -> void
{
    InsertTextCore(caretPosition, anticipated, Text(text));
}

auto TextInputState::GetRangeFromU16Range(Range<CodeUnit> range) const -> Range<CodePoint>
{
    auto const begin = FindCodePointFromU16Index(range.GetBegin());
    auto const end = FindCodePointFromU16Index(range.GetEnd());
    return {begin, end};
}

auto TextInputState::GetRangeFromU8Range(Range<CodeUnit> range) const -> Range<CodePoint>
{
    auto const begin = FindCodePointFromU8Index(range.GetBegin());
    auto const end = FindCodePointFromU8Index(range.GetEnd());
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
    auto const b = _text.GetU8IndexByCodePointIndex(range.GetBegin());
    auto const e = _text.GetU8IndexByCodePointIndex(range.GetEnd());
    return Range<CodeUnit>(b, e);
}

auto TextInputState::FindU16IndexFromCodePoint(CodePoint codePoint) const -> CodeUnit
{
    return _text.GetU16IndexByCodePointIndex(codePoint);
}

auto TextInputState::FindU16RangeFromCodePointRange(Range<CodePoint> range) const -> Range<CodeUnit>
{
    auto const b = _text.GetU16IndexByCodePointIndex(range.GetBegin());
    auto const e = _text.GetU16IndexByCodePointIndex(range.GetEnd());
    return Range<CodeUnit>(b, e);
}

auto TextInputState::InsertTextCore(CodePoint caretPosition, Bool anticipated, Text const& text) -> void
{
    if (BeforeInsertText(text.GetString()))
    {
        auto const oldSelection = GetNormalizedSelectedRange();
        auto const oldU16Selection = GetU16NormalizedSelectedRange();
        _text.Replace(oldSelection, text);

        auto const newPosition = oldSelection.GetBegin() + ((caretPosition > 0) ? text.GetCodePointCount() + caretPosition - 1 : caretPosition);
        _selectedRange = {newPosition, newPosition};

        if (!anticipated)
        {
            OnTextChange(oldU16Selection.GetBegin(), oldU16Selection.GetEnd(), oldU16Selection.GetBegin() + text.GetU16CodeUnitCount());

            if (_selectedRange != oldSelection)
            {
                OnSelectionChange();
            }
        }
    }
    else
    {
        if (anticipated)
        {
            auto const u16SelectedRange = GetU16NormalizedSelectedRange();
            OnTextChange(u16SelectedRange.GetBegin(), u16SelectedRange.GetBegin() + text.GetU16CodeUnitCount(), u16SelectedRange.GetEnd());
            OnSelectionChange();
        }
    }
}

auto TextInputState::BeforeInsertText(String const& text) -> Bool
{
    if (_delegate.beforeInsertText)
    {
        return _delegate.beforeInsertText(text);
    }
    return true;
}

auto TextInputState::BeforeDeleteSurroundingText(CodePoint before, CodePoint after) -> Bool
{
    if (_delegate.beforeDeleteSurroundingText)
    {
        return _delegate.beforeDeleteSurroundingText(before, after);
    }
    return true;
}

auto TextInputState::OnTextChange(CodeUnit u16Begin, CodeUnit oldU16End, CodeUnit newU16End) -> void
{
    if (_delegate.onTextChange)
    {
        _delegate.onTextChange(u16Begin, oldU16End, newU16End);
    }
}

auto TextInputState::OnSelectionChange() -> void
{
    if (_delegate.onSelectionChange)
    {
        _delegate.onSelectionChange();
    }
}
}

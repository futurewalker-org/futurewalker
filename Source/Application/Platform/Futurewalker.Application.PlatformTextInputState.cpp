// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformTextInputState.hpp"

#include "Futurewalker.Core.StringFunction.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include <unicode/utf8.h>
#include <unicode/utf16.h>

namespace FW_DETAIL_NS
{
namespace
{
auto ClampToRange(auto const& val, auto const& range)
{
    return Utility::Clamp(val, range.GetBegin(), range.GetEnd());
}

auto IteratorAt(auto& c, auto idx)
{
    return std::next(c.begin(), static_cast<ptrdiff_t>(idx));
}

auto InsertRange(auto& c, auto b, auto e, auto f, auto l)
{
    auto const nb = std::distance(c.begin(), b);
    auto const tail = (e == c.end()) ? b : std::rotate(b, e, c.end());
    c.erase(tail, c.end());
    auto const len = std::distance(f, l);
    auto const it = c.insert(c.begin() + nb, f, l);
    return std::ranges::subrange(it, it + len);
}

auto Utf8Append(auto& buffer, int32_t c)
{
    auto i = int32_t();
    auto tmp = std::array<uint8_t, 4>();
    U8_APPEND_UNSAFE(tmp.data(), i, c);
    buffer.append_range(std::ranges::subrange(tmp.begin(), tmp.begin() + i));
}

auto Utf16Append(auto& buffer, int32_t c)
{
    auto i = int32_t();
    auto tmp = std::array<UChar, 2>();
    U16_APPEND_UNSAFE(tmp.data(), i, c);
    buffer.append_range(std::ranges::subrange(tmp.begin(), tmp.begin() + i));
}

auto MakeStringData16Core(
  String& utf8String,
  std::u16string& utf16String,
  CodePoint& codePoints,
  std::vector<CodeUnit>& utf8Bounds,
  std::vector<CodeUnit>& utf16Bounds,
  std::u16string_view const source) -> Bool
{
    auto const utf16Str = source.data();
    auto const utf16Length = static_cast<int32_t>(source.size());

    utf8String = {};
    utf16String = {};
    codePoints = 0;

    utf8Bounds = {0};
    utf16Bounds = {0};

    if (utf16Length <= 0)
    {
        return true;
    }

    auto utf8Buffer = std::vector<uint8_t>();
    auto utf16Buffer = std::vector<UChar>();

    utf16Buffer.reserve(utf16Length);

    auto r = true;
    auto i = int32_t();
    while (i < utf16Length)
    {
        auto c = UChar32();
        auto next = i;
        U16_NEXT(utf16Str, next, utf16Length, c);
        if (0 <= c)
        {
            Utf8Append(utf8Buffer, c);
            assert(next - i <= 2);
            utf16Buffer.append_range(std::ranges::subrange(&utf16Str[i], &utf16Str[next]));
        }
        else
        {
            r = false;
            c = 0xfffd;
            Utf8Append(utf8Buffer, c);
            Utf16Append(utf16Buffer, c);
        }

        utf8Bounds.push_back(static_cast<CodeUnit>(utf8Buffer.size()));
        utf16Bounds.push_back(static_cast<CodeUnit>(utf16Buffer.size()));
        ++codePoints;
        i = next;
    }

    utf8String = StringFunction::ConvertUtf8ToStringUnchecked({reinterpret_cast<const char*>(utf8Buffer.data()), utf8Buffer.size()});
    utf16String = std::u16string(utf16Buffer.data(), utf16Buffer.size());

    if (r)
    {
        assert(utf16String == source);
        return true;
    }
    return false;
}

auto MakeStringData8Core(String& utf8String, std::u16string& utf16String, CodePoint& codePoints, std::vector<CodeUnit>& utf8Bounds, std::vector<CodeUnit>& utf16Bounds, String const& source) -> Bool
{
    auto const view = source.GetView();
    auto const data = view.GetData();

    auto const utf8Str = reinterpret_cast<const uint8_t*>(static_cast<const String::ValueType*>(data));
    auto const utf8Length = static_cast<int32_t>(view.GetSize());

    utf8String = {};
    utf16String = {};
    codePoints = 0;

    utf8Bounds = {0};
    utf16Bounds = {0};

    if (utf8Length <= 0)
    {
        return true;
    }

    auto utf8Buffer = std::vector<uint8_t>();
    auto utf16Buffer = std::vector<UChar>();

    utf8Buffer.reserve(utf8Length);

    auto r = true;
    auto i = int32_t();
    while (i < utf8Length)
    {
        auto c = UChar32();
        auto next = i;
        U8_NEXT(utf8Str, next, utf8Length, c);
        if (0 <= c)
        {
            assert(next - i <= 4);
            utf8Buffer.append_range(std::ranges::subrange(&utf8Str[i], &utf8Str[next]));
            Utf16Append(utf16Buffer, c);
        }
        else
        {
            r = false;
            c = 0xfffd;
            Utf8Append(utf8Buffer, c);
            Utf16Append(utf16Buffer, c);
        }

        utf8Bounds.push_back(static_cast<CodeUnit>(utf8Buffer.size()));
        utf16Bounds.push_back(static_cast<CodeUnit>(utf16Buffer.size()));
        ++codePoints;
        i = next;
    }

    utf8String = StringFunction::ConvertUtf8ToStringUnchecked({reinterpret_cast<char const*>(utf8Buffer.data()), utf8Buffer.size()});
    utf16String = std::u16string(utf16Buffer.data(), utf16Buffer.size());

    if (r)
    {
        assert(utf8String == source);
        return true;
    }
    return false;
}

auto MakeStringData8WithBounds(String& utf8String, std::u16string& utf16String, CodePoint& codePoints, std::vector<CodeUnit>& utf8Bounds, std::vector<CodeUnit>& utf16Bounds, String const& source)
  -> Bool
{
    return MakeStringData8Core(utf8String, utf16String, codePoints, utf8Bounds, utf16Bounds, source);
}

auto MakeStringData16WithBounds(
  String& utf8String,
  std::u16string& utf16String,
  CodePoint& codePoints,
  std::vector<CodeUnit>& utf8Bounds,
  std::vector<CodeUnit>& utf16Bounds,
  std::u16string_view const source) -> Bool
{
    return MakeStringData16Core(utf8String, utf16String, codePoints, utf8Bounds, utf16Bounds, source);
}
}

PlatformTextInputState::PlatformTextInputState(Delegate const& delegate)
  : _delegate {delegate}
{
    _utf8Bounds = {0};
    _utf16Bounds = {0};
}

auto PlatformTextInputState::GetText() const -> String
{
    return _utf8Text;
}

auto PlatformTextInputState::GetText(Range<CodePoint> range) const -> String
{
    auto const tr = GetTextRange();
    auto const b = ClampToRange(range.GetBegin(), tr);
    auto const e = ClampToRange(range.GetEnd(), tr);
    auto const b8 = _utf8Bounds[static_cast<size_t>(b)];
    auto const e8 = _utf8Bounds[static_cast<size_t>(e)];
    return _utf8Text.GetSubstring(static_cast<SInt64>(b8), static_cast<SInt64>(e8));
}

auto PlatformTextInputState::GetTextRange() const -> Range<CodePoint>
{
    FW_DEBUG_ASSERT(_utf8Bounds.size() == _utf16Bounds.size());
    return {0, CodePoint::Max(0, CodePoint(_utf8Bounds.size()) - 1)};
}

auto PlatformTextInputState::SetText(String const& text) -> Bool
{
    auto const textRange = GetU16TextRange();
    auto const selectedRange = _selectedRange;

    CodePoint codePoints = 0;
    auto const textChanged = MakeStringData8WithBounds(_utf8Text, _utf16Text, codePoints, _utf8Bounds, _utf16Bounds, text);
    auto const newTextRange = GetU16TextRange();
    auto const newSelectedRange = _selectedRange;

    OnTextChange(textRange.GetBegin(), textRange.GetEnd(), newTextRange.GetEnd());

    if (selectedRange != newSelectedRange)
    {
        OnSelectionChange();
    }

    return textChanged;
}

auto PlatformTextInputState::GetSelectedRange() const -> Range<CodePoint>
{
    return _selectedRange;
}

auto PlatformTextInputState::GetNormalizedSelectedRange() const -> Range<CodePoint>
{
    return Range<CodePoint>::Normalize(_selectedRange);
}

auto PlatformTextInputState::SetSelectedRange(Range<CodePoint> const& range) -> Bool
{
    auto const textRange = GetTextRange();
    auto const selectedRange = _selectedRange;

    auto const newBegin = ClampToRange(range.GetBegin(), textRange);
    auto const newEnd = ClampToRange(range.GetEnd(), textRange);
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

auto PlatformTextInputState::GetComposingRange() const -> Range<CodePoint>
{
    return _composingRange;
}

auto PlatformTextInputState::SetComposingRange(Range<CodePoint> const& range) -> Bool
{
    auto const textRange = GetTextRange();
    _composingRange.SetBegin(ClampToRange(range.GetBegin(), textRange));
    _composingRange.SetEnd(ClampToRange(range.GetEnd(), textRange));

    if (range != _selectedRange)
    {
        return false;
    }
    return true;
}

auto PlatformTextInputState::InsertText(String const& text, CodePoint caretPosition, Bool anticipated) -> void
{
    auto utf8Text = String();
    auto utf8Bounds = std::vector<CodeUnit>();
    auto utf16Text = std::u16string();
    auto utf16Bounds = std::vector<CodeUnit>();
    auto codePoints = CodePoint(0);
    MakeStringData8WithBounds(utf8Text, utf16Text, codePoints, utf8Bounds, utf16Bounds, text);
    InsertTextData(caretPosition, anticipated, utf8Text, utf8Bounds, utf16Text, utf16Bounds, codePoints);
}

auto PlatformTextInputState::DeleteSurroundingText(CodePoint before, CodePoint after, Bool anticipated) -> void
{
    if (BeforeDeleteSurroundingText(before, after))
    {
        auto const textRange = GetTextRange();
        auto const selectedRange = GetNormalizedSelectedRange();

        auto const postBegin = selectedRange.GetEnd();
        auto const postEnd = ClampToRange(selectedRange.GetEnd() + after, textRange);
        auto const postEnd16 = FindU16IndexFromCodePoint(postEnd);
        DeleteTextCore({postBegin, postEnd});

        auto const preBegin = ClampToRange(selectedRange.GetBegin() - before, textRange);
        auto const preBegin16 = FindU16IndexFromCodePoint(preBegin);
        auto const preEnd = selectedRange.GetBegin();
        DeleteTextCore({preBegin, preEnd});

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
            auto const textRange = GetTextRange();
            auto const selectedRange = GetNormalizedSelectedRange();
            auto const oldBegin = ClampToRange(selectedRange.GetBegin() - before, textRange);
            auto const oldEnd = ClampToRange(oldBegin + selectedRange.GetLength(), textRange);
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

auto PlatformTextInputState::GetU16Text() const -> std::u16string_view
{
    return _utf16Text;
}

auto PlatformTextInputState::GetU16Text(Range<CodeUnit> range) const -> std::u16string_view
{
    auto const textRange = GetU16TextRange();
    auto const begin = std::max(textRange.GetBegin(), range.GetBegin());
    auto const end = std::min(textRange.GetEnd(), textRange.GetEnd());
    if (begin >= end)
    {
        return {};
    }
    return std::u16string_view(_utf16Text).substr(static_cast<size_t>(begin), static_cast<size_t>(end - begin));
}

auto PlatformTextInputState::GetU16TextRange() const -> Range<CodeUnit>
{
    return {0, static_cast<SInt64>(_utf16Text.size())};
}

auto PlatformTextInputState::GetU16SelectedRange() const -> Range<CodeUnit>
{
    auto const b = _selectedRange.GetBegin();
    auto const e = _selectedRange.GetEnd();
    auto const b16 = _utf16Bounds[static_cast<size_t>(b)];
    auto const e16 = _utf16Bounds[static_cast<size_t>(e)];
    return {b16, e16};
}

auto PlatformTextInputState::GetU16NormalizedSelectedRange() const -> Range<CodeUnit>
{
    auto const selectedRange = GetU16SelectedRange();
    return Range<CodeUnit>::Normalize(selectedRange);
}

auto PlatformTextInputState::SetU16SelectedRange(Range<CodeUnit> range, Bool anticipated) -> void
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

auto PlatformTextInputState::GetU16ComposingRange() const -> Range<CodeUnit>
{
    auto const b = _composingRange.GetBegin();
    auto const e = _composingRange.GetEnd();
    auto const b16 = _utf16Bounds[static_cast<size_t>(b)];
    auto const e16 = _utf16Bounds[static_cast<size_t>(e)];
    return {b16, e16};
}

auto PlatformTextInputState::SetU16ComposingRange(Range<CodeUnit> range) -> void
{
    auto const b = FindCodePointFromU16Index(range.GetBegin());
    auto const e = FindCodePointFromU16Index(range.GetEnd());
    _composingRange = {b, e};
}

auto PlatformTextInputState::InsertU16Text(std::u16string_view text, CodePoint caretPosition, Bool anticipated) -> void
{
    auto utf8Text = String();
    auto utf8Bounds = std::vector<CodeUnit>();
    auto utf16Text = std::u16string();
    auto utf16Bounds = std::vector<CodeUnit>();
    auto codePoints = CodePoint(0);
    MakeStringData16WithBounds(utf8Text, utf16Text, codePoints, utf8Bounds, utf16Bounds, text);
    InsertTextData(caretPosition, anticipated, utf8Text, utf8Bounds, utf16Text, utf16Bounds, codePoints);
}

auto PlatformTextInputState::GetRangeFromU16Range(Range<CodeUnit> range) const -> Range<CodePoint>
{
    auto const begin = FindCodePointFromU16Index(range.GetBegin());
    auto const end = FindCodePointFromU16Index(range.GetEnd());
    return {begin, end};
}

auto PlatformTextInputState::GetRangeFromU8Range(Range<CodeUnit> range) const -> Range<CodePoint>
{
    auto const begin = FindCodePointFromU8Index(range.GetBegin());
    auto const end = FindCodePointFromU8Index(range.GetEnd());
    return {begin, end};
}

auto PlatformTextInputState::GetU16RangeFromRange(Range<CodePoint> range) const -> Range<CodeUnit>
{
    return FindU16RangeFromCodePointRange(range);
}

auto PlatformTextInputState::GetU8RangeFromRange(Range<CodePoint> range) const -> Range<CodeUnit>
{
    return FindU8RangeFromCodePointRange(range);
}

auto PlatformTextInputState::FindCodePointFromU8Index(CodeUnit index) const -> CodePoint
{
    if (index < 0)
    {
        return 0;
    }

    for (auto i = 0uz; i < _utf8Bounds.size(); ++i)
    {
        if (_utf8Bounds[i] > index)
        {
            return CodePoint(i) - 1;
        }
    }
    FW_DEBUG_ASSERT(!_utf8Bounds.empty());
    return CodePoint::Max(0, CodePoint(_utf8Bounds.size()) - 1);
}

auto PlatformTextInputState::FindCodePointFromU16Index(CodeUnit index) const -> CodePoint
{
    if (index < 0)
    {
        return 0;
    }

    for (auto i = 0uz; i < _utf16Bounds.size(); ++i)
    {
        if (_utf16Bounds[i] > index)
        {
            return CodePoint(i) - 1;
        }
    }
    FW_DEBUG_ASSERT(!_utf16Bounds.empty());
    return CodePoint::Max(0, CodePoint(_utf16Bounds.size()) - 1);
}

auto PlatformTextInputState::FindU8IndexFromCodePoint(CodePoint codePoint) const -> CodeUnit
{
    auto const boundsRange = Range<CodePoint>(0, CodePoint(_utf8Bounds.size()));
    auto const index = ClampToRange(codePoint, boundsRange);
    return _utf8Bounds[static_cast<size_t>(index)];
}

auto PlatformTextInputState::FindU8RangeFromCodePointRange(Range<CodePoint> range) const -> Range<CodeUnit>
{
    auto const boundsRange = Range<CodePoint>(0, CodePoint(_utf8Bounds.size()));
    auto const b = ClampToRange(range.GetBegin(), boundsRange);
    auto const e = ClampToRange(range.GetEnd(), boundsRange);
    FW_DEBUG_ASSERT(Range<CodePoint>(b, e) == range);
    return Range<CodeUnit>(_utf8Bounds[static_cast<size_t>(b)], _utf8Bounds[static_cast<size_t>(e)]);
}

auto PlatformTextInputState::FindU16IndexFromCodePoint(CodePoint codePoint) const -> CodeUnit
{
    auto const boundsRange = Range<CodePoint>(0, CodePoint(_utf16Bounds.size()));
    auto const index = ClampToRange(codePoint, boundsRange);
    return _utf16Bounds[static_cast<size_t>(index)];
}

auto PlatformTextInputState::FindU16RangeFromCodePointRange(Range<CodePoint> range) const -> Range<CodeUnit>
{
    auto const boundsRange = Range<CodePoint>(0, CodePoint(_utf16Bounds.size()));
    auto const b = ClampToRange(range.GetBegin(), boundsRange);
    auto const e = ClampToRange(range.GetEnd(), boundsRange);
    return Range<CodeUnit>(_utf16Bounds[static_cast<size_t>(b)], _utf16Bounds[static_cast<size_t>(e)]);
}

auto PlatformTextInputState::InsertTextData(
  CodePoint caretPosition,
  Bool anticipated,
  StringView utf8Text,
  std::span<CodeUnit const> utf8Bounds,
  std::u16string_view utf16Text,
  std::span<CodeUnit const> utf16Bounds,
  CodePoint codePoints) -> void
{
    if (BeforeInsertText(utf8Text))
    {
        auto const oldSelection = GetNormalizedSelectedRange();
        auto const oldU16Selection = GetU16NormalizedSelectedRange();
        InsertTextCore(utf8Text, utf8Bounds, utf16Text, utf16Bounds, oldSelection);

        auto const newPosition = oldSelection.GetBegin() + ((caretPosition > 0) ? codePoints + caretPosition - 1 : caretPosition);
        _selectedRange = {newPosition, newPosition};

        if (!anticipated)
        {
            OnTextChange(oldU16Selection.GetBegin(), oldU16Selection.GetEnd(), oldU16Selection.GetBegin() + std::ssize(utf16Text));

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
            OnTextChange(u16SelectedRange.GetBegin(), u16SelectedRange.GetBegin() + std::ssize(utf16Text), u16SelectedRange.GetEnd());
            OnSelectionChange();
        }
    }
}

auto PlatformTextInputState::InsertTextCore(StringView utf8Text, std::span<CodeUnit const> utf8Bounds, std::u16string_view utf16Text, std::span<CodeUnit const> utf16Bounds, Range<CodePoint> range)
  -> void
{
    InsertU8TextCore(utf8Text, utf8Bounds, range);
    InsertU16TextCore(utf16Text, utf16Bounds, range);
}

auto PlatformTextInputState::InsertU8TextCore(StringView utf8Text, std::span<CodeUnit const> utf8Bounds, Range<CodePoint> range) -> void
{
    auto const oldUtf8Range = FindU8RangeFromCodePointRange(range);
    auto const oldUtf8Length = oldUtf8Range.GetLength();
    auto const newUtf8Length = CodeUnit(utf8Text.GetSize());

    if (oldUtf8Range.GetLength() > 0)
    {
        _utf8Text.Replace(static_cast<SInt64>(oldUtf8Range.GetBegin()), static_cast<SInt64>(oldUtf8Range.GetEnd()), utf8Text);
    }
    else
    {
        _utf8Text.Insert(static_cast<SInt64>(oldUtf8Range.GetBegin()), utf8Text);
    }

    auto const replaceBegin = IteratorAt(_utf8Bounds, range.GetBegin());
    auto const replaceBeginValue = *replaceBegin;
    auto const replaceEnd = IteratorAt(_utf8Bounds, range.GetEnd() + 1);
    auto const replacedRange = InsertRange(_utf8Bounds, replaceBegin, replaceEnd, utf8Bounds.begin(), utf8Bounds.end());

    for (auto& idx : replacedRange)
    {
        idx += replaceBeginValue;
    }

    for (auto& idx : std::ranges::subrange(replacedRange.end(), _utf8Bounds.end()))
    {
        idx += (newUtf8Length - oldUtf8Length);
    }
}

auto PlatformTextInputState::InsertU16TextCore(std::u16string_view utf16Text, std::span<CodeUnit const> utf16Bounds, Range<CodePoint> range) -> void
{
    auto const oldUtf16Range = FindU16RangeFromCodePointRange(range);
    auto const oldUtf16Length = oldUtf16Range.GetLength();
    auto const newUtf16Length = CodeUnit(utf16Text.size());

    if (oldUtf16Range.GetLength() > 0)
    {
        _utf16Text.replace(static_cast<size_t>(oldUtf16Range.GetBegin()), static_cast<size_t>(oldUtf16Range.GetLength()), utf16Text);
    }
    else
    {
        _utf16Text.insert(static_cast<size_t>(oldUtf16Range.GetBegin()), utf16Text);
    }

    auto const replaceBegin = IteratorAt(_utf16Bounds, range.GetBegin());
    auto const replaceBeginValue = *replaceBegin;
    auto const replaceEnd = IteratorAt(_utf16Bounds, range.GetEnd() + 1);
    auto const replacedRange = InsertRange(_utf16Bounds, replaceBegin, replaceEnd, utf16Bounds.begin(), utf16Bounds.end());

    for (auto& idx : replacedRange)
    {
        idx += replaceBeginValue;
    }

    for (auto& idx : std::ranges::subrange(replacedRange.end(), _utf16Bounds.end()))
    {
        idx += (newUtf16Length - oldUtf16Length);
    }
}

auto PlatformTextInputState::DeleteTextCore(Range<CodePoint> range) -> void
{
    auto const bounds = std::array<CodeUnit, 1> {0};
    InsertU8TextCore({}, bounds, range);
    InsertU16TextCore({}, bounds, range);
}

auto PlatformTextInputState::BeforeInsertText(String const& text) -> Bool
{
    if (_delegate.beforeInsertText)
    {
        return _delegate.beforeInsertText(text);
    }
    return true;
}

auto PlatformTextInputState::BeforeDeleteSurroundingText(CodePoint before, CodePoint after) -> Bool
{
    if (_delegate.beforeDeleteSurroundingText)
    {
        return _delegate.beforeDeleteSurroundingText(before, after);
    }
    return true;
}

auto PlatformTextInputState::OnTextChange(CodeUnit u16Begin, CodeUnit oldU16End, CodeUnit newU16End) -> void
{
    if (_delegate.onTextChange)
    {
        _delegate.onTextChange(u16Begin, oldU16End, newU16End);
    }
}

auto PlatformTextInputState::OnSelectionChange() -> void
{
    if (_delegate.onSelectionChange)
    {
        _delegate.onSelectionChange();
    }
}
}

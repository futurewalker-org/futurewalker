// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Text.Text.hpp"

#include "Futurewalker.Core.StringFunction.hpp"
#include "Futurewalker.Core.UnicodeFunction.hpp"
#include "Futurewalker.Core.Range.hpp"

#include <vector>
#include <algorithm>

namespace FW_DETAIL_NS
{
namespace
{
auto MakeStringDataU8(U16String& utf16String, std::vector<CodeUnit>& utf8Bounds, std::vector<CodeUnit>& utf16Bounds, CodePoint& codePoints, String const& source) -> void
{
    utf16String = {};
    codePoints = 0;
    utf8Bounds = {0};
    utf16Bounds = {0};

    auto index = source.GetBegin();
    auto c = String::CharType();
    while (source.GetChar(index, c, index))
    {
        utf16String.Append(c);
        utf16Bounds.push_back(utf16String.GetEnd());
        utf8Bounds.push_back(index);
        ++codePoints;
    }
}

auto MakeStringDataU16(String& utf8String, std::vector<CodeUnit>& utf8Bounds, std::vector<CodeUnit>& utf16Bounds, CodePoint& codePoints, U16String const& source) -> void
{
    utf8String = {};
    codePoints = 0;
    utf8Bounds = {0};
    utf16Bounds = {0};

    auto index = source.GetBegin();
    auto c = U16String::CharType();
    while (source.GetChar(index, c, index))
    {
        utf8String.Append(c);
        utf8Bounds.push_back(utf8String.GetEnd());
        utf16Bounds.push_back(index);
        ++codePoints;
    }
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
}

///
/// @brief Create text from UTF-8 string.
///
/// @param[in] string UTF-8 string.
///
Text::Text(String const& string)
{
    _state = CreateState(string);
}

///
/// @brief Create text from UTF-16 string.
///
/// @param[in] string UTF-16 string.
///
Text::Text(U16String const& string)
{
    _state = CreateState(string);
}

///
/// @brief Get UTF-8 string.
///
auto Text::GetString() const -> String
{
    if (auto const state = GetImmutableState())
    {
        return state->u8String;
    }
    return {};
}

///
/// @brief Get substring of UTF-8 string.
///
/// @param[in] range Range of code points.
///
auto Text::GetString(Range<CodePoint> range) const -> String
{
    if (auto const state = GetImmutableState())
    {
        auto const begin = GetU8IndexByCodePointIndex(range.begin);
        auto const end = GetU8IndexByCodePointIndex(range.end);
        return state->u8String.GetSubString(begin, end);
    }
    return {};
}

///
/// @brief Set UTF-8 string.
///
/// @param[in] string UTF-8 string.
///
auto Text::SetString(String const& string) -> void
{
    _state.Reset();
    _state = CreateState(string);
}

///
/// @brief Get UTF-16 string.
///
auto Text::GetU16String() const -> U16String
{
    if (auto const state = GetImmutableState())
    {
        return state->u16String;
    }
    return {};
}

///
/// @brief Get substring of UTF-16 string.
///
/// @param[in] range Range of code points.
///
auto Text::GetU16String(Range<CodePoint> range) const -> U16String
{
    if (auto const state = GetImmutableState())
    {
        auto const begin = GetU16IndexByCodePointIndex(range.begin);
        auto const end = GetU16IndexByCodePointIndex(range.end);
        return state->u16String.GetSubString(begin, end);
    }
    return {};
}

///
/// @brief Set UTF-16 string.
///
auto Text::SetU16String(U16String const& string) -> void
{
    _state.Reset();
    _state = CreateState(string);
}

///
/// @brief Get number of code points stored in the text.
///
auto Text::GetCodePointCount() const -> CodePoint
{
    if (auto const state = GetImmutableState())
    {
        return state->codePoints;
    }
    return 0;
}

///
/// @brief Get number of UTF-8 code units stored in the text.
///
auto Text::GetU8CodeUnitCount() const -> CodeUnit
{
    if (auto const state = GetImmutableState())
    {
        return static_cast<CodeUnit>(state->u8String.GetView().GetSize());
    }
    return 0;
}

///
/// @brief Get number of UTF-16 code units stored in the text.
///
auto Text::GetU16CodeUnitCount() const -> CodeUnit
{
    if (auto const state = GetImmutableState())
    {
        return static_cast<CodeUnit>(state->u16String.GetView().GetSize());
    }
    return 0;
}

///
/// @brief Get code point index by UTF-8 code unit index.
///
/// @param[in] index UTF-8 code unit index.
///
/// @return Index of code point which contains the specified UTF-8 code unit.
///
/// @note if `index` is negative, returns 0.
/// @note if `index` is greater than or equal to the number of UTF-8 code units in the text, returns GetCodePointCount().
///
auto Text::GetCodePointIndexByU8Index(CodeUnit const index) const -> CodePoint
{
    if (auto const state = GetImmutableState())
    {
        for (auto i = 0z; i < std::ssize(state->utf8Bounds); ++i)
        {
            if (state->utf8Bounds[i] > index)
            {
                return CodePoint::Max(0, CodePoint(i) - 1);
            }
        }
        return GetCodePointCount();
    }
    return 0;
}

///
/// @brief Get code point index by UTF-16 code unit index.
///
/// @param[in] index UTF-16 code unit index.
///
/// @return Index of code point which contains the specified UTF-16 code unit.
///
/// @note if `index` is negative, returns 0.
/// @note if `index` is greater than or equal to the number of UTF-16 code units in the text, returns GetCodePointCount().
///
auto Text::GetCodePointIndexByU16Index(CodeUnit const index) const -> CodePoint
{
    if (auto const state = GetImmutableState())
    {
        for (auto i = 0z; i < std::ssize(state->utf16Bounds); ++i)
        {
            if (state->utf16Bounds[i] > index)
            {
                return CodePoint::Max(0, CodePoint(i) - 1);
            }
        }
        return GetCodePointCount();
    }
    return 0;
}

///
/// @brief Get UTF-8 code unit index by code point index.
///
/// @param[in] index Code point index.
///
/// @return Index of UTF-8 code unit of the specified code point.
///
/// @note if `index` is negative, returns 0.
/// @note if `index` is greater than or equal to the number of code points in the text, returns GetU8CodeUnitCount().
///
auto Text::GetU8IndexByCodePointIndex(CodePoint const index) const -> CodeUnit
{
    if (auto const state = GetImmutableState())
    {
        auto const idx = CodePoint::Clamp(index, 0, std::ssize(state->utf8Bounds) - 1);
        return state->utf8Bounds[static_cast<size_t>(idx)];
    }
    return 0;
}

///
/// @brief Get UTF-16 code unit index by code point index.
///
/// @param[in] index Code point index.
///
/// @return Index of UTF-16 code unit of the specified code point.
///
/// @note if `index` is negative, returns 0.
/// @note if `index` is greater than or equal to the number of code points in the text, returns GetU16CodeUnitCount().
///
auto Text::GetU16IndexByCodePointIndex(CodePoint const index) const -> CodeUnit
{
    if (auto const state = GetImmutableState())
    {
        auto const idx = CodePoint::Clamp(index, 0, std::ssize(state->utf16Bounds) - 1);
        return state->utf16Bounds[static_cast<size_t>(idx)];
    }
    return 0;
}

///
/// @brief Get sub-text.
///
auto Text::GetSubText(Range<CodePoint> const& range) const -> Text
{
    // TODO: optimize
    return Text(GetString(range));
}

///
/// @brief Get sub-text.
///
auto Text::GetSubTextByU8Range(Range<CodeUnit> const& range) const -> Text
{
    // TODO: optimize
    auto const textRange = Range<CodeUnit>(0, GetU8CodeUnitCount());
    auto const b = Range<CodeUnit>::Clamp(range.begin, textRange);
    auto const e = Range<CodeUnit>::Clamp(range.end, textRange);
    return Text(GetString().GetSubString(b, e));
}

///
/// @brief Get sub-text.
///
auto Text::GetSubTextByU16Range(Range<CodeUnit> const& range) const -> Text
{
    // TODO: optimize
    auto const textRange = Range<CodeUnit>(0, GetU16CodeUnitCount());
    auto const b = Range<CodeUnit>::Clamp(range.begin, textRange);
    auto const e = Range<CodeUnit>::Clamp(range.end, textRange);
    return Text(GetString().GetSubString(b, e));
}

///
/// @brief Insert text at specificed position.
///
/// @param[in] range Range of text to be replaced.
/// @param[in] text Text to insert.
///
auto Text::Replace(Range<CodePoint> const& range, Text const& text) -> void
{
    if (range.begin < 0 || range.begin > range.end || range.end > GetCodePointCount())
    {
        return;
    }

    if (auto const selfState = GetMutableState())
    {
        auto const otherState = text.GetImmutableState();
        {
            auto const& u8String = otherState ? otherState->u8String : String();
            auto const& u8Bounds = otherState ? otherState->utf8Bounds : std::vector<CodeUnit>(1, 0);
            ReplaceU8Core(range, u8String, u8Bounds);
        }
        {
            auto const& u16String = otherState ? otherState->u16String : U16String();
            auto const& u16Bounds = otherState ? otherState->utf16Bounds : std::vector<CodeUnit>(1, 0);
            ReplaceU16Core(range, u16String, u16Bounds);
        }
        auto const codePointCountDifference = text.GetCodePointCount() - range.GetLength();
        selfState->codePoints += codePointCountDifference;
    }
    else
    {
        _state = text._state;
    }
}

auto Text::ReplaceU8Core(Range<CodePoint> const& range, String const& u8String, std::vector<CodeUnit> const& u8Bounds) -> void
{
    if (auto const selfState = GetMutableState())
    {
        auto const u8Range = Range<CodeUnit>(GetU8IndexByCodePointIndex(range.begin), GetU8IndexByCodePointIndex(range.end));
        selfState->u8String.Replace(u8Range.begin, u8Range.end, u8String);

        auto const replaceBegin = IteratorAt(selfState->utf8Bounds, range.begin);
        auto const replaceBeginValue = *replaceBegin;
        auto const replaceEnd = IteratorAt(selfState->utf8Bounds, range.end + 1);
        auto const replacedRange = InsertRange(selfState->utf8Bounds, replaceBegin, replaceEnd, u8Bounds.begin(), u8Bounds.end());

        for (auto& idx : replacedRange)
        {
            idx += replaceBeginValue;
        }

        auto const oldUtf8Length = u8Range.GetLength();
        auto const newUtf8Length = CodeUnit(u8String.GetView().GetSize());
        for (auto& idx : std::ranges::subrange(replacedRange.end(), selfState->utf8Bounds.end()))
        {
            idx += (newUtf8Length - oldUtf8Length);
        }
    }
}

auto Text::ReplaceU16Core(Range<CodePoint> const& range, U16String const& u16String, std::vector<CodeUnit> const& u16Bounds) -> void
{
    if (auto const selfState = GetMutableState())
    {
        auto const u16Range = Range<CodeUnit>(GetU16IndexByCodePointIndex(range.begin), GetU16IndexByCodePointIndex(range.end));
        selfState->u16String.Replace(u16Range.begin, u16Range.end, u16String);

        auto const replaceBegin = IteratorAt(selfState->utf16Bounds, range.begin);
        auto const replaceBeginValue = *replaceBegin;
        auto const replaceEnd = IteratorAt(selfState->utf16Bounds, range.end + 1);
        auto const replacedRange = InsertRange(selfState->utf16Bounds, replaceBegin, replaceEnd, u16Bounds.begin(), u16Bounds.end());

        for (auto& idx : replacedRange)
        {
            idx += replaceBeginValue;
        }

        auto const oldUtf16Length = u16Range.GetLength();
        auto const newUtf16Length = CodeUnit(u16String.GetView().GetSize());
        for (auto& idx : std::ranges::subrange(replacedRange.end(), selfState->utf16Bounds.end()))
        {
            idx += (newUtf16Length - oldUtf16Length);
        }
    }
}

///
/// @brief Create state.
///
auto Text::CreateState(String const& text) const -> Shared<State>
{
    auto state = Shared<State>::Make();
    state->u8String = text;
    MakeStringDataU8(state->u16String, state->utf8Bounds, state->utf16Bounds, state->codePoints, state->u8String);
    return state;
}

///
/// @brief Create state.
///
auto Text::CreateState(U16String const& text) const -> Shared<State>
{
    auto state = Shared<State>::Make();
    state->u16String = text;
    MakeStringDataU16(state->u8String, state->utf8Bounds, state->utf16Bounds, state->codePoints, state->u16String);
    return state;
}

///
/// @brief Get mutable pointer to state.
///
auto Text::GetMutableState() -> Pointer<State>
{
    if (_state)
    {
        if (_state.GetUseCount() != 1)
        {
            _state = Shared<State>::Make(*_state);
        }
        return _state.GetPointer();
    }
    return nullptr;
}

///
/// @brief Get immutable pointer to state.
///
auto Text::GetImmutableState() const -> Pointer<State const>
{
    return _state.GetPointer();
}
}

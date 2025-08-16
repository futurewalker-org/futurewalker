// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Text.Text.hpp"

#include "Futurewalker.Core.StringFunction.hpp"
#include "Futurewalker.Core.Range.hpp"

#include <unicode/utf8.h>
#include <unicode/utf16.h>

#include <vector>
#include <algorithm>

namespace FW_DETAIL_NS
{
namespace
{
auto Utf8Append(auto& buffer, int32_t c)
{
    auto i = int32_t();
    auto tmp = std::array<char8_t, 4>();
    U8_APPEND_UNSAFE(tmp.data(), i, c);
    buffer.append_range(std::ranges::subrange(tmp.begin(), tmp.begin() + i));
}

auto Utf16Append(auto& buffer, int32_t c)
{
    auto i = int32_t();
    auto tmp = std::array<char16_t, 2>();
    U16_APPEND_UNSAFE(tmp.data(), i, c);
    buffer.append_range(std::ranges::subrange(tmp.begin(), tmp.begin() + i));
}

auto MakeStringDataU8(String& utf8String, U16String& utf16String, std::vector<CodeUnit>& utf8Bounds, std::vector<CodeUnit>& utf16Bounds, CodePoint& codePoints, String const& source) -> Bool
{
    auto const view = source.GetView();
    auto const data = view.GetData();

    auto const utf8Str = static_cast<char8_t const*>(data);
    auto const utf8Length = static_cast<int32_t>(view.GetSize());

    utf8String = {};
    utf16String = {};

    utf8Bounds = {0};
    utf16Bounds = {0};

    codePoints = 0;

    if (utf8Length <= 0)
    {
        return true;
    }

    auto utf8Buffer = std::vector<char8_t>();
    auto utf16Buffer = std::vector<char16_t>();

    utf8Buffer.reserve(utf8Length);

    auto r = true;
    auto i = int32_t();
    while (i < utf8Length)
    {
        auto c = char32_t();
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
    utf16String = U16String(utf16Buffer.begin(), utf16Buffer.end());

    if (r)
    {
        assert(utf8String == source);
        return true;
    }
    return false;
}

auto MakeStringDataU16(String& utf8String, U16String& utf16String, std::vector<CodeUnit>& utf8Bounds, std::vector<CodeUnit>& utf16Bounds, CodePoint& codePoints, U16String const& source) -> Bool
{
    auto const view = source.GetView();
    auto const data = view.GetData();

    auto const utf16Str = static_cast<char16_t const*>(data);
    auto const utf16Length = static_cast<int32_t>(view.GetSize());

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
    utf16String = U16String(utf16Buffer.begin(), utf16Buffer.end());

    if (r)
    {
        assert(utf16String == source);
        return true;
    }
    return false;
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
        auto const begin = GetU8IndexByCodePointIndex(range.GetBegin());
        auto const end = GetU8IndexByCodePointIndex(range.GetEnd());
        return state->u8String.GetSubString(SInt64(begin), SInt64(end));
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
        auto const begin = GetU16IndexByCodePointIndex(range.GetBegin());
        auto const end = GetU16IndexByCodePointIndex(range.GetEnd());
        return state->u16String.GetSubString(SInt64(begin), SInt64(end));
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
    auto const b = Range<CodeUnit>::Clamp(range.GetBegin(), textRange);
    auto const e = Range<CodeUnit>::Clamp(range.GetEnd(), textRange);
    return Text(GetString().GetSubString(SInt64(b), SInt64(e)));
}

///
/// @brief Get sub-text.
///
auto Text::GetSubTextByU16Range(Range<CodeUnit> const& range) const -> Text
{
    // TODO: optimize
    auto const textRange = Range<CodeUnit>(0, GetU16CodeUnitCount());
    auto const b = Range<CodeUnit>::Clamp(range.GetBegin(), textRange);
    auto const e = Range<CodeUnit>::Clamp(range.GetEnd(), textRange);
    return Text(GetString().GetSubString(SInt64(b), SInt64(e)));
}

///
/// @brief Insert text at specificed position.
///
/// @param[in] range Range of text to be replaced.
/// @param[in] text Text to insert.
///
auto Text::Replace(Range<CodePoint> const& range, Text const& text) -> void
{
    if (range.GetBegin() < 0 || range.GetBegin() > range.GetEnd() || range.GetEnd() > GetCodePointCount())
    {
        return;
    }

    if (auto const selfState = GetMutableState())
    {
        auto const otherState = text.GetImmutableState();
        {
            auto const u8Begin = GetU8IndexByCodePointIndex(range.GetBegin());
            auto const u8End = GetU8IndexByCodePointIndex(range.GetEnd());
            auto const& u8String = otherState ? otherState->u8String : String();
            auto const& u8Bounds = otherState ? otherState->utf8Bounds : std::vector<CodeUnit>();
            ReplaceU8Core({u8Begin, u8End}, u8String, u8Bounds);
        }
        {
            auto const u16Begin = GetU16IndexByCodePointIndex(range.GetBegin());
            auto const u16End = GetU16IndexByCodePointIndex(range.GetEnd());
            auto const& u16String = otherState ? otherState->u16String : U16String();
            auto const& u16Bounds = otherState ? otherState->utf16Bounds : std::vector<CodeUnit>();
            ReplaceU16Core({u16Begin, u16End}, u16String, u16Bounds);
        }
        auto const codePointCountDifference = text.GetCodePointCount() - range.GetLength();
        selfState->codePoints += codePointCountDifference;
    }
    else
    {
        _state = text._state;
    }
}

auto Text::ReplaceU8Core(Range<CodeUnit> const& u8Range, String const& u8String, std::vector<CodeUnit> const& u8Bounds) -> void
{
    if (auto const selfState = GetMutableState())
    {
        selfState->u8String.Replace(static_cast<SInt64>(u8Range.GetBegin()), static_cast<SInt64>(u8Range.GetEnd()), u8String);

        auto const replaceBegin = IteratorAt(selfState->utf8Bounds, u8Range.GetBegin());
        auto const replaceBeginValue = *replaceBegin;
        auto const replaceEnd = IteratorAt(selfState->utf8Bounds, u8Range.GetEnd());
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

auto Text::ReplaceU16Core(Range<CodeUnit> const& u16Range, U16String const& u16String, std::vector<CodeUnit> const& u16Bounds) -> void
{
    if (auto const selfState = GetMutableState())
    {
        selfState->u16String.Replace(static_cast<SInt64>(u16Range.GetBegin()), static_cast<SInt64>(u16Range.GetLength()), u16String);

        auto const replaceBegin = IteratorAt(selfState->utf16Bounds, u16Range.GetBegin());
        auto const replaceBeginValue = *replaceBegin;
        auto const replaceEnd = IteratorAt(selfState->utf16Bounds, u16Range.GetEnd());
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
    MakeStringDataU8(state->u8String, state->u16String, state->utf8Bounds, state->utf16Bounds, state->codePoints, text);
    return state;
}

///
/// @brief Create state.
///
auto Text::CreateState(U16String const& text) const -> Shared<State>
{
    auto state = Shared<State>::Make();
    MakeStringDataU16(state->u8String, state->u16String, state->utf8Bounds, state->utf16Bounds, state->codePoints, text);
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

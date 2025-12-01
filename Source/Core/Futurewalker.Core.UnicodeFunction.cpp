// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Core.UnicodeFunction.hpp"

#include <unicode/utf16.h>
#include <unicode/utf8.h>

namespace FW_DETAIL_NS
{
///
/// @brief Get code units for code point.
///
/// @param[out] codeUnits The span to store the code units.
/// @param[in] codePoint The code point.
///
/// @return The number of code units written, or nullopt if invalid code point.
///
auto UnicodeFunction::GetCodeUnits(std::span<char8_t, MaxU8CodeUnitCountForCodePoint> codeUnits, char32_t const codePoint) -> Optional<SInt64>
{
    auto const s = codeUnits.data();
    auto const c = UChar32(codePoint);
    auto i = int32_t(0);
    auto isError = UBool();
    U8_APPEND(s, i, MaxU8CodeUnitCountForCodePoint, c, isError);
    if (!isError)
    {
        return i;
    }
    return {};
}

///
/// @brief Get code units for code point.
///
/// @param[out] codeUnits The span to store the code units.
/// @param[in] codePoint The code point.
///
/// @return The number of code units written, or nullopt if invalid code point.
///
auto UnicodeFunction::GetCodeUnits(std::span<char16_t, MaxU16CodeUnitCountForCodePoint> codeUnits, char32_t const codePoint) -> Optional<SInt64>
{
    auto const s = codeUnits.data();
    auto const c = UChar32(codePoint);
    auto i = int32_t(0);
    auto isError = UBool();
    U16_APPEND(s, i, MaxU16CodeUnitCountForCodePoint, c, isError);
    if (!isError)
    {
        return i;
    }
    return {};
}

///
/// @brief Get code units for code point, or U+FFFD if invalid.
///
/// @param[out] codeUnits The span to store the code units.
/// @param[in] codePoint The code point.
///
/// @return The number of code units written.
///
auto UnicodeFunction::GetCodeUnitsOrFFFD(std::span<char8_t, MaxU8CodeUnitCountForCodePoint> codeUnits, char32_t const codePoint) -> SInt64
{
    if (auto const end = GetCodeUnits(codeUnits, codePoint))
    {
        return *end;
    }
    return GetCodeUnitsUnsafe(codeUnits, 0xfffd);
}

///
/// @brief Get code units for code point, or U+FFFD if invalid.
///
/// @param[out] codeUnits The span to store the code units.
/// @param[in] codePoint The code point.
///
/// @return The number of code units written.
///
auto UnicodeFunction::GetCodeUnitsOrFFFD(std::span<char16_t, MaxU16CodeUnitCountForCodePoint> codeUnits, char32_t const codePoint) -> SInt64
{
    if (auto const end = GetCodeUnits(codeUnits, codePoint))
    {
        return *end;
    }
    return GetCodeUnitsUnsafe(codeUnits, 0xfffd);
}

auto UnicodeFunction::GetCodeUnitsUnsafe(std::span<char8_t, MaxU8CodeUnitCountForCodePoint> codeUnits, char32_t const codePoint) -> SInt64
{
    auto const s = codeUnits.data();
    auto const c = UChar32(codePoint);
    auto i = int32_t(0);
    U8_APPEND_UNSAFE(s, i, c);
    return i;
}

///
/// @brief Get code units for code point without validation.
///
/// @param[out] codeUnits The span to store the code units.
/// @param[in] codePoint The code point.
///
/// @return The number of code units written. 
///
auto UnicodeFunction::GetCodeUnitsUnsafe(std::span<char16_t, MaxU16CodeUnitCountForCodePoint> codeUnits, char32_t const codePoint) -> SInt64
{
    auto const s = codeUnits.data();
    auto const c = UChar32(codePoint);
    auto i = int32_t(0);
    U16_APPEND_UNSAFE(s, i, c);
    return i;
}

///
/// @brief Get code point from code units.
///
/// @param[in] data The pointer to the code units.
/// @param[in] index The index of the code unit.
/// @param[in] first The first index of the code units.
/// @param[in] last The last index of the code units.
///
/// @return The code point, or nullopt if invalid.
///
auto UnicodeFunction::GetCodePoint(Pointer<char8_t const> data, SInt64 const& index, SInt64 const& first, SInt64 const& last) -> Optional<char32_t>
{
    auto const s = reinterpret_cast<uint8_t const*>(static_cast<char8_t const*>(data));
    auto const i = static_cast<int32_t>(index);
    auto const start = static_cast<int32_t>(first);
    auto const length = static_cast<int32_t>(last);
    auto c = UChar32();
    U8_GET(s, start, i, length, c);
    if (c >= 0)
    {
        return c;
    }
    return {};
}

///
/// @brief Get code point from code units.
///
/// @param[in] data The pointer to the code units.
/// @param[in] index The index of the code unit.
/// @param[in] min The first index of the code units.
/// @param[in] last The last index of the code units.
///
/// @return The code point, or nullopt if invalid.
///
auto UnicodeFunction::GetCodePoint(Pointer<char16_t const> data, SInt64 const& index, SInt64 const& min, SInt64 const& last) -> Optional<char32_t>
{
    auto const s = static_cast<char16_t const*>(data);
    auto const i = static_cast<int32_t>(index);
    auto const start = static_cast<int32_t>(min);
    auto const length = static_cast<int32_t>(last);
    auto c = UChar32();
    U16_GET(s, start, i, length, c);
    if (c >= 0)
    {
        return c;
    }
    return {};
}

///
/// @brief Get code point from code units, or U+FFFD if invalid.
///
/// @param[in] data The pointer to the code units.
/// @param[in] index The index of the code unit.
/// @param[in] first The first index of the code units.
/// @param[in] last The last index of the code units.
///
/// @return The code point, or U+FFFD if invalid.
///
auto UnicodeFunction::GetCodePointOrFFFD(Pointer<char8_t const> data, SInt64 const& index, SInt64 const& first, SInt64 const& last) -> char32_t
{
    auto const s = reinterpret_cast<uint8_t const*>(static_cast<char8_t const*>(data));
    auto const i = static_cast<int32_t>(index);
    auto const start = static_cast<int32_t>(first);
    auto const length = static_cast<int32_t>(last);
    auto c = UChar32();
    U8_GET_OR_FFFD(s, start, i, length, c);
    return c;
}

///
/// @brief Get code point from code units, or U+FFFD if invalid.
///
/// @param[in] data The pointer to the code units.
/// @param[in] index The index of the code unit.
/// @param[in] first The first index of the code units.
/// @param[in] last The last index of the code units.
///
/// @return The code point, or U+FFFD if invalid.
///
auto UnicodeFunction::GetCodePointOrFFFD(Pointer<char16_t const> data, SInt64 const& index, SInt64 const& first, SInt64 const& last) -> char32_t
{
    auto const s = static_cast<char16_t const*>(data);
    auto const i = static_cast<int32_t>(index);
    auto const start = static_cast<int32_t>(first);
    auto const length = static_cast<int32_t>(last);
    auto c = UChar32();
    U16_GET_OR_FFFD(s, start, i, length, c);
    return c;
}

///
/// @brief Get code point and next index from code units.
///
/// @param[in] data The pointer to the code units.
/// @param[in] index The index of the code unit.
/// @param[in] last The last index of the code units.
///
/// @return The (code point, next index) pair.
///
auto UnicodeFunction::GetCodePointAndNextIndex(Pointer<char8_t const> data, SInt64 const& index, SInt64 const& last) -> std::pair<Optional<char32_t>, SInt64>
{
    auto const s = reinterpret_cast<uint8_t const*>(static_cast<char8_t const*>(data));
    auto const length = static_cast<int32_t>(last);
    auto i = static_cast<int32_t>(index);
    auto c = UChar32();
    U8_NEXT(s, i, length, c);
    if (c >= 0)
    {
        return {c, i};
    }
    return {{}, i};
}

///
/// @brief Get code point and next index from code units.
///
/// @param[in] data The pointer to the code units.
/// @param[in] index The index of the code unit.
/// @param[in] last The last index of the code units.
///
/// @return The (code point, next index) pair.
///
auto UnicodeFunction::GetCodePointAndNextIndex(Pointer<char16_t const> data, SInt64 const& index, SInt64 const& last) -> std::pair<Optional<char32_t>, SInt64>
{
    auto const s = static_cast<char16_t const*>(data);
    auto const length = static_cast<int32_t>(last);
    auto i = static_cast<int32_t>(index);
    auto c = UChar32();
    U16_NEXT(s, i, length, c);
    if (c >= 0)
    {
        return {c, i};
    }
    return {{}, i};
}

///
/// @brief Get code point and next index from code units, or U+FFFD if invalid.
///
/// @param[in] data The pointer to the code units.
/// @param[in] index The index of the code unit.
/// @param[in] last The last index of the code units.
///
/// @return The (code point, next index) pair.
///
auto UnicodeFunction::GetCodePointOrFFFDAndNextIndex(Pointer<char8_t const> data, SInt64 const& index, SInt64 const& last) -> std::pair<char32_t, SInt64>
{
    auto const s = reinterpret_cast<uint8_t const*>(static_cast<char8_t const*>(data));
    auto const length = static_cast<int32_t>(last);
    auto i = static_cast<int32_t>(index);
    auto c = UChar32();
    U8_NEXT_OR_FFFD(s, i, length, c);
    return {c, i};
}

///
/// @brief Get code point and next index from code units, or U+FFFD if invalid.
///
/// @param[in] data The pointer to the code units.
/// @param[in] index The index of the code unit.
/// @param[in] last The last index of the code units.
///
/// @return The (code point, next index) pair.
///
auto UnicodeFunction::GetCodePointOrFFFDAndNextIndex(Pointer<char16_t const> data, SInt64 const& index, SInt64 const& last) -> std::pair<char32_t, SInt64>
{
    auto const s = static_cast<char16_t const*>(data);
    auto const length = static_cast<int32_t>(last);
    auto i = static_cast<int32_t>(index);
    auto c = UChar32();
    U16_NEXT_OR_FFFD(s, i, length, c);
    return {c, i};

}

///
/// @brief Adjust index to the start of a code point.
///
/// @param[in] data The pointer to the code units.
/// @param[in] end The end index of the code units.
/// @param[in] index The index to synchronize.
///
/// @return The adjusted index.
///
/// @note data should contain valid UTF-8 string.
///
auto UnicodeFunction::GetCodePointStartUnsafe(Pointer<char8_t const> data, SInt64 const& end, SInt64 const& index) -> SInt64
{
    if (index <= 0)
    {
        return 0;
    }
    else if (end <= index)
    {
        return end;
    }
    auto const s = reinterpret_cast<uint8_t const*>(static_cast<char8_t const*>(data));
    auto i = static_cast<int32_t>(index);
    U8_SET_CP_START_UNSAFE(s, i);
    return i;
}

///
/// @brief Adjust index to the start of a code point.
///
/// @param[in] data The pointer to the code units.
/// @param[in] end The end index of the code units.
/// @param[in] index The index to synchronize.
///
/// @return The adjusted index.
///
/// @note data should contain valid UTF-16 string.
///
auto UnicodeFunction::GetCodePointStartUnsafe(Pointer<char16_t const> data, SInt64 const& end, SInt64 const& index) -> SInt64
{
    if (index <= 0)
    {
        return 0;
    }
    else if (end <= index)
    {
        return end;
    }
    auto const s = static_cast<char16_t const*>(data);
    auto i = static_cast<int32_t>(index);
    U16_SET_CP_START_UNSAFE(s, i);
    return i;
}

///
/// @brief Adjust index to the end of a code point.
///
/// @param[in] data The pointer to the code units.
/// @param[in] end The end index of the code units.
/// @param[in] index The index to synchronize.
///
/// @return The adjusted index.
///
/// @note data should contain valid UTF-8 string.
///
auto UnicodeFunction::GetCodePointLimitUnsafe(Pointer<char8_t const> data, SInt64 const& end, SInt64 const& index) -> SInt64
{
    if (index <= 0)
    {
        return 0;
    }
    else if (end <= index)
    {
        return end;
    }
    auto const s = reinterpret_cast<uint8_t const*>(static_cast<char8_t const*>(data));
    auto i = static_cast<int32_t>(index);
    U8_SET_CP_LIMIT_UNSAFE(s, i);
    return i;
}

///
/// @brief Adjust index to the end of a code point.
///
/// @param[in] data The pointer to the code units.
/// @param[in] end The end index of the code units.
/// @param[in] index The index to synchronize.
///
/// @return The adjusted index.
///
/// @note data should contain valid UTF-16 string.
///
auto UnicodeFunction::GetCodePointLimitUnsafe(Pointer<char16_t const> data, SInt64 const& end, SInt64 const& index) -> SInt64
{
    if (index <= 0)
    {
        return 0;
    }
    else if (end <= index)
    {
        return end;
    }
    auto const s = static_cast<char16_t const*>(data);
    auto i = static_cast<int32_t>(index);
    U16_SET_CP_LIMIT_UNSAFE(s, i);
    return i;
}

///
/// @brief Get next UTF-8 code point index.
///
/// @param[in] data Pointer to UTF-8 string data.  
/// @param[in] index Current index.
/// @param[in] last Maximum index (one past the end).
/// @param[in] n Number of code points to move forward.
///
/// @return New index after moving forward n code points.
///
auto UnicodeFunction::GetNextCodePointIndex(Pointer<char8_t const> data, SInt64 const& index, SInt64 const& last, SInt64 const& n) -> SInt64
{
    if (n > 0)
    {
        auto const s = reinterpret_cast<uint8_t const*>(static_cast<char8_t const*>(data));
        auto i = static_cast<int32_t>(index);
        auto const num = static_cast<int32_t>(n);
        auto const length = static_cast<int32_t>(last);
        U8_FWD_N(s, i, length, num);
        return i;
    }
    return index;
}

///
/// @brief Get next UTF-16 code point index.
///
/// @param[in] data Pointer to UTF-16 string data.
/// @param[in] index Current index.
/// @param[in] last Maximum index (one past the end).
/// @param[in] n Number of code points to move forward.
///
/// @return New index after moving forward n code points.
///
auto UnicodeFunction::GetNextCodePointIndex(Pointer<char16_t const> data, SInt64 const& index, SInt64 const& last, SInt64 const& n) -> SInt64
{
    if (n > 0)
    {
        auto const s = static_cast<char16_t const*>(data);
        auto i = static_cast<int32_t>(index);
        auto const num = static_cast<int32_t>(n);
        auto const length = static_cast<int32_t>(last);
        U16_FWD_N(s, i, length, num);
        return i;
    }
    return index;
}

///
/// @brief Get previous UTF-8 code point index.
///
/// @param[in] data The pointer to the code units.
/// @param[in] index The index of the code unit.
/// @param[in] begin The beginning index of the code units.
/// @param[in] n Number of code points to move backward.
///
/// @return The new index after moving backward n code points.
///
auto UnicodeFunction::GetPrevCodePointIndex(Pointer<char8_t const> data, SInt64 const& index, SInt64 const& begin, SInt64 const& n) -> SInt64
{
    if (n < 0)
    {
        auto const s = reinterpret_cast<uint8_t const*>(static_cast<char8_t const*>(data));
        auto i = static_cast<int32_t>(index);
        auto const num = static_cast<int32_t>(n);
        auto const start = static_cast<int32_t>(begin);
        U8_BACK_N(s, start, i, num);
        return i;
    }
    return index;
}

///
/// @brief Get previous UTF-16 code point index.
///
/// @param[in] data Pointer to UTF-16 string data.
/// @param[in] index Current index.
/// @param[in] min Minimum index.
/// @param[in] n Number of code points to move backward.
///
/// @return New index after moving backward n code points.
///
auto UnicodeFunction::GetPrevCodePointIndex(Pointer<char16_t const> data, SInt64 const& index, SInt64 const& min, SInt64 const& n) -> SInt64
{
    if (n < 0)
    {
        auto const s = static_cast<char16_t const*>(data);
        auto i = static_cast<int32_t>(index);
        auto const num = static_cast<int32_t>(n);
        auto const start = static_cast<int32_t>(min);
        U16_BACK_N(s, start, i, num);
        return i;
    }
    return index;
}
}

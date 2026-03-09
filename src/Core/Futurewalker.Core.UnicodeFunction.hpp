// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.NonConstructible.hpp"
#include "Futurewalker.Core.Pointer.hpp"
#include "Futurewalker.Core.Optional.hpp"

#include <utility>
#include <span>

namespace FW_DETAIL_NS
{
///
/// @brief Functions for Unicode characters.
///
struct UnicodeFunction : NonConstructible
{
    static constexpr auto MaxU8CodeUnitCountForCodePoint = 4;
    static constexpr auto MaxU16CodeUnitCountForCodePoint = 2;

    static auto GetCodeUnits(std::span<char8_t, MaxU8CodeUnitCountForCodePoint> codeUnits, char32_t const codePoint) -> Optional<SInt64>;
    static auto GetCodeUnits(std::span<char16_t, MaxU16CodeUnitCountForCodePoint> codeUnits, char32_t const codePoint) -> Optional<SInt64>;

    static auto GetCodeUnitsOrFFFD(std::span<char8_t, MaxU8CodeUnitCountForCodePoint> codeUnits, char32_t const codePoint) -> SInt64;
    static auto GetCodeUnitsOrFFFD(std::span<char16_t, MaxU16CodeUnitCountForCodePoint> codeUnits, char32_t const codePoint) -> SInt64;

    static auto GetCodeUnitsUnsafe(std::span<char8_t, MaxU8CodeUnitCountForCodePoint> codeUnits, char32_t const codePoint) -> SInt64;
    static auto GetCodeUnitsUnsafe(std::span<char16_t, MaxU16CodeUnitCountForCodePoint> codeUnits, char32_t const codePoint) -> SInt64;

    static auto GetCodePoint(Pointer<char8_t const> data, SInt64 const& index, SInt64 const& first, SInt64 const& last) -> Optional<char32_t>;
    static auto GetCodePoint(Pointer<char16_t const> data, SInt64 const& index, SInt64 const& first, SInt64 const& last) -> Optional<char32_t>;

    static auto GetCodePointOrFFFD(Pointer<char8_t const> data, SInt64 const& index, SInt64 const& first, SInt64 const& last) -> char32_t;
    static auto GetCodePointOrFFFD(Pointer<char16_t const> data, SInt64 const& index, SInt64 const& first, SInt64 const& last) -> char32_t;

    static auto GetCodePointAndNextIndex(Pointer<char8_t const> data, SInt64 const& index, SInt64 const& last) -> std::pair<Optional<char32_t>, SInt64>;
    static auto GetCodePointAndNextIndex(Pointer<char16_t const> data, SInt64 const& index, SInt64 const& last) -> std::pair<Optional<char32_t>, SInt64>;

    static auto GetCodePointOrFFFDAndNextIndex(Pointer<char8_t const> data, SInt64 const& index, SInt64 const& last) -> std::pair<char32_t, SInt64>;
    static auto GetCodePointOrFFFDAndNextIndex(Pointer<char16_t const> data, SInt64 const& index, SInt64 const& last) -> std::pair<char32_t, SInt64>;

    static auto GetCodePointStartUnsafe(Pointer<char16_t const> data, SInt64 const& last, SInt64 const& index) -> SInt64;
    static auto GetCodePointStartUnsafe(Pointer<char8_t const> data, SInt64 const& last, SInt64 const& index) -> SInt64;

    static auto GetCodePointLimitUnsafe(Pointer<char16_t const> data, SInt64 const& last, SInt64 const& index) -> SInt64;
    static auto GetCodePointLimitUnsafe(Pointer<char8_t const> data, SInt64 const& last, SInt64 const& index) -> SInt64;

    static auto GetNextCodePointIndex(Pointer<char8_t const> data, SInt64 const& index, SInt64 const& last, SInt64 const& n = 1) -> SInt64;
    static auto GetNextCodePointIndex(Pointer<char16_t const> data, SInt64 const& index, SInt64 const& last, SInt64 const& n = 1) -> SInt64;

    static auto GetPrevCodePointIndex(Pointer<char8_t const> data, SInt64 const& index, SInt64 const& first, SInt64 const& n = 1) -> SInt64;
    static auto GetPrevCodePointIndex(Pointer<char16_t const> data, SInt64 const& index, SInt64 const& first, SInt64 const& n = 1) -> SInt64;

    static auto IsSpace(char32_t const codePoint) -> Bool;
    static auto IsBreak(char32_t const codePoint) -> Bool;
};
}
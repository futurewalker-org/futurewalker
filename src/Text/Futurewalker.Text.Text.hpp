// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Text.Prelude.hpp"

#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.U16String.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Pointer.hpp"
#include "Futurewalker.Core.Range.hpp"

#include <string>
#include <vector>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Represents valid Unicode text and its properties.
///
class Text
{
public:
    Text() = default;
    explicit Text(String const& text);
    explicit Text(U16String const& text);

    auto GetString() const -> String;
    auto GetString(Range<CodePoint> range) const -> String;
    auto SetString(String const& text) -> void;

    auto GetU16String() const -> U16String;
    auto GetU16String(Range<CodePoint> range) const -> U16String;
    auto SetU16String(U16String const& text) -> void;

    auto GetCodePointCount() const -> CodePoint;
    auto GetU8CodeUnitCount() const -> CodeUnit;
    auto GetU16CodeUnitCount() const -> CodeUnit;

    auto GetCodePointIndexByU8Index(CodeUnit const index) const -> CodePoint;
    auto GetCodePointIndexByU16Index(CodeUnit const index) const -> CodePoint;

    auto GetU8IndexByCodePointIndex(CodePoint const index) const -> CodeUnit;
    auto GetU16IndexByCodePointIndex(CodePoint const index) const -> CodeUnit;

    auto GetSubText(Range<CodePoint> const& range) const -> Text;
    auto GetSubTextByU8Range(Range<CodeUnit> const& range) const -> Text;
    auto GetSubTextByU16Range(Range<CodeUnit> const& range) const -> Text;

    auto Replace(Range<CodePoint> const& range, Text const& text) -> void;

private:
    auto ReplaceU8Core(Range<CodePoint> const& range, String const& u8String, std::vector<CodeUnit> const& u8Bounds) -> void;
    auto ReplaceU16Core(Range<CodePoint> const& range, U16String const& u16String, std::vector<CodeUnit> const& u16Bounds) -> void;

private:
    struct State
    {
        String u8String;
        U16String u16String;
        std::vector<CodeUnit> utf8Bounds;
        std::vector<CodeUnit> utf16Bounds;
        CodePoint codePoints = 0;
    };
    auto CreateState(String const& text) const -> Shared<State>;
    auto CreateState(U16String const& text) const -> Shared<State>;
    auto GetMutableState() -> Pointer<State>;
    auto GetImmutableState() const -> Pointer<State const>;

    Shared<State> _state;
};
}
}

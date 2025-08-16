// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Text.TextInputStateType.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Text.Text.hpp"

#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.U16String.hpp"
#include "Futurewalker.Core.Range.hpp"
#include "Futurewalker.Core.Function.hpp"

#include <string>
#include <vector>
#include <utility>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Text input state.
///
class TextInputState final
{
public:
    struct Delegate
    {
        Function<Bool(String const& text)> beforeInsertText;
        Function<Bool(CodePoint before, CodePoint after)> beforeDeleteSurroundingText;
        Function<void(CodeUnit u16Begin, CodeUnit oldU16End, CodeUnit newU16End)> onTextChange;
        Function<void()> onSelectionChange;
    };
    TextInputState(Delegate const& delegate);

    auto GetText() const -> Text;
    auto SetText(Text const& text) -> void;

    auto GetString() const -> String;
    auto GetString(Range<CodePoint> range) const -> String;
    auto GetStringRange() const -> Range<CodePoint>;

    auto GetSelectedRange() const -> Range<CodePoint>;
    auto GetNormalizedSelectedRange() const -> Range<CodePoint>;
    auto SetSelectedRange(Range<CodePoint> const& text) -> Bool;

    auto GetComposingRange() const -> Range<CodePoint>;
    auto SetComposingRange(Range<CodePoint> const& text) -> Bool;

    auto InsertText(String const& text, CodePoint caretPosition, Bool anticipated) -> void;
    auto DeleteSurroundingText(CodePoint before, CodePoint after, Bool anticipated) -> void;

public:
    auto GetU16String() const -> U16String;
    auto GetU16String(Range<CodeUnit> range) const -> U16String;
    auto GetU16StringRange() const -> Range<CodeUnit>;
    auto GetU16SelectedRange() const -> Range<CodeUnit>;
    auto GetU16NormalizedSelectedRange() const -> Range<CodeUnit>;
    auto SetU16SelectedRange(Range<CodeUnit> range, Bool anticipated) -> void;
    auto GetU16ComposingRange() const -> Range<CodeUnit>;
    auto SetU16ComposingRange(Range<CodeUnit> range) -> void;

    auto InsertU16Text(U16StringView text, CodePoint caretPosition, Bool anticipated) -> void;

    auto GetRangeFromU16Range(Range<CodeUnit> range) const -> Range<CodePoint>;
    auto GetRangeFromU8Range(Range<CodeUnit> range) const -> Range<CodePoint>;

    auto GetU16RangeFromRange(Range<CodePoint> range) const -> Range<CodeUnit>;
    auto GetU8RangeFromRange(Range<CodePoint> range) const -> Range<CodeUnit>;

private:
    auto FindCodePointFromU8Index(CodeUnit index) const -> CodePoint;
    auto FindCodePointFromU16Index(CodeUnit index) const -> CodePoint;
    auto FindU8IndexFromCodePoint(CodePoint codePoint) const -> CodeUnit;
    auto FindU8RangeFromCodePointRange(Range<CodePoint> range) const -> Range<CodeUnit>;
    auto FindU16IndexFromCodePoint(CodePoint codePoint) const -> CodeUnit;
    auto FindU16RangeFromCodePointRange(Range<CodePoint> range) const -> Range<CodeUnit>;

    auto InsertTextCore(CodePoint caretPosition, Bool anticipated, Text const& text) -> void;

private:
    auto BeforeInsertText(String const& text) -> Bool;
    auto BeforeDeleteSurroundingText(CodePoint before, CodePoint after) -> Bool;
    auto OnTextChange(CodeUnit u16Begin, CodeUnit oldU16End, CodeUnit newU16End) -> void;
    auto OnSelectionChange() -> void;

private:
    Delegate _delegate;
    Text _text;
    // NOTE: Bad IMEs might set cursor between surrogate pairs but we currently cannot handle that.
    Range<CodePoint> _selectedRange;
    Range<CodePoint> _composingRange;
};
}
}

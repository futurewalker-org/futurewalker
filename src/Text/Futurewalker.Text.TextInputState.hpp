// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Text.TextInputStateType.hpp"
#include "Futurewalker.Text.TextSelectionDirection.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Text.Text.hpp"

#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.U16String.hpp"
#include "Futurewalker.Core.Range.hpp"
#include "Futurewalker.Core.Function.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Text input state.
///
class TextInputState : NonCopyable
{
public:
    struct Delegate
    {
        Function<Bool(String const& text, Bool cancellable)> beforeInsertText;
        Function<void(String const& text)> insertText;
        Function<Bool(Bool cancellable)> beforeInsertLineBreak;
        Function<void()> insertLineBreak;
        Function<Bool(CodePoint before, CodePoint after, Bool cancellable)> beforeDeleteSurroundingText;
        Function<void(CodePoint before, CodePoint after)> deleteSurroundingText;
        Function<void(CodeUnit u16Begin, CodeUnit oldU16End, CodeUnit newU16End)> onTextChange;
        Function<void()> onSelectionChange;
    };
    TextInputState(Delegate const& delegate);

    auto GetText() const -> Text;
    auto SetText(Text const& text) -> void;

    auto GetString() const -> String;
    auto GetString(Range<CodePoint> const&range) const -> String;
    auto GetStringRange() const -> Range<CodePoint>;

    auto GetSelectionRange() const -> Range<CodePoint>;
    auto GetSelectionDirection() const -> TextSelectionDirection;
    auto SetSelectionRange(Range<CodePoint> const& text, TextSelectionDirection const direction) -> Bool;

    auto GetComposingRange() const -> Range<CodePoint>;
    auto SetComposingRange(Range<CodePoint> const& text) -> Bool;

    auto InsertText(String const& text, CodePoint caretPosition, Bool anticipated, Bool cancellable) -> void;
    auto InsertLineBreak(Bool anticipated, Bool cancellable) -> void;
    auto DeleteSurroundingText(CodePoint before, CodePoint after, Bool anticipated, Bool cancellable) -> void;

public:
    auto GetU16String() const -> U16String;
    auto GetU16String(Range<CodeUnit> range) const -> U16String;
    auto GetU16StringRange() const -> Range<CodeUnit>;
    auto GetU16SelectionRange() const -> Range<CodeUnit>;
    auto SetU16SelectionRange(Range<CodeUnit> const& range, TextSelectionDirection const direction, Bool anticipated) -> void;
    auto GetU16ComposingRange() const -> Range<CodeUnit>;
    auto SetU16ComposingRange(Range<CodeUnit> const& range) -> void;

    auto InsertU16Text(U16StringView text, CodePoint caretPosition, Bool anticipated, Bool cancellable) -> void;

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

    auto InsertTextCore(CodePoint caretPosition, Bool anticipated, Bool cancellable, Text const& text) -> void;

private:
    auto BeforeInsertText(String const& text, Bool cancellable) -> Bool;
    auto InsertText(String const& text) -> void;
    auto BeforeInsertLineBreak(Bool cancellable) -> Bool;
    auto InsertLineBreak() -> void;
    auto BeforeDeleteSurroundingText(CodePoint before, CodePoint after, Bool cancellable) -> Bool;
    auto DeleteSurroundingText(CodePoint before, CodePoint after) -> void;
    auto OnTextChange(CodeUnit u16Begin, CodeUnit oldU16End, CodeUnit newU16End) -> void;
    auto OnSelectionChange() -> void;

private:
    Delegate _delegate;
    Text _text;
    TextSelectionDirection _selectionDirection = TextSelectionDirection::Forward;
    // NOTE: Bad IMEs might set cursor between surrogate pairs but we currently cannot handle that.
    Range<CodePoint> _selectionRange;
    Range<CodePoint> _composingRange;
};
}
}

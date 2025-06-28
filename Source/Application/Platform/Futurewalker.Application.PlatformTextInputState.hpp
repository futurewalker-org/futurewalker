// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformTextInputStateType.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.String.hpp"
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
class PlatformTextInputState final
{
public:
    struct Delegate
    {
        Function<Bool(String const& text)> beforeInsertText;
        Function<Bool(CodePoint before, CodePoint after)> beforeDeleteSurroundingText;
        Function<void(CodeUnit u16Begin, CodeUnit oldU16End, CodeUnit newU16End)> onTextChange;
        Function<void()> onSelectionChange;
    };
    PlatformTextInputState(Delegate const& delegate);

    auto GetText() const -> String;
    auto GetText(Range<CodePoint> range) const -> String;
    auto GetTextRange() const -> Range<CodePoint>;
    auto SetText(String const& text) -> Bool;

    auto GetSelectedRange() const -> Range<CodePoint>;
    auto GetNormalizedSelectedRange() const -> Range<CodePoint>;
    auto SetSelectedRange(Range<CodePoint> const& text) -> Bool;

    auto GetComposingRange() const -> Range<CodePoint>;
    auto SetComposingRange(Range<CodePoint> const& text) -> Bool;

    auto InsertText(String const& text, CodePoint caretPosition, Bool anticipated) -> void;
    auto DeleteSurroundingText(CodePoint before, CodePoint after, Bool anticipated) -> void;

    auto GetU16Text() const -> std::u16string_view;
    auto GetU16Text(Range<CodeUnit> range) const -> std::u16string_view;
    auto GetU16TextRange() const -> Range<CodeUnit>;
    auto GetU16SelectedRange() const -> Range<CodeUnit>;
    auto GetU16NormalizedSelectedRange() const -> Range<CodeUnit>;
    auto SetU16SelectedRange(Range<CodeUnit> range, Bool anticipated) -> void;
    auto GetU16ComposingRange() const -> Range<CodeUnit>;
    auto SetU16ComposingRange(Range<CodeUnit> range) -> void;

    auto InsertU16Text(std::u16string_view text, CodePoint caretPosition, Bool anticipated) -> void;

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

    auto InsertTextData(CodePoint caretPosition, Bool anticipated, StringView utf8Text, std::span<CodeUnit const> utf8Bounds, std::u16string_view utf16Text, std::span<CodeUnit const> utf16Bounds, CodePoint codePoints) -> void;
    auto InsertTextCore(StringView utf8Text, std::span<CodeUnit const> utf8Bounds, std::u16string_view utf16Text, std::span<CodeUnit const> utf16Bounds, Range<CodePoint> range) -> void;
    auto InsertU8TextCore(StringView utf8Text, std::span<CodeUnit const> utf8Bounds, Range<CodePoint> range) -> void;
    auto InsertU16TextCore(std::u16string_view utf16String, std::span<CodeUnit const> utf16Bounds, Range<CodePoint> range) -> void;

    auto DeleteTextCore(Range<CodePoint> range) -> void;

private:
    auto BeforeInsertText(String const& text) -> Bool;
    auto BeforeDeleteSurroundingText(CodePoint before, CodePoint after) -> Bool;
    auto OnTextChange(CodeUnit u16Begin, CodeUnit oldU16End, CodeUnit newU16End) -> void;
    auto OnSelectionChange() -> void;

private:
    Delegate _delegate;
    String _utf8Text;
    std::u16string _utf16Text;
    // Code point boundaries.
    std::vector<CodeUnit> _utf8Bounds;
    std::vector<CodeUnit> _utf16Bounds;
    // NOTE: Bad IMEs might set cursor between surrogate pairs but we currently cannot handle that.
    Range<CodePoint> _selectedRange;
    Range<CodePoint> _composingRange;
};
}
}

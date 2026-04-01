// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Mac.PlatformInputEditableMacType.hpp"
#include "Futurewalker.Application.PlatformInputEditable.hpp"

#include "Futurewalker.Text.TextInputState.hpp"

#include "Futurewalker.Graphics.TextLayoutInfo.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.PassKey.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformInputEditableMac : public PlatformInputEditable
{
public:
    static auto Make(Delegate const& delegate) -> Shared<PlatformInputEditableMac>;

    PlatformInputEditableMac(PassKey<PlatformInputEditable> key, Delegate const& delegate);

    auto SetTextInputContext(NSTextInputContext* textInputContext) -> void;

    auto GetText() const -> Text override;
    auto SetText(Text const& text) -> void override;

    auto GetString(Range<CodePoint> const& range) const -> String override;
    auto GetStringRange() const -> Range<CodePoint> override;

    auto GetSelectionDirection() const -> TextSelectionDirection override;
    auto GetSelectionRange() const -> Range<CodePoint> override;
    auto SetSelectionRange(Range<CodePoint> const& range, TextSelectionDirection direction) -> void override;

    auto GetComposingRange() const -> Range<CodePoint> override;
    auto SetComposingRange(Range<CodePoint> const& range) -> void override;

    auto GetLayoutRect() const -> Rect<Dp> override;
    auto SetLayoutRect(Rect<Dp> const& rect) -> void override;

    auto GetLayoutInfo() const -> Graphics::TextLayoutInfo override;
    auto SetLayoutInfo(Graphics::TextLayoutInfo const& layoutInfo) -> void override;

    auto InsertText(String const& text, CodePoint caretPosition) -> void override;
    auto InsertLineBreak() -> void override;
    auto DeleteSurroundingText(CodePoint before, CodePoint after) -> void override;

public:
    auto GetU16Text() const -> U16String;
    auto GetU16Text(Range<CodeUnit> range) const -> U16String;
    auto GetU16TextRange() const -> Range<CodeUnit>;
    auto GetU16SelectionRange() const -> Range<CodeUnit>;
    auto SetU16SelectionRange(Range<CodeUnit> range, TextSelectionDirection direction, Bool anticipated) -> void;
    auto GetU16ComposingRange() const -> Range<CodeUnit>;
    auto SetU16ComposingRange(Range<CodeUnit> range) -> void;
    auto GetRangeFromU16Range(Range<CodeUnit> range) const -> Range<CodePoint>;
    auto GetU16RangeFromRange(Range<CodePoint> range) const -> Range<CodeUnit>;
    auto InsertU16Text(U16StringView const text, CodePoint caretPosition, Bool anticipated, Bool cancellable) -> void;

private:
    auto InternalBeforeInsertText(String const& text, Bool cancellable) -> Bool;
    auto InternalInsertText(String const& text) -> void;
    auto InternalBeforeInsertLineBreak(Bool cancellable) -> Bool;
    auto InternalInsertLineBreak() -> void;
    auto InternalBeforeDeleteSurroundingText(CodePoint before, CodePoint after, Bool cancellable) -> Bool;
    auto InternalDeleteSurroundingText(CodePoint before, CodePoint after) -> void;
    auto InternalOnTextChange(CodeUnit u16OldBegin, CodeUnit u16OldEnd, CodeUnit u16NewEnd) -> void;
    auto InternalOnSelectionChange() -> void;

private:
    Rect<Dp> _layoutRect;
    TextInputState _inputState;
    Graphics::TextLayoutInfo _layoutInfo;
    __weak NSTextInputContext* _textInputContext = nil;
};
}
}

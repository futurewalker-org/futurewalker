// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformInputEditableWinType.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodTextStoreWinType.hpp"
#include "Futurewalker.Application.PlatformInputEditable.hpp"

#include "Futurewalker.Text.TextInputState.hpp"

#include "Futurewalker.Unit.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformInputEditableWin : public PlatformInputEditable
{
public:
    static auto Make(Delegate const& delegate) -> Shared<PlatformInputEditableWin>;

    PlatformInputEditableWin(PassKey<PlatformInputEditable> key, Delegate const& delegate);

    auto SetTextStore(Shared<PlatformInputMethodTextStoreWin> textStore) -> void;

    auto GetText() const -> Text override;
    auto SetText(Text const& text) -> void override;

    auto GetString(Range<CodePoint> range) const -> String override;
    auto GetStringRange() const -> Range<CodePoint> override;

    auto GetSelectedRange() const -> Range<CodePoint> override;
    auto SetSelectedRange(Range<CodePoint> const& range) -> void override;

    auto GetComposingRange() const -> Range<CodePoint> override;
    auto SetComposingRange(Range<CodePoint> const& range) -> void override;

    auto GetLayoutRect() const -> Rect<Dp> override;
    auto SetLayoutRect(Rect<Dp> const& layoutRect) -> void override;

    auto GetLayoutInfo() const -> Graphics::TextLayoutInfo override;
    auto SetLayoutInfo(Graphics::TextLayoutInfo const& layoutInfo) -> void override;

    auto InsertText(String const& text, CodePoint caretPosition) -> void override;
    auto InsertLineBreak() -> void override;
    auto DeleteSurroundingText(CodePoint before, CodePoint after) -> void override;

    auto GetU16String() const -> U16String;
    auto GetU16String(Range<CodeUnit> range) const -> U16String;
    auto GetU16StringRange() const -> Range<CodeUnit>;
    auto GetU16SelectedRange() const -> Range<CodeUnit>;
    auto SetU16SelectedRange(Range<CodeUnit> range, Bool anticipated) -> void;
    auto GetU16ComposingRange() const -> Range<CodeUnit>;
    auto SetU16ComposingRange(Range<CodeUnit> range) -> void;

    auto InsertU16String(U16StringView const text, CodePoint caretPosition, Bool anticipated, Bool cancellable) -> void;

    auto CompositionStart() -> void;
    auto CompositionUpdate() -> void;
    auto CompositionEnd() -> void;

    auto GetRangeFromU16Range(Range<CodeUnit> range) const -> Range<CodePoint>;

private:
    auto InternalBeforeInsertText(String const& text, Bool cancellable) -> Bool;
    auto InternalInsertText(String const& text) -> void;
    auto InternalBeforeInsertLineBreak(Bool cancellable) -> Bool;
    auto InternalInsertLineBreak() -> void;
    auto InternalBeforeDeleteSurroundingText(CodePoint before, CodePoint after, Bool cancellable) -> Bool;
    auto InternalDeleteSurroundingText(CodePoint before, CodePoint after) -> void;
    auto InternalOnTextChange(CodeUnit u16OldBegin, CodeUnit u16OldEnd, CodeUnit u16NewEnd) -> void;
    auto InternalOnSelectionChange() -> void;

    auto GetTextStore() -> Shared<PlatformInputMethodTextStoreWin>;

private:
    Weak<PlatformInputMethodTextStoreWin> _textStore;
    TextInputState _state;
    Rect<Dp> _layoutRect;
    Graphics::TextLayoutInfo _layoutInfo;
};
}
}

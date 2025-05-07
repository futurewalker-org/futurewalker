// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.Win.PlatformInputMethodEditableWinType.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodTextStoreWinType.hpp"

#include "Futurewalker.Application.PlatformTextInputState.hpp"
#include "Futurewalker.Application.PlatformInputMethodEditable.hpp"

#include "Futurewalker.Unit.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformInputMethodEditableWin : public PlatformInputMethodEditable
{
public:
    static auto Make(Delegate const& delegate) -> Shared<PlatformInputMethodEditableWin>;

    PlatformInputMethodEditableWin(PassKey<PlatformInputMethodEditableWin>, Delegate const& delegate);

    auto SetTextStore(Shared<PlatformInputMethodTextStoreWin> textStore) -> void;

    auto GetText() const -> String override;
    auto GetText(Range<CodePoint> range) const -> String override;
    auto GetTextRange() const -> Range<CodePoint> override;
    auto SetText(String const& text) -> void override;

    auto GetSelectedRange() const -> Range<CodePoint> override;
    auto SetSelectedRange(Range<CodePoint> const& range) -> void override;

    auto GetComposingRange() const -> Range<CodePoint> override;
    auto SetComposingRange(Range<CodePoint> const& range) -> void override;

    auto GetLayoutRect() const -> Rect<Dp> override;
    auto SetLayoutRect(Rect<Dp> const& layoutRect) -> void override;

    auto GetLayoutInfo() const -> Graphics::TextLayoutInfo override;
    auto SetLayoutInfo(Graphics::TextLayoutInfo const& layoutInfo) -> void override;

    auto InsertText(String const& text, CodePoint caretPosition) -> void override;
    auto DeleteSurroundingText(CodePoint before, CodePoint after) -> void override;

    auto GetU16Text() const -> std::u16string_view;
    auto GetU16Text(Range<CodeUnit> range) const -> std::u16string_view;
    auto GetU16TextRange() const -> Range<CodeUnit>;
    auto GetU16SelectedRange() const -> Range<CodeUnit>;
    auto SetU16SelectedRange(Range<CodeUnit> range) -> void;
    auto GetU16ComposingRange() const -> Range<CodeUnit>;
    auto SetU16ComposingRange(Range<CodeUnit> range) -> void;

    auto InsertU16Text(std::u16string_view const text, CodePoint caretPosition) -> void;

    auto GetRangeFromU16Range(Range<CodeUnit> range) const -> Range<CodePoint>;

private:
    auto InternalBeforeInsertText(String const& text) -> Bool;

    auto GetSelf() -> Shared<PlatformInputMethodEditableWin>;
    auto GetTextStore() -> Shared<PlatformInputMethodTextStoreWin>;

private:
    Weak<PlatformInputMethodEditableWin> _self;
    Weak<PlatformInputMethodTextStoreWin> _textStore;
    PlatformTextInputState _state;
    Rect<Dp> _layoutRect;
    Graphics::TextLayoutInfo _layoutInfo;
};
}
}

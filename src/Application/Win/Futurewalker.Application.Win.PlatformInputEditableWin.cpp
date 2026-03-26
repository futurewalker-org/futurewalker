// SPDX-License-Identifier: MPL-2.0
#include "Futurewalker.Application.Win.PlatformInputEditableWin.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodTextStoreWin.hpp"
#include "Futurewalker.Application.PlatformInputEvent.hpp"

#include "Futurewalker.Core.StringFunction.hpp"

#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
auto PlatformInputEditableWin::Make(Delegate const& delegate) -> Shared<PlatformInputEditableWin>
{
    return PlatformInputEditable::MakeDerived<PlatformInputEditableWin>(delegate);
}

PlatformInputEditableWin::PlatformInputEditableWin(PassKey<PlatformInputEditable> key, Delegate const& delegate)
  : PlatformInputEditable(key, delegate)
  , _state {{
        .beforeInsertText = [&](String const& text, Bool cancellable) { return InternalBeforeInsertText(text, cancellable); },
        .insertText = [&](String const& text) { InternalInsertText(text); },
        .beforeInsertLineBreak = [&](Bool cancellable) { return InternalBeforeInsertLineBreak(cancellable); },
        .insertLineBreak = [&]() { InternalInsertLineBreak(); },
        .beforeDeleteSurroundingText = [&](CodePoint before, CodePoint after, Bool cancellable) { return InternalBeforeDeleteSurroundingText(before, after, cancellable); },
        .deleteSurroundingText = [&](CodePoint before, CodePoint after) { InternalDeleteSurroundingText(before, after); },
        .onTextChange = [&](CodeUnit u16OldBegin, CodeUnit u16OldEnd, CodeUnit u16NewEnd) { return InternalOnTextChange(u16OldBegin, u16OldEnd, u16NewEnd); },
        .onSelectionChange = [&]() { return InternalOnSelectionChange(); },
    }}
{
}

auto PlatformInputEditableWin::SetTextStore(Shared<PlatformInputMethodTextStoreWin> textStore) -> void
{
    _textStore = textStore;
}

auto PlatformInputEditableWin::GetText() const -> Text
{
    return _state.GetText();
}

auto PlatformInputEditableWin::SetText(Text const& text) -> void
{
    _state.SetText(text);
}

auto PlatformInputEditableWin::GetString(Range<CodePoint> const& range) const -> String
{
    return _state.GetString(range);
}

auto PlatformInputEditableWin::GetStringRange() const -> Range<CodePoint>
{
    return _state.GetStringRange();
}

auto PlatformInputEditableWin::GetSelectionDirection() const -> TextSelectionDirection
{
    return _state.GetSelectionDirection();
}

auto PlatformInputEditableWin::GetSelectionRange() const -> Range<CodePoint>
{
    return _state.GetSelectionRange();
}

auto PlatformInputEditableWin::SetSelectionRange(Range<CodePoint> const& range, TextSelectionDirection direction) -> void
{
    _state.SetSelectionRange(range, direction);
}

auto PlatformInputEditableWin::GetComposingRange() const -> Range<CodePoint>
{
    return _state.GetComposingRange();
}

auto PlatformInputEditableWin::SetComposingRange(Range<CodePoint> const& range) -> void
{
    _state.SetComposingRange(range);
}

auto PlatformInputEditableWin::GetLayoutRect() const -> Rect<Dp>
{
    return _layoutRect;
}

auto PlatformInputEditableWin::SetLayoutRect(Rect<Dp> const& layoutRect) -> void
{
    if (_layoutRect != layoutRect)
    {
        _layoutRect = layoutRect;

        if (auto textStore = GetTextStore())
        {
            textStore->NotifyLayoutChange();
        }
    }
}

auto PlatformInputEditableWin::GetLayoutInfo() const -> Graphics::TextLayoutInfo
{
    return _layoutInfo;
}

auto PlatformInputEditableWin::SetLayoutInfo(Graphics::TextLayoutInfo const& layoutInfo) -> void
{
    _layoutInfo = layoutInfo;

    if (auto textStore = GetTextStore())
    {
        textStore->NotifyLayoutChange();
    }
}

auto PlatformInputEditableWin::InsertText(String const& text, CodePoint caretPosition) -> void
{
    FW_DEBUG_LOG_INFO("PlatformInputEditableWin::InsertText: caretPosition={}", static_cast<int64_t>(caretPosition));
    _state.InsertText(text, caretPosition, false, true);
}

auto PlatformInputEditableWin::InsertLineBreak() -> void
{
    FW_DEBUG_LOG_INFO("PlatformInputEditableWin::InsertLineBreak");
    _state.InsertLineBreak(false, true);
}

auto PlatformInputEditableWin::DeleteSurroundingText(CodePoint before, CodePoint after) -> void
{
    FW_DEBUG_LOG_INFO("PlatformInputEditableWin::DeleteSurroundingText: before={}, after={}", static_cast<int64_t>(before), static_cast<int64_t>(after));
    _state.DeleteSurroundingText(before, after, false, true);
}

auto PlatformInputEditableWin::GetU16String() const -> U16String
{
    return _state.GetU16String();
}

auto PlatformInputEditableWin::GetU16String(Range<CodeUnit> range) const -> U16String
{
    return _state.GetU16String(range);
}

auto PlatformInputEditableWin::GetU16StringRange() const -> Range<CodeUnit>
{
    return _state.GetU16StringRange();
}

auto PlatformInputEditableWin::GetU16SelectionRange() const -> Range<CodeUnit>
{
    return _state.GetU16SelectionRange();
}

auto PlatformInputEditableWin::SetU16SelectionRange(Range<CodeUnit> const& range, TextSelectionDirection const direction, Bool anticipated) -> void
{
    _state.SetU16SelectionRange(range, direction, anticipated);
}

auto PlatformInputEditableWin::GetU16ComposingRange() const -> Range<CodeUnit>
{
    return _state.GetU16ComposingRange();
}

auto PlatformInputEditableWin::SetU16ComposingRange(Range<CodeUnit> const& range) -> void
{
    _state.SetU16ComposingRange(range);
}

auto PlatformInputEditableWin::InsertU16String(U16StringView const text, CodePoint caretPosition, Bool anticipated, Bool cancellable) -> void
{
    FW_DEBUG_LOG_INFO("PlatformInputEditableWin::InsertU16String: caretPosition={}", static_cast<int64_t>(caretPosition));
    _state.InsertU16Text(text, caretPosition, anticipated, cancellable);
}

auto PlatformInputEditableWin::CompositionStart() -> void
{
    auto event = Event<>(Event<PlatformInputEvent::CompositionStart>());
    SendInputEventDetached(event);
}

auto PlatformInputEditableWin::CompositionUpdate() -> void
{
    auto event = Event<>(Event<PlatformInputEvent::CompositionUpdate>());
    SendInputEventDetached(event);
}

auto PlatformInputEditableWin::CompositionEnd() -> void
{
    auto event = Event<>(Event<PlatformInputEvent::CompositionEnd>());
    SendInputEventDetached(event);
}

auto PlatformInputEditableWin::GetRangeFromU16Range(Range<CodeUnit> range) const -> Range<CodePoint>
{
    return _state.GetRangeFromU16Range(range);
}

auto PlatformInputEditableWin::InternalBeforeInsertText(String const& text, Bool cancellable) -> Bool
{
    if (cancellable)
    {
        auto parameter = Event<PlatformInputEvent::BeforeInsertText>();
        parameter->SetText(text);
        auto event = Event<>(parameter);
        if (SendInputEventDetached(event))
        {
            if (event.Is<PlatformInputEvent::BeforeInsertText>())
            {
                parameter = event.As<PlatformInputEvent::BeforeInsertText>();
                return !parameter->GetCancel();
            }
            return false;
        }
    }
    else
    {
        auto parameter = Event<PlatformInputEvent::BeforeInsertCompositionText>();
        parameter->SetText(text);
        auto event = Event<>(std::move(parameter));
        SendInputEventDetached(event);
    }
    return true;
}

auto PlatformInputEditableWin::InternalInsertText(String const& text) -> void
{
    auto parameter = Event<PlatformInputEvent::InsertText>();
    parameter->SetText(text);
    auto event = Event<>(std::move(parameter));
    SendInputEventDetached(event);
}

auto PlatformInputEditableWin::InternalBeforeInsertLineBreak(Bool cancellable) -> Bool
{
    FW_DEBUG_ASSERT(cancellable);
    auto parameter = Event<PlatformInputEvent::BeforeInsertLineBreak>();
    auto event = Event<>(parameter);
    if (SendInputEventDetached(event))
    {
        if (cancellable)
        {
            if (event.Is<PlatformInputEvent::BeforeInsertLineBreak>())
            {
                parameter = event.As<PlatformInputEvent::BeforeInsertLineBreak>();
                return !parameter->GetCancel();
            }
            return false;
        }
    }
    return true;
}

auto PlatformInputEditableWin::InternalInsertLineBreak() -> void
{
    auto parameter = Event<PlatformInputEvent::InsertLineBreak>();
    auto event = Event<>(std::move(parameter));
    SendInputEventDetached(event);
}

auto PlatformInputEditableWin::InternalBeforeDeleteSurroundingText(CodePoint before, CodePoint after, Bool cancellable) -> Bool
{
    FW_DEBUG_ASSERT(cancellable);
    auto parameter = Event<PlatformInputEvent::BeforeDeleteSurroundingText>();
    parameter->SetBefore(before);
    parameter->SetAfter(after);
    auto event = Event<>(parameter);
    if (SendInputEventDetached(event))
    {
        if (cancellable)
        {
            if (event.Is<PlatformInputEvent::BeforeDeleteSurroundingText>())
            {
                parameter = event.As<PlatformInputEvent::BeforeDeleteSurroundingText>();
                return !parameter->GetCancel();
            }
            return false;
        }
    }
    return true;
}

auto PlatformInputEditableWin::InternalDeleteSurroundingText(CodePoint before, CodePoint after) -> void
{
    auto parameter = Event<PlatformInputEvent::DeleteSurroundingText>();
    parameter->SetBefore(before);
    parameter->SetAfter(after);
    auto event = Event<>(std::move(parameter));
    SendInputEventDetached(event);
}

auto PlatformInputEditableWin::InternalOnTextChange(CodeUnit u16OldBegin, CodeUnit u16OldEnd, CodeUnit u16NewEnd) -> void
{
    if (auto textStore = GetTextStore())
    {
        textStore->NotifyTextChange(u16OldBegin, u16OldEnd, u16NewEnd);
    }
}

auto PlatformInputEditableWin::InternalOnSelectionChange() -> void
{
    if (auto textStore = GetTextStore())
    {
        textStore->NotifySelectionChange();
    }
}

auto PlatformInputEditableWin::GetTextStore() -> Shared<PlatformInputMethodTextStoreWin>
{
    return _textStore.Lock();
}
}

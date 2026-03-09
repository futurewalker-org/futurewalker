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
        .beforeInsertText = [&](String const& text) { return InternalBeforeInsertText(text); },
        .beforeDeleteSurroundingText = [&](CodePoint before, CodePoint after) { return InternalBeforeDeleteSurroundingText(before, after); },
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

auto PlatformInputEditableWin::GetString(Range<CodePoint> range) const -> String
{
    return _state.GetString(range);
}

auto PlatformInputEditableWin::GetStringRange() const -> Range<CodePoint>
{
    return _state.GetStringRange();
}

auto PlatformInputEditableWin::GetSelectedRange() const -> Range<CodePoint>
{
    return _state.GetSelectedRange();
}

auto PlatformInputEditableWin::SetSelectedRange(Range<CodePoint> const& range) -> void
{
    _state.SetSelectedRange(range);
}

auto PlatformInputEditableWin::GetComposingRange() const -> Range<CodePoint>
{
    return _state.GetSelectedRange();
}

auto PlatformInputEditableWin::SetComposingRange(Range<CodePoint> const& range) -> void
{
    _state.SetSelectedRange(range);
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
    _state.InsertText(text, caretPosition, false);
}

auto PlatformInputEditableWin::DeleteSurroundingText(CodePoint before, CodePoint after) -> void
{
    FW_DEBUG_LOG_INFO("PlatformInputEditableWin::DeleteSurroundingText: before={}, after={}", static_cast<int64_t>(before), static_cast<int64_t>(after));
    _state.DeleteSurroundingText(before, after, false);
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

auto PlatformInputEditableWin::GetU16SelectedRange() const -> Range<CodeUnit>
{
    return _state.GetU16SelectedRange();
}

auto PlatformInputEditableWin::SetU16SelectedRange(Range<CodeUnit> range, Bool anticipated) -> void
{
    _state.SetU16SelectedRange(range, anticipated);
}

auto PlatformInputEditableWin::GetU16ComposingRange() const -> Range<CodeUnit>
{
    return _state.GetU16ComposingRange();
}

auto PlatformInputEditableWin::SetU16ComposingRange(Range<CodeUnit> range) -> void
{
    _state.SetU16ComposingRange(range);
}

auto PlatformInputEditableWin::InsertU16String(U16StringView const text, CodePoint caretPosition, Bool anticipated) -> void
{
    FW_DEBUG_LOG_INFO("PlatformInputEditableWin::InsertU16String: caretPosition={}", static_cast<int64_t>(caretPosition));
    {
        auto u8String = _state.GetString();
        auto u16String = _state.GetU16String();
        FW_DEBUG_LOG_INFO("PlatformInputEditableWin::InsertU16String: u8string size={}", (int)u8String.GetView().GetSize());
        FW_DEBUG_LOG_INFO("PlatformInputEditableWin::InsertU16String: u16string size={}", (int)u16String.GetView().GetSize());
    }

    _state.InsertU16Text(text, caretPosition, anticipated);

    {
        auto u8String = _state.GetString();
        auto u16String = _state.GetU16String();
        FW_DEBUG_LOG_INFO("PlatformInputEditableWin::InsertU16String: u8string size={}", (int)u8String.GetView().GetSize());
        FW_DEBUG_LOG_INFO("PlatformInputEditableWin::InsertU16String: u16string size={}", (int)u16String.GetView().GetSize());
        if (u8String.GetView().GetSize() > 100)
        {
            auto t = Text(text);
            __debugbreak();
        }
    }
}

auto PlatformInputEditableWin::GetRangeFromU16Range(Range<CodeUnit> range) const -> Range<CodePoint>
{
    return _state.GetRangeFromU16Range(range);
}

auto PlatformInputEditableWin::InternalBeforeInsertText(String const& text) -> Bool
{
    auto parameter = Event<PlatformInputEvent::InsertText>();
    parameter->SetText(text);
    auto event = Event<>(parameter);
    if (SendInputEventDetached(event))
    {
        if (event.Is<PlatformInputEvent::InsertText>())
        {
            parameter = event.As<PlatformInputEvent::InsertText>();
            return !parameter->GetCancel();
        }
        return false;
    }
    return true;
}

auto PlatformInputEditableWin::InternalBeforeDeleteSurroundingText(CodePoint before, CodePoint after) -> Bool
{
    auto parameter = Event<PlatformInputEvent::DeleteSurroundingText>();
    parameter->SetBefore(before);
    parameter->SetAfter(after);
    auto event = Event<>(parameter);
    if (SendInputEventDetached(event))
    {
        if (event.Is<PlatformInputEvent::DeleteSurroundingText>())
        {
            parameter = event.As<PlatformInputEvent::DeleteSurroundingText>();
            return !parameter->GetCancel();
        }
        return false;
    }
    return true;
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

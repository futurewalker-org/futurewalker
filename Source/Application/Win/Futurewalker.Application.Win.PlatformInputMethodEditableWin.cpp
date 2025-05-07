// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.Win.PlatformInputMethodEditableWin.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodTextStoreWin.hpp"
#include "Futurewalker.Application.Win.PlatformEventLoopContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformEventLoopWin.hpp"
#include "Futurewalker.Application.PlatformInputEvent.hpp"

#include "Futurewalker.Core.StringFunction.hpp"

#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
auto PlatformInputMethodEditableWin::Make(Delegate const& delegate) -> Shared<PlatformInputMethodEditableWin>
{
    auto editable = Shared<PlatformInputMethodEditableWin>::Make(PassKey<PlatformInputMethodEditableWin>(), delegate);
    editable->_self = editable;
    return editable;
}

PlatformInputMethodEditableWin::PlatformInputMethodEditableWin(PassKey<PlatformInputMethodEditableWin>, Delegate const& delegate)
  : PlatformInputMethodEditable(delegate)
  , _state {{
      .beforeInsertText = [&](String const& text) { return InternalBeforeInsertText(text); },
    }}
{
}

auto PlatformInputMethodEditableWin::SetTextStore(Shared<PlatformInputMethodTextStoreWin> textStore) -> void
{
    _textStore = textStore;
}

auto PlatformInputMethodEditableWin::GetText() const -> String
{
    return _state.GetText();
}

auto PlatformInputMethodEditableWin::GetText(Range<CodePoint> range) const -> String
{
    return _state.GetText(range);
}

auto PlatformInputMethodEditableWin::GetTextRange() const -> Range<CodePoint>
{
    return _state.GetTextRange();
}

auto PlatformInputMethodEditableWin::SetText(String const& text) -> void
{
    if (_state.SetText(text))
    {
    }
    else
    {
    }
}

auto PlatformInputMethodEditableWin::GetSelectedRange() const -> Range<CodePoint>
{
    return _state.GetSelectedRange();
}

auto PlatformInputMethodEditableWin::SetSelectedRange(Range<CodePoint> const& range) -> void
{
    if (_state.SetSelectedRange(range))
    {
    }
    else
    {
    }
}

auto PlatformInputMethodEditableWin::GetComposingRange() const -> Range<CodePoint>
{
    return _state.GetSelectedRange();
}

auto PlatformInputMethodEditableWin::SetComposingRange(Range<CodePoint> const& range) -> void
{
    if (_state.SetSelectedRange(range))
    {
    }
    else
    {
    }
}

auto PlatformInputMethodEditableWin::GetLayoutRect() const -> Rect<Dp>
{
    return _layoutRect;
}

auto PlatformInputMethodEditableWin::SetLayoutRect(Rect<Dp> const& layoutRect) -> void
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

auto PlatformInputMethodEditableWin::GetLayoutInfo() const -> Graphics::TextLayoutInfo
{
    return _layoutInfo;
}

auto PlatformInputMethodEditableWin::SetLayoutInfo(Graphics::TextLayoutInfo const& layoutInfo) -> void
{
    _layoutInfo = layoutInfo;

    if (auto textStore = GetTextStore())
    {
        textStore->NotifyLayoutChange();
    }
}

auto PlatformInputMethodEditableWin::InsertText(String const& text, CodePoint caretPosition) -> void
{
    _state.InsertText(text, caretPosition);
}

auto PlatformInputMethodEditableWin::DeleteSurroundingText(CodePoint before, CodePoint after) -> void
{
    _state.DeleteSurroundingText(before, after);
}

auto PlatformInputMethodEditableWin::GetU16Text() const -> std::u16string_view
{
    return _state.GetU16Text();
}

auto PlatformInputMethodEditableWin::GetU16Text(Range<CodeUnit> range) const -> std::u16string_view
{
    return _state.GetU16Text(range);
}

auto PlatformInputMethodEditableWin::GetU16TextRange() const -> Range<CodeUnit>
{
    return _state.GetU16TextRange();
}

auto PlatformInputMethodEditableWin::GetU16SelectedRange() const -> Range<CodeUnit>
{
    return _state.GetU16SelectedRange();
}

auto PlatformInputMethodEditableWin::SetU16SelectedRange(Range<CodeUnit> range) -> void
{
    _state.SetU16SelectedRange(range);
}

auto PlatformInputMethodEditableWin::GetU16ComposingRange() const -> Range<CodeUnit>
{
    return _state.GetU16ComposingRange();
}

auto PlatformInputMethodEditableWin::SetU16ComposingRange(Range<CodeUnit> range) -> void
{
    _state.SetU16ComposingRange(range);
}

auto PlatformInputMethodEditableWin::InsertU16Text(std::u16string_view const text, CodePoint caretPosition) -> void
{
    _state.InsertU16Text(text, caretPosition);
}

auto PlatformInputMethodEditableWin::GetRangeFromU16Range(Range<CodeUnit> range) const -> Range<CodePoint>
{
    return _state.GetRangeFromU16Range(range);
}

auto PlatformInputMethodEditableWin::InternalBeforeInsertText(String const& text) -> Bool
{
    if (const auto eventLoopContext = Locator::GetInstance<PlatformEventLoopContext>())
    {
        if (const auto eventLoop = eventLoopContext->GetEventLoopForThread().As<PlatformEventLoopWin>())
        {
            const auto self = GetSelf();
            auto parameter = PlatformInputEvent::InsertText();
            parameter.SetText(text);
            auto event = Event(parameter);
            eventLoop->Spawn(AsyncFunction::Fn([self, e = event] mutable -> Lazy<void> { co_await self->SendInputEvent(e); }), false);
        }
    }
    return true;
}

auto PlatformInputMethodEditableWin::GetSelf() -> Shared<PlatformInputMethodEditableWin>
{
    return _self.Lock();
}

auto PlatformInputMethodEditableWin::GetTextStore() -> Shared<PlatformInputMethodTextStoreWin>
{
    return _textStore.Lock();
}
}

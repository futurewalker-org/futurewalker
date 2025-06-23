// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.InputMethodEditable.hpp"
#include "Futurewalker.Application.PlatformInputMethod.hpp"
#include "Futurewalker.Application.PlatformInputMethodContext.hpp"
#include "Futurewalker.Application.PlatformInputMethodEditable.hpp"
#include "Futurewalker.Application.PlatformInputEvent.hpp"
#include "Futurewalker.Application.InputEvent.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Make new instance.
///
auto InputMethodEditable::Make() -> Shared<InputMethodEditable>
{
    return Shared<InputMethodEditable>::Make(PassKey<InputMethodEditable>());
}

///
/// @brief Constructor.
///
InputMethodEditable::InputMethodEditable(PassKey<InputMethodEditable>)
{
    if (auto inputMethodContext = Locator::GetInstance<PlatformInputMethodContext>())
    {
        _platform = inputMethodContext->MakeEditable({
          .sendInputevent = [&](Event<>& event) -> Async<Bool> { co_return co_await HandlePlatformInputEvent(event); },
        });
    }
    else
    {
        FW_DEBUG_ASSERT(false);
    }
    _eventReceiver = EventReceiver::Make();
}

///
/// @brief Get entire text.
///
/// @return Copy of UTF-8 text.
///
auto InputMethodEditable::GetText() const -> String
{
    if (_platform)
    {
        return _platform->GetText();
    }
    return {};
}

///
/// @brief Get part of text.
///
/// @param[in] range Range of string in code points
///
/// @return Copy of the part of the string.
///
auto InputMethodEditable::GetText(Range<CodePoint> range) const -> String
{
    if (_platform)
    {
        return _platform->GetText(range);
    }
    return {};
}

///
/// @brief Get whole range of text.
///
/// @return Range of (0, length of text in code points).
///
auto InputMethodEditable::GetTextRange() const -> Range<CodePoint>
{
    if (_platform)
    {
        return _platform->GetTextRange();
    }
    return {};
}

///
/// @brief Set new text.
///
/// @param[in] text New text value.
///
auto InputMethodEditable::SetText(String const& text) -> void
{
    if (_platform)
    {
        _platform->SetText(text);
    }
}

///
/// @brief Get selected range.
///
/// @return Selected range in code points.
///
auto InputMethodEditable::GetSelectedRange() const -> Range<CodePoint>
{
    if (_platform)
    {
        return _platform->GetSelectedRange();
    }
    return {};
}

///
/// @brief Set new selected range.
///
/// @param[in] range New selected range in code points.
///
auto InputMethodEditable::SetSelectedRange(Range<CodePoint> const& range) -> void
{
    if (_platform)
    {
        return _platform->SetSelectedRange(range);
    }
}

///
/// @brief Get composing range.
///
/// @return Composing range in code points.
///
auto InputMethodEditable::GetComposingRange() const -> Range<CodePoint>
{
    if (_platform)
    {
        return _platform->GetComposingRange();
    }
    return {};
}

///
/// @brief Set new composing range.
///
/// @param[in] range New composing range in code points.
///
auto InputMethodEditable::SetComposingRange(Range<CodePoint> const& range) -> void
{
    if (_platform)
    {
        _platform->SetComposingRange(range);
    }
}

///
/// @brief
///
auto InputMethodEditable::GetLayoutRect() const -> Rect<Dp>
{
    if (_platform)
    {
        return _platform->GetLayoutRect();
    }
    return {};
}

///
/// @brief
///
auto InputMethodEditable::SetLayoutRect(Rect<Dp> const& rect) -> void
{
    if (_platform)
    {
        _platform->SetLayoutRect(rect);
    }
}

///
/// @brief
///
auto InputMethodEditable::GetLayoutInfo() const -> Graphics::TextLayoutInfo
{
    if (_platform)
    {
        return _platform->GetLayoutInfo();
    }
    return {};
}

///
/// @brief
///
auto InputMethodEditable::SetLayoutInfo(Graphics::TextLayoutInfo const& layoutInfo) -> void
{
    if (_platform)
    {
        _platform->SetLayoutInfo(layoutInfo);
    }
}

///
/// @brief Insert text.
///
auto InputMethodEditable::InsertText(String const& text, CodePoint caretPosition) -> void
{
    if (_platform)
    {
        _platform->InsertText(text, caretPosition);
    }
}

///
/// @brief
///
auto InputMethodEditable::DeleteSurroundingText(CodePoint before, CodePoint after) -> void
{
    if (_platform)
    {
        _platform->DeleteSurroundingText(before, after);
    }
}

///
/// @brief
///
auto InputMethodEditable::GetEventReceiver() -> EventReceiver&
{
    return *_eventReceiver;
}

///
/// @brief
///
auto InputMethodEditable::GetEventReceiver() const -> EventReceiver const&
{
    return *_eventReceiver;
}

///
/// @brief
///
auto InputMethodEditable::Attach(PassKey<InputMethod>, Shared<PlatformInputMethod> inputMethod) -> void
{
    if (inputMethod)
    {
        inputMethod->SetEditable(_platform);
    }
}

///
/// @brief
///
auto InputMethodEditable::HandlePlatformInputEvent(Event<>& event) -> Async<Bool>
{
    if (_eventReceiver)
    {
        if (event.Is<PlatformInputEvent::InsertText>())
        {
            auto inputEventParameter = Event<InputEvent::InsertText>::Make();
            inputEventParameter->SetText(event.As<PlatformInputEvent::InsertText>()->GetText());
            auto inputEvent = Event<>(inputEventParameter);
            co_return co_await _eventReceiver->SendEvent(inputEvent);
        }
        else if (event.Is<PlatformInputEvent::DeleteSurroundingText>())
        {
            auto inputEventParameter = Event<InputEvent::DeleteSurroundingText>::Make();
            inputEventParameter->SetBefore(event.As<PlatformInputEvent::DeleteSurroundingText>()->GetBefore());
            inputEventParameter->SetAfter(event.As<PlatformInputEvent::DeleteSurroundingText>()->GetAfter());
            auto inputEvent = Event<>(inputEventParameter);
            co_return co_await _eventReceiver->SendEvent(inputEvent);
        }
        else if (event.Is<PlatformInputEvent::InsertCompositionText>())
        {
            auto inputEventParameter = Event<InputEvent::InsertCompositionText>();
            inputEventParameter->SetText(event.As<PlatformInputEvent::InsertCompositionText>()->GetText());
            auto inputEvent = Event<>(inputEventParameter);
            co_return co_await _eventReceiver->SendEvent(inputEvent);
        }
        else if (event.Is<PlatformInputEvent::SelectionChange>())
        {
            auto inputEvent = Event<>(Event<InputEvent::SelectionChange>());
            co_return co_await _eventReceiver->SendEvent(inputEvent);
        }
    }
    co_return false;
}
}

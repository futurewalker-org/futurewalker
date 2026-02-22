// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.InputEditable.hpp"
#include "Futurewalker.Application.PlatformInputMethod.hpp"
#include "Futurewalker.Application.PlatformInputMethodContext.hpp"
#include "Futurewalker.Application.PlatformInputEditable.hpp"
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
auto InputEditable::Make() -> Shared<InputEditable>
{
    return Shared<InputEditable>::Make(PassKey<InputEditable>());
}

///
/// @brief Constructor.
///
InputEditable::InputEditable(PassKey<InputEditable>)
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
auto InputEditable::GetText() const -> Text
{
    if (_platform)
    {
        return _platform->GetText();
    }
    return {};
}

///
/// @brief Set new text.
///
/// @param[in] text New text value.
///
auto InputEditable::SetText(Text const& text) -> void
{
    if (_platform)
    {
        _platform->SetText(text);
    }
}

///
/// @brief Get whole text as string.
///
auto InputEditable::GetString() const -> String
{
    if (_platform)
    {
        return _platform->GetString(_platform->GetStringRange());
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
auto InputEditable::GetString(Range<CodePoint> range) const -> String
{
    if (_platform)
    {
        return _platform->GetString(range);
    }
    return {};
}

///
/// @brief Get whole range of text.
///
/// @return Range of (0, length of text in code points).
///
auto InputEditable::GetStringRange() const -> Range<CodePoint>
{
    if (_platform)
    {
        return _platform->GetStringRange();
    }
    return {};
}

///
/// @brief Get selected range.
///
/// @return Selected range in code points.
///
auto InputEditable::GetSelectedRange() const -> Range<CodePoint>
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
auto InputEditable::SetSelectedRange(Range<CodePoint> const& range) -> void
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
auto InputEditable::GetComposingRange() const -> Range<CodePoint>
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
auto InputEditable::SetComposingRange(Range<CodePoint> const& range) -> void
{
    if (_platform)
    {
        _platform->SetComposingRange(range);
    }
}

///
/// @brief
///
auto InputEditable::GetLayoutOffset() const -> Offset<Dp>
{
    return _layoutOffset;
}

///
/// @brief
///
auto InputEditable::SetLayoutOffset(Offset<Dp> const& offset) -> void
{
    if (_layoutOffset != offset)
    {
        _layoutOffset = offset;
        UpdateLayoutRect();
    }
}

///
/// @brief
///
auto InputEditable::GetLayoutRect() const -> Rect<Dp>
{
    return _layoutRect;
}

///
/// @brief
///
auto InputEditable::SetLayoutRect(Rect<Dp> const& rect) -> void
{
    if (_layoutRect != rect)
    {
        _layoutRect = rect;
        UpdateLayoutRect();
    }
}

///
/// @brief
///
auto InputEditable::GetLayoutInfo() const -> Graphics::TextLayoutInfo
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
auto InputEditable::SetLayoutInfo(Graphics::TextLayoutInfo const& layoutInfo) -> void
{
    if (_platform)
    {
        _platform->SetLayoutInfo(layoutInfo);
    }
}

///
/// @brief Insert text.
///
auto InputEditable::InsertText(String const& text, CodePoint caretPosition) -> void
{
    if (_platform)
    {
        _platform->InsertText(text, caretPosition);
    }
}

///
/// @brief
///
auto InputEditable::DeleteSurroundingText(CodePoint before, CodePoint after) -> void
{
    if (_platform)
    {
        _platform->DeleteSurroundingText(before, after);
    }
}

///
/// @brief
///
auto InputEditable::GetEventReceiver() -> EventReceiver&
{
    return *_eventReceiver;
}

///
/// @brief
///
auto InputEditable::GetEventReceiver() const -> EventReceiver const&
{
    return *_eventReceiver;
}

///
/// @brief
///
auto InputEditable::GetPlatformObject(PassKey<InputMethod>) -> Shared<PlatformInputEditable>
{
    return _platform;
}

///
/// @brief
///
auto InputEditable::HandlePlatformInputEvent(Event<>& event) -> Async<Bool>
{
    if (_eventReceiver)
    {
        if (event.Is<PlatformInputEvent::InsertText>())
        {
            auto platformInputEventParameter = event.As<PlatformInputEvent::InsertText>();
            auto inputEventParameter = Event<InputEvent::InsertText>();
            inputEventParameter->SetText(platformInputEventParameter->GetText());
            inputEventParameter->SetCancel(platformInputEventParameter->GetCancel());
            auto inputEvent = Event<>(inputEventParameter);
            if (co_await _eventReceiver->SendEvent(inputEvent))
            {
                if (inputEvent.Is<InputEvent::InsertText>())
                {
                    inputEventParameter = inputEvent.As<InputEvent::InsertText>();
                    platformInputEventParameter->SetCancel(inputEventParameter->GetCancel());
                    event = platformInputEventParameter;
                }
                co_return true;
            }
        }
        else if (event.Is<PlatformInputEvent::DeleteSurroundingText>())
        {
            auto platformInputEventParameter = event.As<PlatformInputEvent::DeleteSurroundingText>();
            auto inputEventParameter = Event<InputEvent::DeleteSurroundingText>();
            inputEventParameter->SetBefore(platformInputEventParameter->GetBefore());
            inputEventParameter->SetAfter(platformInputEventParameter->GetAfter());
            inputEventParameter->SetCancel(platformInputEventParameter->GetCancel());
            auto inputEvent = Event<>(inputEventParameter);
            if (co_await _eventReceiver->SendEvent(inputEvent))
            {
                if (inputEvent.Is<InputEvent::DeleteSurroundingText>())
                {
                    inputEventParameter = inputEvent.As<InputEvent::DeleteSurroundingText>();
                    platformInputEventParameter->SetCancel(inputEventParameter->GetCancel());
                    event = platformInputEventParameter;
                }
                co_return true;
            }
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

auto InputEditable::UpdateLayoutRect() -> void
{
    if (_platform)
    {
        _platform->SetLayoutRect(Rect<Dp>::Offset(_layoutRect, _layoutOffset));
    }
}
}

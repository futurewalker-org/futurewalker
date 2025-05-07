// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.InputMethodEditable.hpp"
#include "Futurewalker.Application.InputMethod.hpp"
#include "Futurewalker.Application.PlatformInputMethodEditableType.hpp"
#include "Futurewalker.Application.PlatformInputMethodType.hpp"

#include "Futurewalker.Event.EventReceiverType.hpp"
#include "Futurewalker.Event.EventType.hpp"

#include "Futurewalker.Async.Async.hpp"

#include "Futurewalker.Geometry.Rect.hpp"

#include "Futurewalker.Unit.Unit.hpp" 

#include "Futurewalker.Graphics.TextLayoutInfo.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.Range.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Editable interface for IME interaction.
///
class InputMethodEditable : NonCopyable
{
public:
    static auto Make() -> Shared<InputMethodEditable>;

    InputMethodEditable(PassKey<InputMethodEditable>);

    auto GetText() const -> String;
    auto GetText(Range<CodePoint> range) const -> String;
    auto GetTextRange() const -> Range<CodePoint>;
    auto SetText(String const& text) -> void;

    auto GetSelectedRange() const -> Range<CodePoint>;
    auto SetSelectedRange(Range<CodePoint> const& range) -> void;

    auto GetComposingRange() const -> Range<CodePoint>;
    auto SetComposingRange(Range<CodePoint> const& range) -> void;

    auto GetLayoutRect() const -> Rect<Dp>;
    auto SetLayoutRect(Rect<Dp> const& rect) -> void;

    auto GetLayoutInfo() const -> Graphics::TextLayoutInfo;
    auto SetLayoutInfo(Graphics::TextLayoutInfo const& layoutInfo) -> void;

    auto InsertText(String const& text, CodePoint caretPosition) -> void;
    auto DeleteSurroundingText(CodePoint before, CodePoint after) -> void;

    auto GetEventReceiver() -> EventReceiver&;
    auto GetEventReceiver() const -> EventReceiver const&;

    auto Attach(PassKey<InputMethod>, Shared<PlatformInputMethod> inputMethod) -> void;

private:
    auto HandlePlatformInputEvent(Event& event) -> Async<Bool>;

private:
    Shared<PlatformInputMethodEditable> _platform;
    Shared<EventReceiver> _eventReceiver;
};
}
}

// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.InputEditable.hpp"
#include "Futurewalker.Application.InputMethod.hpp"
#include "Futurewalker.Application.PlatformInputEditableType.hpp"
#include "Futurewalker.Application.PlatformInputMethodType.hpp"

#include "Futurewalker.Event.EventReceiverType.hpp"
#include "Futurewalker.Event.EventType.hpp"

#include "Futurewalker.Async.Async.hpp"

#include "Futurewalker.Geometry.Rect.hpp"

#include "Futurewalker.Unit.Unit.hpp" 

#include "Futurewalker.Graphics.TextLayoutInfo.hpp"

#include "Futurewalker.Text.Text.hpp"
#include "Futurewalker.Text.TextSelectionDirection.hpp" 

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
/// Bidirectional communication channel between input method and text input controls.
///
class InputEditable : NonCopyable
{
public:
    static auto Make() -> Shared<InputEditable>;

    InputEditable(PassKey<InputEditable>);

    auto GetText() const -> Text;
    auto SetText(Text const& text) -> void;

    auto GetString() const -> String;
    auto GetString(Range<CodePoint> range) const -> String;
    auto GetStringRange() const -> Range<CodePoint>;

    auto GetSelectionDirection() const -> TextSelectionDirection;
    auto GetSelectionRange() const -> Range<CodePoint>;
    auto SetSelectionRange(Range<CodePoint> const& range, TextSelectionDirection direction) -> void;

    auto GetComposingRange() const -> Range<CodePoint>;

    auto GetLayoutOffset() const -> Vector<Dp>;
    auto SetLayoutOffset(Vector<Dp> const& offset) -> void;

    auto GetLayoutRect() const -> Rect<Dp>;
    auto SetLayoutRect(Rect<Dp> const& rect) -> void;

    auto GetLayoutInfo() const -> Graphics::TextLayoutInfo;
    auto SetLayoutInfo(Graphics::TextLayoutInfo const& layoutInfo) -> void;

    auto InsertText(String const& text, CodePoint caretPosition) -> void;
    auto InsertLineBreak() -> void;
    auto DeleteSurroundingText(CodePoint before, CodePoint after) -> void;

    auto GetEventReceiver() -> EventReceiver&;
    auto GetEventReceiver() const -> EventReceiver const&;

    auto GetPlatformObject(PassKey<InputMethod>) -> Shared<PlatformInputEditable>;

private:
    auto HandlePlatformInputEvent(Event<>& event) -> Async<Bool>;
    auto UpdateLayoutRect() -> void;

private:
    Shared<PlatformInputEditable> _platform;
    Shared<EventReceiver> _eventReceiver;
    Vector<Dp> _layoutOffset;
    Rect<Dp> _layoutRect;
};
}
}

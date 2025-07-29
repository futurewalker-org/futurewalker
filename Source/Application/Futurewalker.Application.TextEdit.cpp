// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.TextEdit.hpp"
#include "Futurewalker.Application.ViewEvent.hpp"
#include "Futurewalker.Application.PointerEvent.hpp"
#include "Futurewalker.Application.KeyEvent.hpp"
#include "Futurewalker.Application.InputEvent.hpp"
#include "Futurewalker.Application.FocusEvent.hpp"
#include "Futurewalker.Application.DrawScope.hpp"
#include "Futurewalker.Application.Key.hpp"
#include "Futurewalker.Application.InputMethod.hpp"
#include "Futurewalker.Application.InputMethodEditable.hpp"

#include "Futurewalker.Graphics.Scene.hpp"
#include "Futurewalker.Graphics.TextShaper.hpp"

#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
auto TextEdit::Make() -> Shared<TextEdit>
{
    return View::MakeDerived<TextEdit>();
}

TextEdit::TextEdit(PassKey<View> key)
  : View(key)
{
}

auto TextEdit::Initialize() -> void
{
    SetPointerTrackingFlags(ViewPointerTrackingFlags::All);
    SetFocusTrackingFlags(ViewFocusTrackingFlags::All);

    _inputMethodEditable = InputMethodEditable::Make();

    EventReceiver::Connect(*_inputMethodEditable, *this, &TextEdit::ReceiveInputEvent);
    EventReceiver::Connect(*this, *this, &TextEdit::ReceiveInputEvent);
    EventReceiver::Connect(*this, *this, &TextEdit::ReceiveKeyEvent);
    EventReceiver::Connect(*this, *this, &TextEdit::ReceiveFocusEvent);
    EventReceiver::Connect(*this, *this, &TextEdit::ReceivePointerEvent);
}

auto TextEdit::Draw(DrawScope& scope) -> void
{
    auto& scene = scope.GetScene();

    scene.AddRect({
        .rect = GetContentRect(),
        .color = RGBAColor(1, 1, 1, 1),
        .drawStyle = Graphics::DrawStyle::Fill,
    });

    scene.AddRect({
        .rect = GetContentRect(),
        .color = IsFocused() ? RGBAColor(1, 0, 0, 1) : RGBAColor(0, 0, 1, 1),
        .drawStyle = Graphics::DrawStyle::Stroke,
        .strokeWidth = IsFocused() ? 2.0 : 1.0,
    });

    if (_inputMethodEditable)
    {
        auto const text = _inputMethodEditable->GetText();
        auto shaper = Graphics::TextShaper::Make();
        auto shaped = shaper->ShapeText(text, nullptr, 96, Dp::Infinity());

        scene.AddText({
            .shaped = shaped,
            .color = RGBAColor(0, 0, 0, 1),
        });
    }
}

auto TextEdit::Measure(MeasureScope& scope) -> void
{
    View::Measure(scope);
}

auto TextEdit::Arrange(ArrangeScope& scope) -> void
{
    if (_inputMethodEditable)
    {
        _inputMethodEditable->SetLayoutRect(LocalToRootRect(GetContentRect()));
    }
    View::Arrange(scope);
}

auto TextEdit::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    (void)event;
    co_return false;
}

auto TextEdit::ReceiveKeyEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<KeyEvent::Down>())
    {
        FW_DEBUG_LOG_INFO("TextEdit::Down");
        co_return true;
    }
    else if (event.Is<KeyEvent::Up>())
    {
        FW_DEBUG_LOG_INFO("TextEdit::Up");
        co_return true;
    }
    co_return false;
}

auto TextEdit::ReceiveInputEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<InputEvent::Attach>())
    {
        FW_DEBUG_LOG_INFO("TextEdit::ReceiveInputEvent SetContext");
        auto const parameter = event.As<InputEvent::Attach>();
        InternalSetInputMethod(parameter->GetInputMethod());
        co_return true;
    }
    else if (event.Is<InputEvent::Detach>())
    {
        FW_DEBUG_LOG_INFO("TextEdit::ReceiveInputEvent ReleaseContext");
        InternalSetInputMethod(nullptr);
        co_return true;
    }
    else if (event.Is<InputEvent::InsertText>())
    {
        FW_DEBUG_LOG_INFO("TextEdit::ReceiveInputEvent InsertText");
        InvalidateVisual();
        co_return false;
    }
    else if (event.Is<InputEvent::DeleteSurroundingText>())
    {
        FW_DEBUG_LOG_INFO("TextEdit::ReceiveInputEvent DeleteSurroundgText");
        InvalidateVisual();
        co_return false;
    }
    co_return false;
}

auto TextEdit::ReceivePointerEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<PointerEvent>())
    {
        if (event.Is<PointerEvent::Motion::Down>())
        {
            FW_DEBUG_LOG_INFO("TextEdit: Pointer down");
            SetFocused(true);
        }
        else if (event.Is<PointerEvent::Motion::Up>())
        {
            FW_DEBUG_LOG_INFO("TextEdit: Pointer up");
        }
        else if (event.Is<PointerEvent::Motion::Move>())
        {
            FW_DEBUG_LOG_INFO("TextEdit: Pointer move");
        }
        else if (event.Is<PointerEvent::Motion::Enter>())
        {
            FW_DEBUG_LOG_INFO("TextEdit: Pointer enter");
        }
        else if (event.Is<PointerEvent::Motion::Leave>())
        {
            FW_DEBUG_LOG_INFO("TextEdit: Pointer leave");
        }
        co_return true;
    }
    co_return false;
}

auto TextEdit::ReceiveFocusEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<FocusEvent::FocusIn>())
    {
        FW_DEBUG_LOG_INFO("TextEdit: focus in");
        InvalidateVisual();
    }
    else if (event.Is<FocusEvent::FocusOut>())
    {
        FW_DEBUG_LOG_INFO("TextEdit: focus out");
        InvalidateVisual();
    }
    co_return false;
}

auto TextEdit::InternalSetInputMethod(Shared<InputMethod> const& inputMethod) -> void
{
    if (_inputMethod != inputMethod)
    {
        _inputMethod = inputMethod;

        if (_inputMethod)
        {
            _inputMethod->SetEditable(_inputMethodEditable);
        }
    }
}

auto TextEdit::InternalInsertText(String const& text, CodePoint newSelection) -> void
{
    if (_inputMethodEditable)
    {
        _inputMethodEditable->InsertText(text, newSelection);
        InvalidateVisual();
    }
}

auto TextEdit::InternalGetSelectedRange() -> Range<CodePoint>
{
    if (_inputMethodEditable)
    {
        return _inputMethodEditable->GetSelectedRange();
    }
    return {};
}

auto TextEdit::InternalSetSelectedRange(Range<CodePoint> range) -> void
{
    if (_inputMethodEditable)
    {
        _inputMethodEditable->SetSelectedRange(range);
    }
}

auto TextEdit::InternalGetComposingRange() -> Range<CodePoint>
{
    if (_inputMethodEditable)
    {
        return _inputMethodEditable->GetComposingRange();
    }
    return {};
}

auto TextEdit::InternalSetComposingRange(Range<CodePoint> range) -> void
{
    if (_inputMethodEditable)
    {
        _inputMethodEditable->SetComposingRange(range);
    }
}

auto TextEdit::InternalGetText(Range<CodePoint> range) -> String
{
    if (_inputMethodEditable)
    {
        return _inputMethodEditable->GetText(range);
    }
    return {};
}

auto TextEdit::InternalGetTextRange() const -> Range<CodePoint>
{
    if (_inputMethodEditable)
    {
        return _inputMethodEditable->GetTextRange();
    }
    return {};
}

auto TextEdit::InternalDeleteBackward() -> void
{
    if (_inputMethodEditable)
    {
        _inputMethodEditable->DeleteSurroundingText(1, 0);
        InvalidateVisual();
    }
}

auto TextEdit::InternalDeleteForward() -> void
{
    if (_inputMethodEditable)
    {
        _inputMethodEditable->DeleteSurroundingText(0, 1);
        InvalidateVisual();
    }
}
}
}

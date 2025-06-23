// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.InputEventType.hpp"
#include "Futurewalker.Application.InputMethodType.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

#include "Futurewalker.Unit.Text.hpp"

#include "Futurewalker.Core.Range.hpp"
#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class InputEvent : public EventParameter
{
public:
    class Attach;
    class Detach;
    class InsertText;
    class InsertCompositionText;
    class SelectionChange;
    class DeleteSurroundingText;
    class CompositionStart;
    class CompositionUpdate;
    class CompositionEnd;
};

class InputEvent::Attach final : public InputEvent
{
public:
    auto GetInputMethod() const -> Shared<InputMethod>;
    auto SetInputMethod(Shared<InputMethod> const& inputMethod) -> void;

private:
    Shared<InputMethod> _inputMethod;
};

class InputEvent::Detach final : public InputEvent
{
};

class InputEvent::InsertText final : public InputEvent
{
public:
    auto GetText() const -> String;
    auto SetText(String const& text) -> void;

private:
    String _text;
};

class InputEvent::InsertCompositionText final : public InputEvent
{
public:
    auto GetText() const -> String { return _text; }
    auto SetText(String const& text) -> void { _text = text; }

private:
    String _text;
};

class InputEvent::SelectionChange final : public InputEvent
{
};

class InputEvent::DeleteSurroundingText final : public InputEvent
{
public:
    auto GetBefore() const -> CodePoint;
    auto SetBefore(CodePoint before) -> void;

    auto GetAfter() const -> CodePoint;
    auto SetAfter(CodePoint after) -> void;

private:
    CodePoint _before = 0;
    CodePoint _after = 0;
};
}
}

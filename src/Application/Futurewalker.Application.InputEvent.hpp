// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.InputEventType.hpp"
#include "Futurewalker.Application.InputMethodType.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

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
    class BeforeInsertText;
    class InsertText;
    class BeforeInsertCompositionText;
    class InsertCompositionText;
    class BeforeInsertLineBreak;
    class InsertLineBreak;
    class BeforeDeleteSurroundingText;
    class DeleteSurroundingText;
    class SelectionChange;
    class CompositionStart;
    class CompositionUpdate;
    class CompositionEnd;
};

class InputEvent::Attach final : public InputEvent
{
};

class InputEvent::Detach final : public InputEvent
{
};

class InputEvent::BeforeInsertText final : public InputEvent
{
public:
    auto GetCancel() const -> Bool;
    auto SetCancel(Bool cancel) -> void;

    auto GetText() const -> String;
    auto SetText(String const& text) -> void;

private:
    Bool _cancel = false;
    String _text;
};

class InputEvent::InsertText final : public InputEvent
{
public:
    auto GetText() const -> String;
    auto SetText(String const& text) -> void;

private:
    String _text;
};

class InputEvent::BeforeInsertCompositionText final : public InputEvent
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
    auto GetText() const -> String;
    auto SetText(String const& text) -> void;

private:
    String _text;
};

class InputEvent::BeforeInsertLineBreak final : public InputEvent
{
public:
    auto GetCancel() const -> Bool;
    auto SetCancel(Bool cancel) -> void;

private:
    Bool _cancel = false;
};

class InputEvent::InsertLineBreak final : public InputEvent
{
};

class InputEvent::BeforeDeleteSurroundingText final : public InputEvent
{
public:
    auto GetCancel() const -> Bool;
    auto SetCancel(Bool cancel) -> void;

    auto GetBefore() const -> CodePoint;
    auto SetBefore(CodePoint before) -> void;

    auto GetAfter() const -> CodePoint;
    auto SetAfter(CodePoint after) -> void;

private:
    Bool _cancel = false;
    CodePoint _before = 0;
    CodePoint _after = 0;
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

class InputEvent::SelectionChange final : public InputEvent
{
};

class InputEvent::CompositionStart final : public InputEvent
{
};

class InputEvent::CompositionUpdate final : public InputEvent
{
};

class InputEvent::CompositionEnd final : public InputEvent
{
};
}
}

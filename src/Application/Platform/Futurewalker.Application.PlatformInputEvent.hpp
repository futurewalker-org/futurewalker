// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformInputEventType.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

#include "Futurewalker.Core.String.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformInputEvent : public EventParameter
{
public:
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
    class CompositionEnd;
    class CompositionUpdate;
};

class PlatformInputEvent::BeforeInsertText final : public PlatformInputEvent
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

class PlatformInputEvent::InsertText final : public PlatformInputEvent
{
public:
    auto GetText() const -> String;
    auto SetText(String const& text) -> void;

private:
    String _text;
};

class PlatformInputEvent::BeforeInsertCompositionText final : public PlatformInputEvent
{
public:
    auto GetText() const -> String;
    auto SetText(String const& text) -> void;

private:
    String _text;
};

class PlatformInputEvent::InsertCompositionText final : public PlatformInputEvent
{
public:
    auto GetText() const -> String;
    auto SetText(String const& text) -> void;

private:
    String _text;
};

class PlatformInputEvent::BeforeInsertLineBreak final : public PlatformInputEvent
{
public:
    auto GetCancel() const -> Bool;
    auto SetCancel(Bool cancel) -> void;

private:
    Bool _cancel = false;
};

class PlatformInputEvent::InsertLineBreak final : public PlatformInputEvent
{
};

class PlatformInputEvent::BeforeDeleteSurroundingText final : public PlatformInputEvent
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

class PlatformInputEvent::DeleteSurroundingText final : public PlatformInputEvent
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

class PlatformInputEvent::SelectionChange final : public PlatformInputEvent
{
};

class PlatformInputEvent::CompositionStart final : public PlatformInputEvent
{
};

class PlatformInputEvent::CompositionEnd final : public PlatformInputEvent
{
};

class PlatformInputEvent::CompositionUpdate final : public PlatformInputEvent
{
};
}
}

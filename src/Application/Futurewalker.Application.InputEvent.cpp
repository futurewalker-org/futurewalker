// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.InputEvent.hpp"

namespace FW_DETAIL_NS
{
auto InputEvent::BeforeInsertText::GetCancel() const -> Bool
{
    return _cancel;
}

auto InputEvent::BeforeInsertText::SetCancel(Bool cancel) -> void
{
    _cancel = cancel;
}

auto InputEvent::BeforeInsertText::SetText(String const& text) -> void
{
    _text = text;
}

auto InputEvent::BeforeInsertText::GetText() const -> String
{
    return _text;
}

auto InputEvent::InsertText::GetText() const -> String
{
    return _text;
}

auto InputEvent::InsertText::SetText(String const& text) -> void
{
    _text = text;
}

auto InputEvent::BeforeInsertCompositionText::GetText() const -> String
{
    return _text;
}

auto InputEvent::BeforeInsertCompositionText::SetText(String const& text) -> void
{
    _text = text;
}


auto InputEvent::InsertCompositionText::GetText() const -> String
{
    return _text;
}

auto InputEvent::InsertCompositionText::SetText(String const& text) -> void
{
    _text = text;
}

auto InputEvent::BeforeInsertLineBreak::GetCancel() const -> Bool
{
    return _cancel;
}

auto InputEvent::BeforeInsertLineBreak::SetCancel(Bool cancel) -> void
{
    _cancel = cancel;
}

auto InputEvent::BeforeDeleteSurroundingText::GetCancel() const -> Bool
{
    return _cancel;
}

auto InputEvent::BeforeDeleteSurroundingText::SetCancel(Bool cancel) -> void
{
    _cancel = cancel;
}

auto InputEvent::BeforeDeleteSurroundingText::GetBefore() const -> CodePoint
{
    return _before;
}

auto InputEvent::BeforeDeleteSurroundingText::SetBefore(CodePoint before) -> void
{
    _before = before;
}

auto InputEvent::BeforeDeleteSurroundingText::GetAfter() const -> CodePoint
{
    return _after;
}

auto InputEvent::BeforeDeleteSurroundingText::SetAfter(CodePoint after) -> void
{
    _after = after;
}

auto InputEvent::DeleteSurroundingText::GetBefore() const -> CodePoint
{
    return _before;
}

auto InputEvent::DeleteSurroundingText::SetBefore(CodePoint before) -> void
{
    _before = before;
}

auto InputEvent::DeleteSurroundingText::GetAfter() const -> CodePoint
{
    return _after;
}

auto InputEvent::DeleteSurroundingText::SetAfter(CodePoint after) -> void
{
    _after = after;
}
}

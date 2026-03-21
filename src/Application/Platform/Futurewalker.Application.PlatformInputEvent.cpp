// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformInputEvent.hpp"

namespace FW_DETAIL_NS
{
auto PlatformInputEvent::BeforeInsertText::GetCancel() const -> Bool
{
    return _cancel;
}

auto PlatformInputEvent::BeforeInsertText::SetCancel(Bool cancel) -> void
{
    _cancel = cancel;
}

auto PlatformInputEvent::BeforeInsertText::GetText() const -> String
{
    return _text;
}

auto PlatformInputEvent::BeforeInsertText::SetText(String const& text) -> void
{
    _text = text;
}

auto PlatformInputEvent::InsertText::GetText() const -> String
{
    return _text;
}

auto PlatformInputEvent::InsertText::SetText(String const& text) -> void
{
    _text = text;
}

auto PlatformInputEvent::BeforeInsertCompositionText::GetText() const -> String
{
    return _text;
}

auto PlatformInputEvent::BeforeInsertCompositionText::SetText(String const& text) -> void
{
    _text = text;
}

auto PlatformInputEvent::InsertCompositionText::GetText() const -> String
{
    return _text;
}

auto PlatformInputEvent::InsertCompositionText::SetText(String const& text) -> void
{
    _text = text;
}

auto PlatformInputEvent::BeforeInsertLineBreak::GetCancel() const -> Bool
{
    return _cancel;
}

auto PlatformInputEvent::BeforeInsertLineBreak::SetCancel(Bool cancel) -> void
{
    _cancel = cancel;
}

auto PlatformInputEvent::BeforeDeleteSurroundingText::GetCancel() const -> Bool
{
    return _cancel;
}

auto PlatformInputEvent::BeforeDeleteSurroundingText::SetCancel(Bool cancel) -> void
{
    _cancel = cancel;
}

auto PlatformInputEvent::BeforeDeleteSurroundingText::GetBefore() const -> CodePoint
{
    return _before;
}

auto PlatformInputEvent::BeforeDeleteSurroundingText::SetBefore(CodePoint before) -> void
{
    _before = before;
}

auto PlatformInputEvent::BeforeDeleteSurroundingText::GetAfter() const -> CodePoint
{
    return _after;
}

auto PlatformInputEvent::BeforeDeleteSurroundingText::SetAfter(CodePoint after) -> void
{
    _after = after;
}

auto PlatformInputEvent::DeleteSurroundingText::GetBefore() const -> CodePoint
{
    return _before;
}

auto PlatformInputEvent::DeleteSurroundingText::SetBefore(CodePoint before) -> void
{
    _before = before;
}

auto PlatformInputEvent::DeleteSurroundingText::GetAfter() const -> CodePoint
{
    return _after;
}

auto PlatformInputEvent::DeleteSurroundingText::SetAfter(CodePoint after) -> void
{
    _after = after;
}
}

// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.InputEvent.hpp"

namespace FW_DETAIL_NS
{
auto InputEvent::InsertText::GetCancel() const -> Bool
{
    return _cancel;
}

auto InputEvent::InsertText::SetCancel(Bool cancel) -> void
{
    _cancel = cancel;
}

auto InputEvent::InsertText::GetText() const -> String
{
    return _text;
}

auto InputEvent::InsertText::SetText(String const& text) -> void
{
    _text = text;
}

auto InputEvent::DeleteSurroundingText::GetCancel() const -> Bool
{
    return _cancel;
}

auto InputEvent::DeleteSurroundingText::SetCancel(Bool cancel) -> void
{
    _cancel = cancel;
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

auto InputEvent::InsertCompositionText::GetText() const -> String
{
    return _text;
}

auto InputEvent::InsertCompositionText::SetText(String const& text) -> void
{
    _text = text;
}
}

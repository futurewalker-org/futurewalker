// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.InputEvent.hpp"

namespace FW_DETAIL_NS
{
auto InputEvent::Attach::GetInputMethod() const -> Shared<InputMethod>
{
    return _inputMethod;
}

auto InputEvent::Attach::SetInputMethod(Shared<InputMethod> const& inputMethod) -> void
{
    _inputMethod = inputMethod;
}

auto InputEvent::InsertText::GetText() const -> String
{
    return _text;
}

auto InputEvent::InsertText::SetText(String const& text) -> void
{
    _text = text;
}

auto InputEvent::DeleteSurroundingText::GetBefore() const -> SInt64
{
    return _before;
}

auto InputEvent::DeleteSurroundingText::SetBefore(SInt64 before) -> void
{
    _before = before;
}

auto InputEvent::DeleteSurroundingText::GetAfter() const -> SInt64
{
    return _after;
}

auto InputEvent::DeleteSurroundingText::SetAfter(SInt64 after) -> void
{
    _after = after;
}
}

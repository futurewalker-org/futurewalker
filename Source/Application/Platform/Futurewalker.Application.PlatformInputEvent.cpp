// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformInputEvent.hpp"

namespace FW_DETAIL_NS
{
auto PlatformInputEvent::InsertText::GetText() const -> String
{
    return _text;
}

auto PlatformInputEvent::InsertText::SetText(String const& text) -> void
{
    _text = text;
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

auto PlatformInputEvent::InsertCompositionText::GetText() const -> String
{
    return _text;
}

auto PlatformInputEvent::InsertCompositionText::SetText(String const& text) -> void
{
    _text = text;
}
}

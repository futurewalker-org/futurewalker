// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformWindowEvent.hpp"

namespace FW_DETAIL_NS
{
auto PlatformWindowEvent::CloseRequested::IsCancelled() const -> Bool
{
    return _cancelled;
}

auto PlatformWindowEvent::CloseRequested::SetCancelled(Bool const cancelled) -> void
{
    _cancelled = cancelled;
}

auto PlatformWindowEvent::VisibleChanged::IsVisible() const -> Bool
{
    return _visible;
}

auto PlatformWindowEvent::VisibleChanged::SetVisible(Bool const visible) -> void
{
    _visible = visible;
}

auto PlatformWindowEvent::ActiveChanged::IsActive() const -> Bool
{
    return _active;
}

auto PlatformWindowEvent::ActiveChanged::SetActive(Bool const active) -> void
{
    _active = active;
}

auto PlatformWindowEvent::FocusedChanged::IsFocused() const -> Bool
{
    return _focused;
}

auto PlatformWindowEvent::FocusedChanged::SetFocused(Bool const focused) -> void
{
    _focused = focused;
}
}


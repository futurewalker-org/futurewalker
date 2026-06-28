// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformApplicationEvent.hpp"

namespace FW_DETAIL_NS
{
auto PlatformApplicationEvent::ExitRequested::IsCancelled() const -> Bool
{
    return _cancelled;
}

auto PlatformApplicationEvent::ExitRequested::SetCancelled(Bool const cancelled) -> void
{
    _cancelled = cancelled;
}

auto PlatformApplicationEvent::ActiveChanged::IsActive() const -> Bool
{
    return _active;
}

auto PlatformApplicationEvent::ActiveChanged::SetActive(Bool const active) -> void
{
    _active = active;
}

auto PlatformApplicationEvent::ForegroundChanged::IsForeground() const -> Bool
{
    return _foreground;
}

auto PlatformApplicationEvent::ForegroundChanged::SetForeground(Bool const foreground) -> void
{
    _foreground = foreground;
}
}
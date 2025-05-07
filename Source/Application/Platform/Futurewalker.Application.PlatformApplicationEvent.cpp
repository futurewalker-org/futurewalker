// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.PlatformApplicationEvent.hpp"

namespace FW_DETAIL_NS
{
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
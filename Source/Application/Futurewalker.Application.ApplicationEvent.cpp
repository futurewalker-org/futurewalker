// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.ApplicationEvent.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
auto ApplicationEvent::ActiveChanged::IsActive() const -> Bool
{
    return _active;
}

///
/// @brief
///
auto ApplicationEvent::ActiveChanged::SetActive(Bool const active) -> void
{
    _active = active;
}

///
/// @brief
///
auto ApplicationEvent::ExitRequested::IsCancelled() const -> Bool
{
    return _cancelled;
}

///
/// @brief
///
auto ApplicationEvent::ExitRequested::SetCancelled(Bool const cancelled) -> void
{
    _cancelled = cancelled;
}

///
/// @brief
///
auto ApplicationEvent::ForegroundChanged::IsForeground() const -> Bool
{
    return _foreground;
}

///
/// @brief
///
/// @param foreground
///
auto ApplicationEvent::ForegroundChanged::SetForeground(Bool const foreground) -> void
{
    _foreground = foreground;
}
}

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
}


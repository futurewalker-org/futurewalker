// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformFrameEvent.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
auto PlatformFrameEvent::Tick::GetTargetTimestamp() const -> MonotonicTime
{
    return _targetTimestamp;
}

///
/// @brief
///
auto PlatformFrameEvent::Tick::SetTargetTimestamp(MonotonicTime const& targetTimestamp) -> void
{
    _targetTimestamp = targetTimestamp;
}
}


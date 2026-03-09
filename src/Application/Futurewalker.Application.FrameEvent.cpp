// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.FrameEvent.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
auto FrameEvent::Tick::GetTargetTimestamp() const -> MonotonicTime
{
    return _targetTimestamp;
}

///
/// @brief
///
auto FrameEvent::Tick::SetTargetTimestamp(MonotonicTime const& targetTimestamp) -> void
{
    _targetTimestamp = targetTimestamp;
}
}

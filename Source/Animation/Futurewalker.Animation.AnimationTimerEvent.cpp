// SPDX-License-Identifier: MIT

#include "Futurewalker.Animation.AnimationTimerEvent.hpp"

namespace FW_DETAIL_NS
{
auto AnimationTimerEvent::Tick::GetTime() const -> MonotonicTime
{
    return _time;
}

auto AnimationTimerEvent::Tick::SetTime(MonotonicTime const& time) -> void
{
    _time = time;
}
}

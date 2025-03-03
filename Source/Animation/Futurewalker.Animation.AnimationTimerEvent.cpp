// SPDX-License-Identifier: MIT

#include "Futurewalker.Animation.AnimationTimerEvent.hpp"

namespace FW_DETAIL_NS
{
auto AnimationTimerEvent::Tick::GetTime() const -> MonotonicTime
{
    return _time;
}

auto AnimationTimerEvent::Tick::SetTime(const MonotonicTime& time) -> void
{
    _time = time;
}
}

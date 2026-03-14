// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Animation.AnimationTickerEvent.hpp"

namespace FW_DETAIL_NS
{
auto AnimationTickerEvent::Tick::GetTime() const -> MonotonicTime
{
    return _time;
}

auto AnimationTickerEvent::Tick::SetTime(MonotonicTime const& time) -> void
{
    _time = time;
}
}

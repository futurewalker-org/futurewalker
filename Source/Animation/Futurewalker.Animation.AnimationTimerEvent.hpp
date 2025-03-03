// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Animation.AnimationTimerEventType.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

#include "Futurewalker.Core.MonotonicTime.hpp" 

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
class AnimationTimerEvent : public EventParameter
{
public:
    class ActiveChanged;
    class EnabledChanged;
    class Tick;
};

///
/// @brief
///
class AnimationTimerEvent::ActiveChanged final : public AnimationTimerEvent 
{
};

///
/// @brief
///
class AnimationTimerEvent::EnabledChanged final : public AnimationTimerEvent
{
};

///
/// @brief
///
class AnimationTimerEvent::Tick : public AnimationTimerEvent
{
public:
    auto GetTime() const -> MonotonicTime;
    auto SetTime(const MonotonicTime& time) -> void;

private:
    MonotonicTime _time;
};
}
}

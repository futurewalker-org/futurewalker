// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Animation.AnimationTickerEventType.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

#include "Futurewalker.Core.MonotonicTime.hpp" 

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
class AnimationTickerEvent : public EventParameter
{
public:
    class ActiveChanged;
    class EnabledChanged;
    class Tick;
};

///
/// @brief
///
class AnimationTickerEvent::ActiveChanged final : public AnimationTickerEvent 
{
};

///
/// @brief
///
class AnimationTickerEvent::EnabledChanged final : public AnimationTickerEvent
{
};

///
/// @brief
///
class AnimationTickerEvent::Tick : public AnimationTickerEvent
{
public:
    auto GetTime() const -> MonotonicTime;
    auto SetTime(MonotonicTime const& time) -> void;

private:
    MonotonicTime _time;
};
}
}

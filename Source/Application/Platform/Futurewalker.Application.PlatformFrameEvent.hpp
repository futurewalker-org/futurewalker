// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.PlatformFrameEventType.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

#include "Futurewalker.Core.MonotonicTime.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformFrameEvent : public EventParameter
{
public:
    PlatformFrameEvent() = default;
    PlatformFrameEvent(PlatformFrameEvent const&) = default;
    PlatformFrameEvent& operator=(PlatformFrameEvent const&) = default;

public:
    class Tick;
};

class PlatformFrameEvent::Tick final : public PlatformFrameEvent
{
public:
    auto GetTargetTimestamp() const -> MonotonicTime;
    auto SetTargetTimestamp(MonotonicTime const& targetTimestamp) -> void;

private:
    MonotonicTime _targetTimestamp;
};

}
}

// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.FrameEventType.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

#include "Futurewalker.Core.MonotonicTime.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Frame event.
///
class FrameEvent : public EventParameter
{
public:
    class Tick;
};

///
/// @brief Frame tick event.
///
class FrameEvent::Tick final : public FrameEvent
{
public:
    auto GetTargetTimestamp() const -> MonotonicTime;
    auto SetTargetTimestamp(MonotonicTime const& targetTimestamp) -> void;

private:
    MonotonicTime _targetTimestamp;
};
}
}

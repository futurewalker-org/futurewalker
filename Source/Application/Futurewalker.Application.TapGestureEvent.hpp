// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.TapGestureEventType.hpp"
#include "Futurewalker.Application.GestureEvent.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Tap gesture event.
///
class TapGestureEvent : public GestureEvent
{
public:
    explicit TapGestureEvent(const TapGestureEventType eventType);

    auto GetEventType() const -> TapGestureEventType;
    auto SetEventType(const TapGestureEventType eventType) -> void;

private:
    TapGestureEventType _eventType;
};
}
}

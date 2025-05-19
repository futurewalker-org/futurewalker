// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.TapGestureViewEventType.hpp"
#include "Futurewalker.Application.TapGestureEventType.hpp" 
#include "Futurewalker.Application.ViewEvent.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class TapGestureViewEvent : public ViewEvent::Notify
{
public:
    explicit TapGestureViewEvent(const TapGestureEventType eventType);

    auto GetEventType() const -> TapGestureEventType;
    auto SetEventType(const TapGestureEventType eventType) -> void;

private:
    TapGestureEventType _eventType;
};
}
}

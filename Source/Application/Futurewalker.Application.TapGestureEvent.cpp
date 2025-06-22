// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.TapGestureEvent.hpp"

namespace FW_DETAIL_NS
{
TapGestureEvent::TapGestureEvent(const TapGestureEventType eventType)
  : _eventType {eventType}
{
}

auto TapGestureEvent::GetEventType() const -> TapGestureEventType
{
    return _eventType;
}

auto TapGestureEvent::SetEventType(const TapGestureEventType eventType) -> void
{
    _eventType = eventType;
}
}

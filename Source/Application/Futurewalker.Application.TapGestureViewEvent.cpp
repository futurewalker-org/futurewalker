// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.TapGestureViewEvent.hpp"

namespace FW_DETAIL_NS
{
TapGestureViewEvent::TapGestureViewEvent(const TapGestureEventType eventType)
  : _eventType {eventType}
{
}

auto TapGestureViewEvent::GetEventType() const -> TapGestureEventType
{
    return _eventType;
}

auto TapGestureViewEvent::SetEventType(const TapGestureEventType eventType) -> void
{
    _eventType = eventType;
}
}

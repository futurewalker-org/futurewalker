// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.HoverGestureEventType.hpp"
#include "Futurewalker.Application.GestureEvent.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Hover gesture event.
///
class HoverGestureEvent : public GestureEvent
{
public:
    class Enter;
    class Leave;
};

///
/// @brief Enterring hover state.
///
class HoverGestureEvent::Enter final : public HoverGestureEvent
{
};

///
/// @brief Leaving hover state.
///
class HoverGestureEvent::Leave final : public HoverGestureEvent
{
};
}
}

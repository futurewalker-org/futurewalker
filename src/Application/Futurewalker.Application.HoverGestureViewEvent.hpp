// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.HoverGestureViewEventType.hpp"
#include "Futurewalker.Application.HoverGestureEventType.hpp" 
#include "Futurewalker.Application.ViewEvent.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class HoverGestureViewEvent : public ViewEvent::Notify
{
public:
    class Enter;
    class Leave;
};

class HoverGestureViewEvent::Enter final : public HoverGestureViewEvent
{
};

class HoverGestureViewEvent::Leave final : public HoverGestureViewEvent
{
};
}
}

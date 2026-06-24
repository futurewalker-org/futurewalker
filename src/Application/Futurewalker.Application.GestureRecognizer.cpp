// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.GestureRecognizer.hpp" 

#include "Futurewalker.Event.EventReceiver.hpp"

namespace FW_DETAIL_NS
{
GestureRecognizer::GestureRecognizer()
{
    _eventReceiver = EventReceiver::Make();
}

GestureRecognizer::~GestureRecognizer() = default;

auto GestureRecognizer::SendEvent(Event<>& event) -> Bool
{
    return _eventReceiver->SendEvent(event);
}

auto GestureRecognizer::GetEventReceiver() -> EventReceiver&
{
    return *_eventReceiver;
}

auto GestureRecognizer::GetEventReceiver() const -> EventReceiver const&
{
    return *_eventReceiver;
}
}

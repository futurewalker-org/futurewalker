// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.GestureRecognizer.hpp" 

#include "Futurewalker.Event.EventReceiver.hpp"

namespace FW_DETAIL_NS
{
GestureRecognizer::GestureRecognizer()
{
    _eventReceiver = EventReceiver::Make();
}

GestureRecognizer::~GestureRecognizer() = default;

auto GestureRecognizer::SendEvent(Event& event) -> Async<Bool>
{
    co_return co_await _eventReceiver->SendEvent(event);
}

auto GestureRecognizer::SendEventDetached(Event& event) -> Bool
{
    return _eventReceiver->SendEventDetached(event);
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

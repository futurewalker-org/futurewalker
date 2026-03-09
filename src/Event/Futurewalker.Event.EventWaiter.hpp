// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Event.EventWaiterType.hpp"
#include "Futurewalker.Event.EventReceiver.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"

#include "Futurewalker.Async.AsyncBroadcastChannel.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Event waiter.
///
class EventWaiter : NonCopyable
{
public:
    template <class Receiver>
    explicit EventWaiter(Receiver& receiver)
    {
        _tracker = Tracker::Make();
        EventReceiver::Connect(receiver, *this, &EventWaiter::ReceiveEvent);
    }

    auto GetTracker() -> Tracker&;
    auto GetTracker() const -> Tracker const&;

    template <class T>
    auto Wait() -> Async<Event<T>>
    {
        while (true)
        {
            auto const event = co_await WaitEvent();
            if (event.template Is<T>())
            {
                co_return event.template As<T>();
            }
        }
    }

private:
    auto WaitEvent() -> Async<Event<>>;
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;

private:
    Shared<Tracker> _tracker;
    AsyncBroadcastChannel<Event<>> _channel;
};
}
}

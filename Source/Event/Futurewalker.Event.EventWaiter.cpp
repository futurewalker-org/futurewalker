// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Event.EventWaiter.hpp"

#include "Futurewalker.Async.ThisThread.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Wait for next event.
///
auto EventWaiter::WaitEvent() -> Async<Event<>>
{
    co_return co_await ThisThread::Resume(_channel.Read());
}

///
/// @brief Get tracker.
///
auto EventWaiter::GetTracker() -> Tracker&
{
    return *_tracker;
}

///
/// @brief Get tracker.
///
auto EventWaiter::GetTracker() const -> Tracker const&
{
    return *_tracker;
}

///
/// @brief Receive event.
///
auto EventWaiter::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    co_return _channel.Write(event);
}
}

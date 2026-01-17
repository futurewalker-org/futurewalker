// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Event.EventReceiver.hpp"

#include "Futurewalker.Async.AsyncFunction.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Send event to receiver.
///
/// @param[in] delegate Delegate.
///
/// @return Non-null EventReceiver object.
///
auto EventReceiver::Make(Delegate delegate) -> Unique<EventReceiver>
{
    return Unique<EventReceiver>::Make(PassKey<EventReceiver>(), std::move(delegate));
}

///
/// @brief Check if receiver has any connection.
///
auto EventReceiver::HasConnection() const -> Bool
{
    return !_eventSignal.IsEmpty();
}

///
/// @brief Send event to receiver.
///
EventReceiver::EventReceiver(PassKey<EventReceiver>, Delegate delegate)
  : _delegate {std::move(delegate)}
{
    _tracker = Tracker::Make();
}

///
/// @brief Send event to receiver.
///
/// @param[in, out] event Event to send.
///
auto EventReceiver::SendEvent(Event<>& event) -> Async<Bool>
{
    if (_delegate.dispatchEvent)
    {
        co_return co_await _delegate.dispatchEvent(event, [&](Event<>& event) -> Async<Bool> { co_return co_await DispatchEvent(event); });
    }
    co_return co_await DispatchEvent(event);
}

///
/// @brief Send event in a detached asynchronous context.
///
/// @param[in, out] event
///
/// @return When all event slots complate without suspending current coroutine, it returns result of event dispatch.
/// @return Retruns `false` otherwise.
///
auto EventReceiver::SendEventDetached(Event<>& event) -> Bool
{
    auto const r = Shared<Bool>::Make(false);
    auto const e = Shared<Event<>>::Make(event);
    AsyncFunction::SpawnFn([=, this]() -> Async<void> { *r = co_await SendEvent(*e); }).Detach();
    event = *e;
    return *r;
}

///
/// @brief Get tracker.
///
auto EventReceiver::GetTracker() -> Tracker&
{
    return *_tracker;
}

///
/// @brief Get tracker.
///
auto EventReceiver::GetTracker() const -> Tracker const&
{
    return *_tracker;
}

///
/// @brief Get EventReceiver instance.
///
auto EventReceiver::GetEventReceiver() -> EventReceiver&
{
    return *this;
}

///
/// @brief Get EventReceiver instance.
///
auto EventReceiver::GetEventReceiver() const -> EventReceiver const&
{
    return *this;
}

///
/// @brief Dispatch event to connected objects.
///
/// @param[in] event Event to dispatch.
///
/// @note This function is intended to be used from delegate.
///
auto EventReceiver::DispatchEvent(Event<>& event) -> Async<Bool>
{
    co_return co_await _eventSignal(event);
}
}

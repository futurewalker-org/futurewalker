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
    return Unique<EventReceiver>::Make(std::move(delegate));
}

///
/// @brief Check if receiver has any connection.
///
auto EventReceiver::HasConnection() const -> Bool
{
    return !_eventSignal.IsEmpty();
}

///
/// @brief Disconnect all connections.
///
auto EventReceiver::DisconnectAll() -> void
{
    _eventSignal.DisconnectAll();
}

///
/// @brief Send event to receiver.
///
EventReceiver::EventReceiver(Delegate delegate)
  : _delegate {std::move(delegate)}
{
}

///
/// @brief Send event to receiver.
///
/// @param[in, out] event Event to send.
///
auto EventReceiver::SendEvent(Event<>& event) -> Bool
{
    if (_delegate.dispatchEvent)
    {
        return _delegate.dispatchEvent(event, [&](Event<>& event) -> Bool { return DispatchEvent(event); });
    }
    return DispatchEvent(event);
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
auto EventReceiver::DispatchEvent(Event<>& event) -> Bool
{
    return _eventSignal(event);
}

///
/// @brief Get reference to signal if exists, or create new one.
///
auto EventReceiver::GetSignal() -> EventSignal&
{
    return _eventSignal;
}
}

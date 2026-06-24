// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Event.Event.hpp"
#include "Futurewalker.Event.EventSignal.hpp"

#include "Futurewalker.Signal.SignalConnection.hpp"
#include "Futurewalker.Signal.SignalFunction.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Function.hpp"
#include "Futurewalker.Core.Optional.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Event receiver class.
///
/// EventReceiver object accepts event and dispatches it to observers.
///
class EventReceiver : NonCopyable
{
public:
    struct Delegate
    {
        Function<Bool(Event<>& event, EventFunction const& dispatch)> dispatchEvent;
    };
    static auto Make(Delegate delegate = {}) -> Unique<EventReceiver>;

    template <class Receiver, class Function>
    static auto Connect(Receiver& receiver, Function&& function) -> SignalConnection;

    template <class Receiver, class Observer, class Function>
    static auto Connect(Receiver& receiver, Observer&& observer, Function&& function) -> SignalConnection;

    auto HasConnection() const -> Bool;
    auto DisconnectAll() -> void;

public:
    explicit EventReceiver(Delegate delegate = {});

    auto SendEvent(Event<>& event) -> Bool;

    auto GetEventReceiver() -> EventReceiver&;
    auto GetEventReceiver() const -> EventReceiver const&;

private:
    auto DispatchEvent(Event<>& event) -> Bool;
    auto GetSignal() -> EventSignal&;

private:
    Delegate _delegate;
    EventSignal _eventSignal;
};

///
/// @brief Connect event handler to receiver.
///
/// @param[in] receiver Event receiver object.
/// @param[in] function Event handler function (callback).
///
template <class Receiver, class Function>
auto EventReceiver::Connect(Receiver& receiver, Function&& function) -> SignalConnection
{
    auto& eventReceiver = receiver.GetEventReceiver();
    return SignalFunction::Connect(eventReceiver.GetSignal(), std::forward<Function>(function));
}

///
/// @brief Connect event handler to receiver.
///
/// @param[in] receiver Event receiver object.
/// @param[in] observer Observer object.
/// @param[in] function Event handler function (callback).
///
template <class Receiver, class Observer, class Function>
auto EventReceiver::Connect(Receiver& receiver, Observer&& observer, Function&& function) -> SignalConnection
{
    auto& eventReceiver = receiver.GetEventReceiver();
    return SignalFunction::Connect(eventReceiver.GetSignal(), observer, std::forward<Function>(function));
}
}
}

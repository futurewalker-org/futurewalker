// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Event.Event.hpp"
#include "Futurewalker.Event.EventSignal.hpp"

#include "Futurewalker.Signal.Tracker.hpp"
#include "Futurewalker.Signal.SignalConnection.hpp"
#include "Futurewalker.Signal.SignalFunction.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Function.hpp"

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
        Function<Async<Bool>(Event& event, EventFunction const& dispatch)> dispatchEvent;
    };
    static auto Make(Delegate delegate = {}) -> Unique<EventReceiver>;

    template <class Receiver, class Function>
    static auto Connect(Receiver& receiver, Function&& function) -> SignalConnection;

    template <class Receiver, class Observer, class Function>
    static auto Connect(Receiver& receiver, Observer&& observer, Function&& function) -> SignalConnection;

public:
    EventReceiver(PassKey<EventReceiver>, Delegate delegate);

    auto SendEvent(Event& event) -> Async<Bool>;
    auto SendEventDetached(Event& event) -> Bool;

    auto GetTracker() -> Tracker&;
    auto GetTracker() const -> Tracker const&;

    auto GetEventReceiver() -> EventReceiver&;
    auto GetEventReceiver() const -> EventReceiver const&;

private:
    auto DispatchEvent(Event& event) -> Async<Bool>;

private:
    Delegate _delegate;
    Shared<Tracker> _tracker;
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
    auto slot = [function](Event& event) -> Lazy<Bool> { return AsyncFunction::Fn([=, &event]() -> Lazy<Bool> { co_return co_await std::invoke(function, event); }); };
    return SignalFunction::Connect(eventReceiver._eventSignal, std::move(slot), SignalConnectPosition::front);
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
    auto slot = [function]<class T>(T&& observer, Event& event) -> Lazy<Bool> {
        // Observer may be destroyed when returned task is awaited, so we need to track it.
        // Function also need to be copied since slot can be destroyed.
        auto tracker = Tracker::Track(observer);
        return AsyncFunction::Fn([=, &observer, &event]() -> Lazy<Bool> {
            if (!tracker.IsExpired())
            {
                co_return co_await std::invoke(function, std::forward<T>(observer), event);
            }
            co_return false;
        });
    };
    return SignalFunction::Connect(eventReceiver._eventSignal, observer, std::move(slot), SignalConnectPosition::front);
}
}
}

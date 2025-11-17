// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Action.Action.hpp"

#include "Futurewalker.Event.Event.hpp"
#include "Futurewalker.Event.EventReceiver.hpp"

namespace FW_DETAIL_NS
{
auto Action::Make() -> Shared<Action>
{
    return Shared<Action>::Make(PassKey<Action>());
}

Action::Action(PassKey<Action>)
{
    _eventReceiver = EventReceiver::Make();
}

auto Action::Execute() -> void
{
    auto event = Event<ActionEvent::Execute>();
    GetEventReceiver().SendEventDetached(event);
}

auto Action::State(Bool& enabled, Bool& toggled) -> Bool
{
    auto event = Event<ActionEvent::State>();
    if (GetEventReceiver().SendEventDetached(event))
    {
        if (event.Is<ActionEvent::State>())
        {
            auto state = event.As<ActionEvent::State>();
            enabled = state->IsEnabled();
            toggled = state->IsToggled();
        }
        return true;
    }
    return false;
}

auto Action::NotifyStateChanged() -> void
{
    auto event = Event<ActionEvent::StateChanged>();
    GetEventReceiver().SendEventDetached(event);
}

auto Action::GetTracker() -> Tracker&
{
    return _eventReceiver->GetTracker();
}

auto Action::GetTracker() const -> Tracker const&
{
    return _eventReceiver->GetTracker();
}

auto Action::GetEventReceiver() -> EventReceiver&
{
    return *_eventReceiver;
}

auto Action::GetEventReceiver() const -> EventReceiver const&
{
    return *_eventReceiver;
}
}

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

auto Action::Execute(CommandId const command) -> void
{
    auto eventParameter = Event<ActionEvent::Execute>();
    eventParameter->SetCommandId(command);
    auto event = Event<>(std::move(eventParameter));
    GetEventReceiver().SendEventDetached(event);
}

auto Action::State(CommandId const command, Bool& enabled, Bool& toggled) -> Bool
{
    auto eventParameter = Event<ActionEvent::State>();
    eventParameter->SetCommandId(command);
    auto event = Event<>(std::move(eventParameter));
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
    auto event = Event<>(Event<ActionEvent::StateChanged>());
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

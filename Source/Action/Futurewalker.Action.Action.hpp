// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Action.ActionType.hpp"
#include "Futurewalker.Action.ActionEvent.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.PassKey.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
class Action : NonCopyable
{
public:
    static auto Make() -> Shared<Action>;

    template <class Function>
    static auto Make(Function&& function) -> Shared<Action>;

    template <class Receiver, class Function>
    static auto Make(Receiver& receiver, Function&& function) -> Shared<Action>;

    Action(PassKey<Action>);

    auto Execute(CommandId const command) -> void;
    auto State(CommandId const command, Bool& enabled, Bool& toggled) -> Bool;

    auto NotifyStateChanged() -> void;

    auto GetTracker() -> Tracker&;
    auto GetTracker() const -> Tracker const&;

    auto GetEventReceiver() -> EventReceiver&;
    auto GetEventReceiver() const -> EventReceiver const&;

private:
    Unique<EventReceiver> _eventReceiver;
};

///
/// @brief
///
/// @param function
///
template <class Function>
auto Action::Make(Function&& function) -> Shared<Action>
{
    auto action = Make();
    EventReceiver::Connect(*action, std::forward<Function>(function));
    return action;
}

///
/// @brief
///
/// @param receiver
/// @param function
///
template <class Receiver, class Function>
auto Action::Make(Receiver& receiver, Function&& function) -> Shared<Action>
{
    auto action = Make();
    EventReceiver::Connect(*action, receiver, std::forward<Function>(function));
    return action;
}
}
}

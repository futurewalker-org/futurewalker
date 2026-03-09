// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Action.ActionEventType.hpp"
#include "Futurewalker.Action.CommandId.hpp" 

#include "Futurewalker.Event.EventParameter.hpp"

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class ActionEvent : public EventParameter
{
    CommandId _commandId = 0U;

public:
    auto GetCommandId() const -> CommandId;
    auto SetCommandId(CommandId const commandId) -> void;

public:
    class StateChanged;
    class State;
    class Execute;
};

class ActionEvent::StateChanged final : public ActionEvent
{
};

class ActionEvent::State final : public ActionEvent
{
public:
    auto IsEnabled() const -> Bool;
    auto SetEnabled(Bool const enabled) -> void;

    auto IsToggled() const -> Bool;
    auto SetToggled(Bool const toggled) -> void;

private:
    Bool _enabled = false;
    Bool _toggled = false;
};

class ActionEvent::Execute final : public ActionEvent
{
};
}
}

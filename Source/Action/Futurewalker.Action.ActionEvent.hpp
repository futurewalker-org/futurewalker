// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Action.ActionEventType.hpp"
#include "Futurewalker.Action.CommandParameter.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class ActionEvent : public EventParameter
{
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
    auto IsEnabled() const -> Bool
    {
        return _enabled;
    }

    auto SetEnabled(Bool const enabled) -> void
    {
        _enabled = enabled;
    }

    auto IsToggled() const -> Bool
    {
        return _toggled;
    }

    auto SetToggled(Bool const toggled) -> void
    {
        _toggled = toggled;
    }

private:
    Bool _enabled = false;
    Bool _toggled = false;
};

class ActionEvent::Execute final : public ActionEvent
{
public:
    auto GetParameter() const -> CommandParameter const&
    {
        return _parameter;
    }

    auto SetParameter(CommandParameter const& parameter) -> void
    {
        _parameter = parameter;
    }

private:
    CommandParameter _parameter;
};
}
}

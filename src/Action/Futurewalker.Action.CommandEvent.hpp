// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Action.CommandEventType.hpp"
#include "Futurewalker.Action.CommandId.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class CommandEvent : public EventParameter
{
public:
    class StateChanged;
};

class CommandEvent::StateChanged final : public CommandEvent
{
public:
    auto GetCommandId() const -> CommandId;
    auto SetCommandId(CommandId const& commandId) -> void;

private:
    CommandId _commandId = 0U;
};
}
}

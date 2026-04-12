// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.Prelude.hpp"

#include "Futurewalker.Application.ViewEvent.hpp"

#include "Futurewalker.Action.CommandId.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
class MenuViewEvent : public ViewEvent::Notify
{
public:
    class Activated;
};

class MenuViewEvent::Activated : public MenuViewEvent
{
public:
    auto GetCommandId() const -> CommandId;
    auto SetCommandId(CommandId const& commandId) -> void;

private:
    CommandId _commandId = 0U;
};
}
}

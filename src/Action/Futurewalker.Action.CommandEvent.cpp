// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Action.CommandEvent.hpp"

namespace FW_DETAIL_NS
{
auto CommandEvent::StateChanged::GetCommandId() const -> CommandId
{
    return _commandId;
}

auto CommandEvent::StateChanged::SetCommandId(CommandId const& commandId) -> void
{
    _commandId = commandId;
}
}

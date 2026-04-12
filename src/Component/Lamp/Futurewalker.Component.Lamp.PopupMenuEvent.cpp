// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.PopupMenuEvent.hpp"

namespace FW_LAMP_DETAIL_NS
{
auto PopupMenuEvent::Activated::GetCommandId() const -> CommandId
{
    return _commandId;
}

auto PopupMenuEvent::Activated::SetCommandId(CommandId const& commandId) -> void
{
    _commandId = commandId;
}
}

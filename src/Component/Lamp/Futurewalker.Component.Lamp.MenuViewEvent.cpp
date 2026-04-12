// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.MenuViewEvent.hpp"

namespace FW_LAMP_DETAIL_NS
{
auto MenuViewEvent::Activated::GetCommandId() const -> CommandId
{
    return _commandId;
}

auto MenuViewEvent::Activated::SetCommandId(CommandId const& commandId) -> void
{
    _commandId = commandId;
}
}

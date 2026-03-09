// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Action.ActionEvent.hpp"

namespace FW_DETAIL_NS
{
auto ActionEvent::GetCommandId() const -> CommandId
{
    return _commandId;
}

auto ActionEvent::SetCommandId(CommandId const commandId) -> void
{
    _commandId = commandId;
}

auto ActionEvent::State::IsEnabled() const -> Bool
{
    return _enabled;
}

auto ActionEvent::State::SetEnabled(Bool const enabled) -> void
{
    _enabled = enabled;
}

auto ActionEvent::State::IsToggled() const -> Bool
{
    return _toggled;
}

auto ActionEvent::State::SetToggled(Bool const toggled) -> void
{
    _toggled = toggled;
}
}

// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Action.RootCommandNode.hpp"
#include "Futurewalker.Action.CommandDispatcher.hpp"

namespace FW_DETAIL_NS
{
auto RootCommandNode::Make() -> Shared<RootCommandNode>
{
    return CommandNode::MakeDerived<RootCommandNode>();
}

RootCommandNode::RootCommandNode(PassKey<CommandNode> key)
  : CommandNode(key)
{
}

auto RootCommandNode::RootIsEnabled(CommandId const& command) -> Bool
{
    return CommandDispatcher::IsEnabled(command);
}

auto RootCommandNode::RootIsToggled(CommandId const& command) -> Bool
{
    return CommandDispatcher::IsToggled(command);
}

auto RootCommandNode::RootExecute(CommandId const& command) -> Bool
{
    return CommandDispatcher::Execute(command);
}
}

// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Action.CommandNode.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class RootCommandNode : public CommandNode
{
public:
    static auto Make() -> Shared<RootCommandNode>;

    RootCommandNode(PassKey<CommandNode> key);

private:
    auto RootIsEnabled(CommandId const& command) -> Bool override;
    auto RootIsToggled(CommandId const& command) -> Bool override;
    auto RootExecute(CommandId const& command) -> Bool override;
};
}
}

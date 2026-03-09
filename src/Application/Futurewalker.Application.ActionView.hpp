// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.ActionViewType.hpp"
#include "Futurewalker.Application.View.hpp" 

#include "Futurewalker.Action.CommandId.hpp"
#include "Futurewalker.Action.ActionType.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class ActionView : public View
{
public:
    static auto Make() -> Shared<ActionView>;
    static auto MakeWithContent(Shared<View> contentView) -> Shared<ActionView>;

    ActionView(PassKey<View> key);

    auto GetContent() -> Shared<View>;
    auto SetContent(Shared<View> content) -> void;

    auto AddAction(CommandId const& command, Shared<Action> const& action) -> void;
    auto RemoveAction(CommandId const& command) -> void;
};
}
}

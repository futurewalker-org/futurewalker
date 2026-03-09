// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Action.CommandDispatcherType.hpp"
#include "Futurewalker.Action.CommandDispatcherContextType.hpp"
#include "Futurewalker.Action.CommandId.hpp"
#include "Futurewalker.Action.CommandObserver.hpp"
#include "Futurewalker.Action.ActionType.hpp"

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.HashMap.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class CommandDispatcher
{
public:
    static auto Make() -> Unique<CommandDispatcher>;

    static auto Execute(CommandId const& command) -> Bool;

    static auto IsEnabled(CommandId const& command) -> Bool;
    static auto IsToggled(CommandId const& command) -> Bool;

    static auto GetObserver(CommandId const& command) -> Unique<CommandObserver>;

    CommandDispatcher();
    ~CommandDispatcher();

    auto Attach() -> void;
    auto Detach() -> void;

    auto IsBlocking() const -> Bool;
    auto SetBlocking(Bool const blocking) -> void;

    auto AddAction(CommandId const& command, Shared<Action> const& action) -> void;
    auto RemoveAction(CommandId const& command) -> void;

private:
    auto InternalFindAction(CommandId const& command) -> Shared<Action>;
    auto InternalSendCommand(CommandId const& command) -> Bool;
    auto InternalNotifyStateChanged(CommandId const& command) -> void;

private:
    Shared<CommandDispatcherContext> _context;
    Bool _blocking = false;
    Pointer<CommandDispatcher> _prevDispatcher = nullptr;
    Pointer<CommandDispatcher> _nextDispatcher = nullptr;
    HashMap<CommandId, Shared<Action>> _actionMap;
};
}
}

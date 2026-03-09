// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Action.CommandDispatcherContextType.hpp"
#include "Futurewalker.Action.CommandDispatcherType.hpp"
#include "Futurewalker.Action.CommandObserver.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.Pointer.hpp"
#include "Futurewalker.Core.Memory.hpp"

namespace FW_DETAIL_NS
{
class CommandDispatcherContext : NonCopyable
{
public:
    static auto Make() -> Shared<CommandDispatcherContext>;

    CommandDispatcherContext(PassKey<CommandDispatcherContext>);

    auto GetFirstDispatcher(PassKey<CommandDispatcher>) const -> Pointer<CommandDispatcher>;
    auto SetFirstDispatcher(PassKey<CommandDispatcher>, Pointer<CommandDispatcher> dispatcher) -> void;

    auto GetObserver(PassKey<CommandDispatcher>, CommandId const& commandId) -> Unique<CommandObserver>;

    auto NotifyStateChanged(PassKey<CommandDispatcher>, CommandId const& commandId) -> void;

    auto GetCommandNode() -> CommandNode&;
    auto GetCommandNode() const -> CommandNode const&;

private:
    Weak<CommandDispatcherContext> _self;
    Unique<EventReceiver> _eventReceiver;
    Shared<CommandNode> _commandNode;
    Pointer<CommandDispatcher> _firstDispatcher = nullptr;
};

template <>
struct Locator::Resolver<CommandDispatcherContext>
{
    using Interface = CommandDispatcherContext;
    static auto Resolve() -> Shared<CommandDispatcherContext>;
};
}

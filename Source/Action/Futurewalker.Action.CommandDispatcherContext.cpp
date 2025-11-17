// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Action.CommandDispatcherContext.hpp"
#include "Futurewalker.Action.CommandEvent.hpp"
#include "Futurewalker.Action.CommandObserver.hpp"
#include "Futurewalker.Action.CommandNode.hpp"
#include "Futurewalker.Action.RootCommandNode.hpp"

namespace FW_DETAIL_NS
{
auto CommandDispatcherContext::Make() -> Shared<CommandDispatcherContext>
{
    auto context = Shared<CommandDispatcherContext>::Make(PassKey<CommandDispatcherContext>());
    context->_self = context;
    return context;
}

CommandDispatcherContext::CommandDispatcherContext(PassKey<CommandDispatcherContext>)
{
    _eventReceiver = EventReceiver::Make();
    _commandNode = RootCommandNode::Make();
}

auto CommandDispatcherContext::GetFirstDispatcher(PassKey<CommandDispatcher>) const -> Pointer<CommandDispatcher>
{
    return _firstDispatcher;
}

auto CommandDispatcherContext::SetFirstDispatcher(PassKey<CommandDispatcher>, Pointer<CommandDispatcher> dispatcher) -> void
{
    _firstDispatcher = dispatcher;
}

auto CommandDispatcherContext::GetObserver(PassKey<CommandDispatcher>, CommandId const& commandId) -> Unique<CommandObserver>
{
    return CommandNode::GetObserver(*_commandNode, commandId);
}

auto CommandDispatcherContext::NotifyStateChanged(PassKey<CommandDispatcher>, CommandId const& commandId) -> void
{
    _commandNode->NotifyStateChanged({}, commandId);
}

auto CommandDispatcherContext::GetCommandNode() -> CommandNode&
{
    return *_commandNode;
}

auto CommandDispatcherContext::GetCommandNode() const -> CommandNode const&
{
    return *_commandNode;
}

auto Locator::Resolver<CommandDispatcherContext>::Resolve() -> Shared<CommandDispatcherContext>
{
    return CommandDispatcherContext::Make();
}
}

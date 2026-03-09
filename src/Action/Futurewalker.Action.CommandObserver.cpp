// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Action.CommandObserver.hpp"
#include "Futurewalker.Action.CommandDispatcher.hpp"
#include "Futurewalker.Action.CommandNode.hpp"

namespace FW_DETAIL_NS
{
CommandObserver::CommandObserver(PassKey<CommandNode>, Shared<CommandNode> commandNode, CommandId const& commandId)
  : _commandNode {commandNode}
  , _commandId {commandId}
{
    _eventReceiver = EventReceiver::Make();
    EventReceiver::Connect(*commandNode, *this, &CommandObserver::ReceiveEvent);
}

auto CommandObserver::GetCommandId() const -> CommandId
{
    return _commandId;
}

auto CommandObserver::IsCommandEnabled() const -> Bool
{
    if (auto const node = _commandNode.Lock())
    {
        return CommandNode::IsEnabled(*node, _commandId);
    }
    return false;
}

auto CommandObserver::IsCommandToggled() const -> Bool
{
    if (auto const node = _commandNode.Lock())
    {
        return CommandNode::IsToggled(*node, _commandId);
    }
    return false;
}

auto CommandObserver::GetTracker() -> Tracker&
{
    return _eventReceiver->GetTracker();
}

auto CommandObserver::GetTracker() const -> Tracker const&
{
    return _eventReceiver->GetTracker();
}

auto CommandObserver::GetEventReceiver() -> EventReceiver&
{
    return *_eventReceiver;
}

auto CommandObserver::GetEventReceiver() const -> EventReceiver const&
{
    return *_eventReceiver;
}

auto CommandObserver::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<CommandEvent::StateChanged>())
    {
        auto const parameter = event.As<CommandEvent::StateChanged>();
        if (parameter->GetCommandId() == _commandId)
        {
            co_return co_await GetEventReceiver().SendEvent(event);
        }
    }
    co_return false;
}
}

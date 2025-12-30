// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Action.CommandNode.hpp"
#include "Futurewalker.Action.Action.hpp"
#include "Futurewalker.Action.CommandEvent.hpp"

namespace FW_DETAIL_NS
{
auto CommandNode::Make() -> Shared<CommandNode>
{
    return CommandNode::MakeDerived<CommandNode>();
}

CommandNode::CommandNode(PassKey<CommandNode>)
{
    _eventReceiver = EventReceiver::Make();
}

CommandNode::~CommandNode() noexcept = default;

auto CommandNode::AddChild(Shared<CommandNode> const& child) -> void
{
    if (child)
    {
        child->_parent = GetSelf();
        _children.push_back(child);
        child->InternalNotifyStateChanged();
    }
}

auto CommandNode::RemoveChild(Shared<CommandNode> const& child) -> void
{
    if (child)
    {
        _children.remove(child);
        child->_parent.Reset();
        child->InternalNotifyStateChanged();
    }
}

auto CommandNode::GetTracker() -> Tracker&
{
    return _eventReceiver->GetTracker();
}

auto CommandNode::GetTracker() const -> Tracker const&
{
    return _eventReceiver->GetTracker();
}

auto CommandNode::GetEventReceiver() -> EventReceiver&
{
    return *_eventReceiver;
}

auto CommandNode::GetEventReceiver() const -> EventReceiver const&
{
    return *_eventReceiver;
}

auto CommandNode::GetCommandNode() -> CommandNode&
{
    return *this;
}

auto CommandNode::GetCommandNode() const -> CommandNode const&
{
    return *this;
}

auto CommandNode::NotifyStateChanged(PassKey<CommandDispatcherContext>, CommandId const& command) -> void
{
    InternalNotifyStateChanged(command);
}

auto CommandNode::InitializeSelf(Shared<CommandNode> const& self) -> void
{
    _self = self;
}

auto CommandNode::GetSelf() -> Shared<CommandNode>
{
    return _self.Lock();
}

auto CommandNode::GetSelf() const -> Shared<CommandNode const>
{
    return _self.Lock();
}

auto CommandNode::GetParent() -> Shared<CommandNode>
{
    return _parent.Lock();
}

auto CommandNode::GetParent() const -> Shared<CommandNode const>
{
    return _parent.Lock();
}

auto CommandNode::InternalFindAction(CommandId const& command) -> Shared<Action>
{
    if (auto const it = _actionMap.find(command); it != _actionMap.end())
    {
        return it->second;
    }
    return nullptr;
}

auto CommandNode::InternalNotifyStateChanged(CommandId const& command) -> void
{
    if (auto it = _observerMap.find(command); it != _observerMap.end())
    {
        auto const& observers = it->second;
        if (std::any_of(observers.begin(), observers.end(), [](auto const& tracked) { return !tracked.IsExpired(); }))
        {
            auto stateEvent = Event<CommandEvent::StateChanged>();
            stateEvent->SetCommandId(command);
            auto event = Event<>(stateEvent);
            GetEventReceiver().SendEventDetached(event);
        }
    }

    for (auto const& child : _children)
    {
        child->InternalNotifyStateChanged(command);
    }
}

auto CommandNode::InternalNotifyStateChanged() -> void
{
    for (auto const& [command, observers] : _observerMap)
    {
        if (std::any_of(observers.begin(), observers.end(), [](auto const& tracked) { return !tracked.IsExpired(); }))
        {
            auto stateEvent = Event<CommandEvent::StateChanged>();
            stateEvent->SetCommandId(command);
            auto event = Event<>(stateEvent);
            GetEventReceiver().SendEventDetached(event);
        }
    }

    for (auto const& child : _children)
    {
        child->InternalNotifyStateChanged();
    }
}

auto CommandNode::InternalExecute(CommandId const& command) -> Bool
{
    if (auto const action = InternalFindAction(command))
    {
        auto enabled = False;
        auto toggled = False;
        if (action->State(enabled, toggled))
        {
            if (enabled)
            {
                action->Execute();
            }
            return true;
        }
    }

    if (auto parent = GetParent())
    {
        return parent->InternalExecute(command);
    }
    return RootExecute(command);
}

auto CommandNode::InternalIsEnabled(CommandId const& command) -> Bool
{
    if (auto const action = InternalFindAction(command))
    {
        auto enabled = False;
        auto toggled = False;
        if (action->State(enabled, toggled))
        {
            return enabled;
        }
    }

    if (auto parent = GetParent())
    {
        return parent->InternalIsEnabled(command);
    }
    return RootIsEnabled(command);
}

auto CommandNode::InternalIsToggled(CommandId const& command) -> Bool
{
    if (auto const action = InternalFindAction(command))
    {
        auto enabled = False;
        auto toggled = False;
        if (action->State(enabled, toggled))
        {
            return toggled;
        }
    }

    if (auto parent = GetParent())
    {
        return parent->InternalIsToggled(command);
    }
    return RootIsToggled(command);
}

auto CommandNode::InternalGetObserver(CommandId const& command) -> Unique<CommandObserver>
{
    auto observer = Unique<CommandObserver>::Make(PassKey<CommandNode>(), GetSelf(), command);
    _observerMap[command].push_back(Tracked(observer));
    return observer;
}

auto CommandNode::InternalAddAction(CommandId const& command, Shared<Action> const& action) -> void
{
    if (command != 0U)
    {
        _actionMap.insert_or_assign(command, action);
        InternalNotifyStateChanged(command);
    }
}

auto CommandNode::InternalRemoveAction(CommandId const& command) -> void
{
    if (command != 0U)
    {
        _actionMap.erase(command);
        InternalNotifyStateChanged(command);
    }
}

auto CommandNode::InternalIsBlocking() const -> Bool
{
    return _blocking;
}

auto CommandNode::InternalSetBlocking(Bool const blocking) -> void
{
    if (_blocking != blocking)
    {
        _blocking = blocking;
        InternalNotifyStateChanged();
    }
}

auto CommandNode::RootIsEnabled(CommandId const& command) -> Bool
{
    (void)command;
    return false;
}

auto CommandNode::RootIsToggled(CommandId const& command) -> Bool
{
    (void)command;
    return false;
}

auto CommandNode::RootExecute(CommandId const& command) -> Bool
{
    (void)command;
    return false;
}
}

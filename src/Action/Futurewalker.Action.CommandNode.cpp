// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Action.CommandNode.hpp"
#include "Futurewalker.Action.Action.hpp"
#include "Futurewalker.Action.CommandEvent.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
auto CommandNode::Make() -> Shared<CommandNode>
{
    return CommandNode::MakeDerived<CommandNode>();
}

CommandNode::CommandNode(PassKey<CommandNode> key)
  : CommandNode(key, false)
{
}

CommandNode::CommandNode(PassKey<CommandNode>, Bool rootCanExecute)
  : _rootCanExecute(rootCanExecute)
{
}

CommandNode::~CommandNode() noexcept
{
    try
    {
        for (auto const& child : _children)
        {
            child->_parent = nullptr;
        }

        if (CanExecute())
        {
            for (auto const& child : _children)
            {
                child->InternalNotifyStateChanged();
            }
        }
        _children.clear();
    }
    catch (...)
    {
        FW_DEBUG_ASSERT(false);
    }
}

auto CommandNode::AddChild(Shared<CommandNode> const& child) -> void
{
    if (child)
    {
        if (WouldFormCycle(child))
        {
            FW_DEBUG_ASSERT(false);
            return;
        }

        child->RemoveFromParent();

        AdoptChild(child);

        if (CanExecute())
        {
            child->InternalNotifyStateChanged();
        }
    }
}

auto CommandNode::RemoveChild(Shared<CommandNode> const& child) -> void
{
    if (child && child->GetParent() == GetSelf())
    {
        child->RemoveFromParent();
    }
}

auto CommandNode::GetTracker() -> Weak<void>
{
    return _self;
}

auto CommandNode::GetTracker() const -> Weak<void const>
{
    return _self;
}

auto CommandNode::GetEventReceiver() -> EventReceiver&
{
    return _eventReceiver;
}

auto CommandNode::GetEventReceiver() const -> EventReceiver const&
{
    return _eventReceiver;
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

auto CommandNode::RemoveFromParent() -> void
{
    if (auto const parent = GetParent())
    {
        parent->AbandonChild(GetSelf());

        if (parent->CanExecute())
        {
            InternalNotifyStateChanged();
        }
    }
}

auto CommandNode::AdoptChild(Shared<CommandNode> const& child) -> void
{
    child->_parent = GetSelf();
    _children.push_back(child);
}

auto CommandNode::AbandonChild(Shared<CommandNode> const& child) -> void
{
    child->_parent = nullptr;
    _children.remove(child);
}

auto CommandNode::WouldFormCycle(Shared<CommandNode> const& child) const -> Bool
{
    for (auto ancestor = GetSelf(); ancestor; ancestor = ancestor->GetParent())
    {
        if (ancestor == child)
        {
            return true;
        }
    }
    return false;
}

auto CommandNode::CanExecute() const -> Bool
{
    if (!_actionMap.empty())
    {
        return true;
    }

    if (auto const parent = GetParent())
    {
        return parent->CanExecute();
    }
    return _rootCanExecute;
}

auto CommandNode::InternalFindAction(CommandId const& command) -> Shared<Action>
{
    if (auto const it = _actionMap.find(command); it != _actionMap.end())
    {
        return it->second;
    }
    return nullptr;
}

auto CommandNode::PruneExpiredObservers(std::vector<Tracked<CommandObserver>>& observers) -> void
{
    std::erase_if(observers, [](auto const& tracked) { return tracked.IsExpired(); });
}

auto CommandNode::InternalNotifyStateChanged(CommandId const& command) -> void
{
    if (auto it = _observerMap.find(command); it != _observerMap.end())
    {
        PruneExpiredObservers(it->second);

        if (it->second.empty())
        {
            _observerMap.erase(it);
        }
        else
        {
            auto stateEvent = Event<CommandEvent::StateChanged>();
            stateEvent->SetCommandId(command);
            auto event = Event<>(std::move(stateEvent));
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
    for (auto it = _observerMap.begin(); it != _observerMap.end();)
    {
        PruneExpiredObservers(it->second);

        if (it->second.empty())
        {
            it = _observerMap.erase(it);
            continue;
        }

        auto stateEvent = Event<CommandEvent::StateChanged>();
        stateEvent->SetCommandId(it->first);
        auto event = Event<>(std::move(stateEvent));
        GetEventReceiver().SendEventDetached(event);

        ++it;
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
        if (action->State(command, enabled, toggled))
        {
            if (enabled)
            {
                action->Execute(command);
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
        if (action->State(command, enabled, toggled))
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
        if (action->State(command, enabled, toggled))
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
    auto& observers = _observerMap[command];
    PruneExpiredObservers(observers);

    auto observer = Unique<CommandObserver>::Make(PassKey<CommandNode>(), GetSelf(), command);
    observers.push_back(Tracked(observer));
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

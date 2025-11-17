// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Action.CommandNodeType.hpp"
#include "Futurewalker.Action.CommandId.hpp"
#include "Futurewalker.Action.CommandObserver.hpp"
#include "Futurewalker.Action.CommandDispatcherContextType.hpp"
#include "Futurewalker.Action.CommandEvent.hpp"
#include "Futurewalker.Action.ActionType.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.HashMap.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class CommandNode : NonCopyable
{
public:
    static auto Make() -> Shared<CommandNode>;

    template <class Owner>
    static auto Execute(Owner& owner, CommandId const& command) -> Bool;

    template <class Owner>
    static auto IsEnabled(Owner& owner, CommandId const& command) -> Bool;
    template <class Owner>
    static auto IsToggled(Owner& owner, CommandId const& command) -> Bool;

    template <class Owner>
    static auto GetObserver(Owner& owner, CommandId const& command) -> Unique<CommandObserver>;

    template <class Owner>
    static auto AddAction(Owner& owner, CommandId const& command, Shared<Action> const& action) -> void;
    template <class Owner>
    static auto RemoveAction(Owner& owner, CommandId const& command) -> void;

    template <class Owner>
    static auto IsBlocking(Owner& owner) -> Bool;
    template <class Owner>
    static auto SetBlocking(Owner& owner, Bool const blocking) -> void;

    CommandNode(PassKey<CommandNode>);

    auto AddChild(Shared<CommandNode> const& child) -> void;
    auto RemoveChild(Shared<CommandNode> const& child) -> void;

    auto GetTracker() -> Tracker&;
    auto GetTracker() const -> Tracker const&;

    auto GetEventReceiver() -> EventReceiver&;
    auto GetEventReceiver() const -> EventReceiver const&;

    auto GetCommandNode() -> CommandNode&;
    auto GetCommandNode() const -> CommandNode const&;

    auto NotifyStateChanged(PassKey<CommandDispatcherContext>, CommandId const& command) -> void;

protected:
    template <class Derived, class... Args>
    static auto MakeDerived(Args&&... args) -> Shared<Derived>;

private:
    auto InitializeSelf(Shared<CommandNode> const& self) -> void;

    auto GetSelf() -> Shared<CommandNode>;
    auto GetSelf() const -> Shared<CommandNode const>;

    auto GetParent() -> Shared<CommandNode>;
    auto GetParent() const -> Shared<CommandNode const>;

    auto InternalFindAction(CommandId const& command) -> Shared<Action>;
    auto InternalNotifyStateChanged(CommandId const& command) -> void;
    auto InternalNotifyStateChanged() -> void;

    auto InternalExecute(CommandId const& command) -> Bool;
    auto InternalIsEnabled(CommandId const& command) -> Bool;
    auto InternalIsToggled(CommandId const& command) -> Bool;
    auto InternalGetObserver(CommandId const& command) -> Unique<CommandObserver>;

    auto InternalAddAction(CommandId const& command, Shared<Action> const& action) -> void;
    auto InternalRemoveAction(CommandId const& command) -> void;

    auto InternalIsBlocking() const -> Bool;
    auto InternalSetBlocking(Bool const blocking) -> void;

private:
    virtual auto RootIsEnabled(CommandId const& command) -> Bool;
    virtual auto RootIsToggled(CommandId const& command) -> Bool;
    virtual auto RootExecute(CommandId const& command) -> Bool;

private:
    Weak<CommandNode> _self;
    Weak<CommandNode> _parent;
    CommandNodeList _children;
    Unique<EventReceiver> _eventReceiver;
    Bool _blocking = false;
    HashMap<CommandId, Shared<Action>> _actionMap;
    HashMap<CommandId, std::vector<Tracked<CommandObserver>>> _observerMap;
};

template <class Owner>
auto CommandNode::Execute(Owner& owner, CommandId const& command) -> Bool
{
    auto& node = owner.GetCommandNode();
    return node.InternalExecute(command);
}

template <class Owner>
auto CommandNode::IsEnabled(Owner& owner, CommandId const& command) -> Bool
{
    auto& node = owner.GetCommandNode();
    return node.InternalIsEnabled(command);
}

template <class Owner>
auto CommandNode::IsToggled(Owner& owner, CommandId const& command) -> Bool
{
    auto& node = owner.GetCommandNode();
    return node.InternalIsToggled(command);
}

template <class Owner>
auto CommandNode::GetObserver(Owner& owner, CommandId const& command) -> Unique<CommandObserver>
{
    auto& node = owner.GetCommandNode();
    return node.InternalGetObserver(command);
}

template <class Owner>
auto CommandNode::AddAction(Owner& owner, CommandId const& command, Shared<Action> const& action) -> void
{
    auto& node = owner.GetCommandNode();
    return node.InternalAddAction(command, action);
}

template <class Owner>
auto CommandNode::RemoveAction(Owner& owner, CommandId const& command) -> void
{
    auto& node = owner.GetCommandNode();
    return node.InternalRemoveAction(command);
}

template <class Owner>
auto CommandNode::IsBlocking(Owner& owner) -> Bool
{
    auto& node = owner.GetCommandNode();
    return node.InternalIsBlocking();
}

template <class Owner>
auto CommandNode::SetBlocking(Owner& owner, Bool const blocking) -> void
{
    auto& node = owner.GetCommandNode();
    return node.InternalSetBlocking(blocking);
}

///
/// @brief Allocate derived classes.
///
/// @param args Arguments for constructor of derived classes.
///
template <class Derived, class... Args>
auto CommandNode::MakeDerived(Args&&... args) -> Shared<Derived>
{
    auto key = PassKey<CommandNode>();
    auto node = Shared<Derived>::Make(key, std::forward<Args>(args)...);
    static_cast<CommandNode&>(*node).InitializeSelf(node);
    return node;
}
}
}

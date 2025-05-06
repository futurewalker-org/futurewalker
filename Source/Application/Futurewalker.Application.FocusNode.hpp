// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.FocusNodeType.hpp"

#include "Futurewalker.Event.EventReceiverType.hpp"
#include "Futurewalker.Event.EventType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Concepts.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Focus node.
///
class FocusNode : NonCopyable
{
public:
    static auto Make() -> Shared<FocusNode>;

    FocusNode(PassKey<FocusNode>);
    virtual ~FocusNode();

    auto RequestFocus() -> void;
    auto ReleaseFocus() -> void;

    auto IsFocused() const -> Bool;

    auto GetNextFocus() -> Shared<FocusNode>;

    auto AddChild(Shared<FocusNode> const& child, Pointer<FocusNode const> position) -> void;
    auto RemoveChild(Shared<FocusNode> const& child) -> void;

    auto GetChildren() -> FocusNodeList&;
    auto GetChildren() const -> FocusNodeList const&;

    auto GetEventReceiver() -> EventReceiver&;
    auto GetEventReceiver() const -> EventReceiver const&;

    auto SendEvent(Event& event) -> Async<Bool>;
    auto SendEventDetached(Event& event) -> Bool;

protected:
    template <Concepts::DerivedFrom<FocusNode> Derived, class... Args>
    static auto MakeDerived(Args&&... args) -> Shared<Derived>;

private:
    auto GetSelf() -> Shared<FocusNode>;
    auto GetSelf() const -> Shared<FocusNode const>;
    auto SetSelf(Shared<FocusNode> self) -> void;

    auto GetParent() -> Shared<FocusNode>;
    auto GetParent() const -> Shared<FocusNode const>;
    auto SetParent(Weak<FocusNode> parent) -> void;

    auto GetRoot() -> Shared<FocusNode>;
    auto GetRoot() const -> Shared<FocusNode const>;

    auto AdoptChild(Shared<FocusNode> const& child, Pointer<FocusNode const> position) -> void;
    auto AbandonChild(Shared<FocusNode> const& child) -> void;

private:
    virtual auto RootGetFocusedNode() const -> Shared<FocusNode>;
    virtual auto RootRequestFocus(Shared<FocusNode> node) -> void;
    virtual auto RootReleaseFocus(Shared<FocusNode> node) -> void;

private:
    Weak<FocusNode> _self;
    Weak<FocusNode> _parent;
    FocusNodeList _children;
    Shared<EventReceiver> _eventReceiver;
};

///
/// @brief Make derived instance.
///
/// @param[in] args Constructor arguments.
///
template <Concepts::DerivedFrom<FocusNode> Derived, class... Args>
auto FocusNode::MakeDerived(Args&&... args) -> Shared<Derived>
{
    auto self = Shared<Derived>::Make(PassKey<FocusNode>(), std::forward<Args>(args)...);
    self->SetSelf(self);
    return self;
}
}
}

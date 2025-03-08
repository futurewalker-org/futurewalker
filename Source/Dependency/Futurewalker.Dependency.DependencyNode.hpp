// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Dependency.DependencyNodeType.hpp"
#include "Futurewalker.Dependency.DependencyNodeEvent.hpp"

#include "Futurewalker.Event.EventType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.PropertyStore.hpp"
#include "Futurewalker.Core.Concepts.hpp"
#include "Futurewalker.Core.Function.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT 
{
///
/// @brief Dependency node.
///
/// Lightweight object for building dependency trees.
///
class DependencyNode final : NonCopyable
{
public:
    struct Delegate
    {
        Function<Bool(Event&)> dispatchEvent;
    };
    static auto Make(Delegate delegate) -> Shared<DependencyNode>;

public:
    DependencyNode(PassKey<DependencyNode>, Delegate delegate);

    auto Notify() -> void;
    auto NotifyWithEvent(Event const& event) -> void;

    auto AddChild(Shared<DependencyNode> const& node, Pointer<DependencyNode const> after) -> void;
    auto GetChildAt(SInt64 const index) -> Shared<DependencyNode>;
    auto GetChildCount() const -> SInt64;
    auto RemoveChild(Shared<DependencyNode> const& node) -> void;

    auto GetParent() -> Shared<DependencyNode>;
    auto GetParent() const -> Shared<DependencyNode const>;

    auto GetPropertyStore() -> PropertyStore&;
    auto GetPropertyStore() const -> PropertyStore const&;

private:
    auto GetSelf() -> Shared<DependencyNode>;
    auto SetSelf(Shared<DependencyNode> const& self) -> void;
    auto SetParent(Shared<DependencyNode> const& parent) -> void;

    auto RemoveFromParent() -> void;

    auto NotifyCore(Event const& event) -> void;
    auto NotifySelf(Event const& event) -> Bool;
    auto NotifyChildren(Event const& event) -> void;

    auto SendNotifyEvent(Event const& event) -> Bool;

private:
    Delegate _delegate;
    Weak<DependencyNode> _self;
    Unique<PropertyStore> _propertyStore;
    Weak<DependencyNode> _parent;
    DependencyNodeList _children;
};
}
}

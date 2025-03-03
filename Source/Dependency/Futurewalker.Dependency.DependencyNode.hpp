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
    auto NotifyWithEvent(const Event& event) -> void;

    auto AddChild(const Shared<DependencyNode>& node, Pointer<const DependencyNode> after) -> void;
    auto GetChildAt(const SInt64 index) -> Shared<DependencyNode>;
    auto GetChildCount() const -> SInt64;
    auto RemoveChild(const Shared<DependencyNode>& node) -> void;

    auto GetParent() -> Shared<DependencyNode>;
    auto GetParent() const -> Shared<const DependencyNode>;

    auto GetPropertyStore() -> PropertyStore&;
    auto GetPropertyStore() const -> const PropertyStore&;

private:
    auto GetSelf() -> Shared<DependencyNode>;
    auto SetSelf(const Shared<DependencyNode>& self) -> void;
    auto SetParent(const Shared<DependencyNode>& parent) -> void;

    auto RemoveFromParent() -> void;

    auto NotifyCore(const Event& event) -> void;
    auto NotifySelf(const Event& event) -> Bool;
    auto NotifyChildren(const Event& event) -> void;

    auto SendNotifyEvent(const Event& event) -> Bool;

private:
    Delegate _delegate;
    Weak<DependencyNode> _self;
    Unique<PropertyStore> _propertyStore;
    Weak<DependencyNode> _parent;
    DependencyNodeList _children;
};
}
}

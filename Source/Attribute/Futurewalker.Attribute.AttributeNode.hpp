// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Attribute.AttributeNodeType.hpp"
#include "Futurewalker.Attribute.AttributeObserverType.hpp"
#include "Futurewalker.Attribute.AttributeSlot.hpp"
#include "Futurewalker.Attribute.AttributeValue.hpp"
#include "Futurewalker.Attribute.StaticAttribute.hpp"

#include "Futurewalker.Dependency.DependencyNode.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.Optional.hpp"
#include "Futurewalker.Core.StaticPointer.hpp"
#include "Futurewalker.Core.PropertyStore.hpp"
#include "Futurewalker.Core.ReferenceArg.hpp"

#include "Futurewalker.Event.EventReceiverType.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Attribute node.
///
class AttributeNode : NonCopyable
{
public:
    static auto Make() -> Shared<AttributeNode>;

    AttributeNode(PassKey<AttributeNode>);

    auto AddChild(Shared<AttributeNode> const& child) -> void;
    auto RemoveChild(Shared<AttributeNode> const& child) -> void;

    auto GetAttributeNode() -> AttributeNode&;
    auto GetAttributeNode() const -> AttributeNode const&;

    auto GetTracker() -> Tracker&;
    auto GetTracker() const -> Tracker const&;

    auto GetEventReceiver() -> EventReceiver&;
    auto GetEventReceiver() const -> EventReceiver const&;

    template <class T, class Owner>
    static auto GetValue(Owner& owner, StaticAttributeRef<T> description) -> Optional<T>;
    template <class T, class U, class Owner>
    static auto SetValue(Owner& owner, StaticAttributeRef<T> description, const U& value) -> void;
    template <class T, class Owner>
    static auto SetReference(Owner& owner, StaticAttributeRef<T> description, StaticAttributeRef<T> reference) -> void;
    template <class T, class Owner>
    static auto Clear(Owner& owner, StaticAttributeRef<T> description) -> void;
    template <class T, class Owner>
    static auto GetObserver(Owner& owner, StaticAttributeRef<T> description) -> Unique<AttributeObserver<T>>;

private:
    auto GetSelf() -> Shared<AttributeNode>;
    auto GetSelf() const -> Shared<AttributeNode const>;

    auto GetParent() -> Shared<AttributeNode>;
    auto GetParent() const -> Shared<AttributeNode const>;

    auto GetRoot() -> Shared<AttributeNode>;
    auto GetRoot() const -> Shared<AttributeNode const>;

    auto IsAncestorOf(ReferenceArg<AttributeNode const> node) const -> Bool;

    auto AddAttributeSlot(StaticAttributeBaseRef description) -> void;
    auto ResetAttributeSlot(StaticAttributeBaseRef description) -> void;
    auto SetAttributeSlotValue(StaticAttributeBaseRef description, AttributeValue const& value) -> void;
    auto SetAttributeSlotReference(StaticAttributeBaseRef description, StaticAttributeBaseRef const reference) -> void;

    auto InsertAttributeSlot(StaticAttributeBaseRef description) -> Shared<AttributeSlot>;
    auto FindAttributeSlot(AttributeId const& id) -> Shared<AttributeSlot>;
    auto FindAncestorAttributeSlot(AttributeId const& id) -> Shared<AttributeSlot>;

    auto ResolveSource(StaticAttributeBaseRef reference) -> Shared<AttributeSlot>;
    auto ResolveSourceCore(StaticAttributeBaseRef reference, SInt64 const depth) -> Shared<AttributeSlot>;

    auto ResolveValue(StaticAttributeBaseRef reference) -> Shared<AttributeSlot>;
    auto ResolveValueCore(StaticAttributeBaseRef reference, SInt64 const depth) -> Shared<AttributeSlot>;

    auto UpdateSlotCacheRecursive(AttributeSlot& slot) -> void;

    static auto CheckReferenceLoop(StaticAttributeBaseRef reference) -> Bool;

private:
    Weak<AttributeNode> _self;
    Weak<AttributeNode> _parent;
    AttributeNodeList _children;
    AttributeSlotMap _slots;
    Shared<EventReceiver> _eventReceiver;
};

///
/// @brief Get value of attribute.
///
/// @param[in] owner Reference to owner of AttributeNode.
/// @param[in] description Description of attribute.
///
/// @return Value of the attribute.
///
template <class T, class Owner>
auto AttributeNode::GetValue(Owner& owner, StaticAttributeRef<T> description) -> Optional<T>
{
    if (CheckReferenceLoop(description))
    {
        auto& node = owner.GetAttributeNode();

        node.AddAttributeSlot(description);

        auto const id = description.Get().GetId();

        if (auto const slot = node.FindAttributeSlot(id))
        {
            if (auto const cache = slot->GetValueCache())
            {
                return cache->template GetValue<T>();
            }
        }
    }
    return {};
}

///
/// @brief Set value of attribute.
///
/// @param[in] owner Reference to owner of AttributeNode.
/// @param[in] description Description of attribute.
/// @param[in] value Value of attribute.
///
template <class T, class U, class Owner>
auto AttributeNode::SetValue(Owner& owner, StaticAttributeRef<T> description, const U& value) -> void
{
    if (CheckReferenceLoop(description))
    {
        auto& node = owner.GetAttributeNode();
        node.AddAttributeSlot(description);
        node.SetAttributeSlotValue(description, AttributeValue(T(value)));
    }
}

///
/// @brief Set reference of attribute.
///
/// @param[in] owner Reference to owner of AttributeNode.
/// @param[in] description Description of attribute.
/// @param[in] reference Reference to another attribute.
///
template <class T, class Owner>
auto AttributeNode::SetReference(Owner& owner, StaticAttributeRef<T> description, StaticAttributeRef<T> reference) -> void
{
    if (CheckReferenceLoop(description))
    {
        auto& node = owner.GetAttributeNode();
        node.AddAttributeSlot(description);
        node.SetAttributeSlotReference(description, reference);
    }
}

///
/// @brief Clear value/reference of attribute.
///
/// @param[in] owner Reference to owner of attribute node.
/// @param[in] description Description of attribute.
///
template <class T, class Owner>
auto AttributeNode::Clear(Owner& owner, StaticAttributeRef<T> description) -> void
{
    if (CheckReferenceLoop(description))
    {
        auto& node = owner.GetAttributeNode();
        node.ResetAttributeSlot(description);
    }
}

///
/// @brief Get observer for attribute.
///
/// @param[in] owner Reference to owner of attribute node.
/// @param[in] description Description of attribute to observe.
///
/// @return Observer of the attribute.
///
template <class T, class Owner>
auto AttributeNode::GetObserver(Owner& owner, StaticAttributeRef<T> description) -> Unique<AttributeObserver<T>>
{
    if (CheckReferenceLoop(description))
    {
        auto& node = owner.GetAttributeNode();
        node.AddAttributeSlot(description);
        return Unique<AttributeObserver<T>>::Make(PassKey<AttributeNode>(), node._self, description);
    }
    return Unique<AttributeObserver<T>>::Make(PassKey<AttributeNode>(), Weak<AttributeNode>(), description);
}
}
}

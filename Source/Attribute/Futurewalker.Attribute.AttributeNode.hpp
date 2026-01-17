// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Attribute.AttributeNodeType.hpp"
#include "Futurewalker.Attribute.AttributeObserverType.hpp"
#include "Futurewalker.Attribute.AttributeSlot.hpp"
#include "Futurewalker.Attribute.AttributeValue.hpp"
#include "Futurewalker.Attribute.StaticAttribute.hpp"
#include "Futurewalker.Attribute.AttributeFunction.hpp"

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

    template <const auto* Attribute>
    using ValueTypeOf = typename std::remove_pointer_t<decltype(Attribute)>::ValueType;

    template <const auto* Attribute, class Owner>
    static auto GetValue(Owner& owner) -> Optional<ValueTypeOf<Attribute>>;
    template <class T, class Owner>
    static auto GetValue(Owner& owner, StaticAttributeRef<T> attribute) -> Optional<T>;

    template <const auto* Attribute, class U, class Owner>
    static auto SetValue(Owner& owner, const U& value) -> void;
    template <class T, class U, class Owner>
    static auto SetValue(Owner& owner, StaticAttributeRef<T> attribute, const U& value) -> void;

    template <const auto* Attribute, class Owner>
    static auto SetReference(Owner& owner, StaticAttributeRef<ValueTypeOf<Attribute>> reference) -> void;
    template <class T, class Owner>
    static auto SetReference(Owner& owner, StaticAttributeRef<T> attribute, StaticAttributeRef<T> reference) -> void;

    template <const auto* Attribute, class Owner, class F, class... Ts>
    static auto SetFunction(Owner& owner, F&& f, StaticAttributeRef<Ts>... references) -> void;
    template <class T, class Owner, class F, class... Ts>
    static auto SetFunction(Owner& owner, F&& f, StaticAttributeRef<T> attribute, StaticAttributeRef<Ts>... references) -> void;

    template <const auto* Attribute, class Owner>
    static auto Clear(Owner& owner) -> void;
    template <class T, class Owner>
    static auto Clear(Owner& owner, StaticAttributeRef<T> attribute) -> void;

    template <const auto* Attribute, class Owner, class Observer, class Function>
    static auto ConnectAttributeEvent(Owner& owner, Observer&& observer, Function&& function) -> SignalConnection;
    template <class T, class Owner, class Observer, class Function>
    static auto ConnectAttributeEvent(Owner& owner, StaticAttributeRef<T> attribute, Observer&& observer, Function&& function) -> SignalConnection;

    template <const auto* Attribute, class Owner>
    static auto GetObserver(Owner& owner) -> Unique<AttributeObserver<ValueTypeOf<Attribute>>>;
    template <class T, class Owner>
    static auto GetObserver(Owner& owner, StaticAttributeRef<T> attribute) -> Unique<AttributeObserver<T>>;

private:
    auto GetSelf() -> Shared<AttributeNode>;
    auto GetSelf() const -> Shared<AttributeNode const>;

    auto GetParent() -> Shared<AttributeNode>;
    auto GetParent() const -> Shared<AttributeNode const>;

    auto GetRoot() -> Shared<AttributeNode>;
    auto GetRoot() const -> Shared<AttributeNode const>;

    auto IsAncestorOf(ReferenceArg<AttributeNode const> node) const -> Bool;

    auto AddAttributeSlot(StaticAttributeBaseRef description) -> Shared<AttributeSlot>;
    auto ResetAttributeSlot(StaticAttributeBaseRef description) -> void;
    auto SetAttributeSlotValue(StaticAttributeBaseRef description, AttributeValue const& value) -> void;
    auto SetAttributeSlotReference(StaticAttributeBaseRef description, StaticAttributeBaseRef const reference) -> void;
    auto SetAttributeSlotFunction(StaticAttributeBaseRef description, StaticAttributeComputeFunction const& computeFunction, std::span<StaticAttributeBaseRef const> const references) -> void;

    auto InsertAttributeSlot(StaticAttributeBaseRef description) -> Shared<AttributeSlot>;
    auto EraseAttributeSlot(StaticAttributeBaseRef description) -> void;
    auto FindAttributeSlot(AttributeId const& id) -> Shared<AttributeSlot>;
    auto FindAncestorAttributeSlot(AttributeId const& id) -> Shared<AttributeSlot>;

    auto ResolveSource(StaticAttributeBaseRef reference) -> Shared<AttributeSlot>;

    auto ResolveValue(StaticAttributeBaseRef reference) -> Shared<AttributeSlot>;

    auto UpdateSlotCacheRecursive(AttributeSlot& slot, UInt64 const updateNumber) -> Bool;

    auto NotifyValueChangedRecursive(AttributeSlot& slot, UInt64 const updateNumber) -> void;

    static auto CheckReferenceLoop(StaticAttributeBaseRef reference) -> Bool;

private:
    Weak<AttributeNode> _self;
    Weak<AttributeNode> _parent;
    AttributeNodeList _children;
    AttributeSlotMap _slots;
};

///
/// @brief Get value of attribute.
///
/// @tparam Attribute Attribute 
///
/// @param[in] owner Reference to owner of AttributeNode.
///
template <const auto* Attribute, class Owner>
auto AttributeNode::GetValue(Owner& owner) -> Optional<ValueTypeOf<Attribute>>
{
    static_assert(Concepts::SpecializationOf<std::remove_cv_t<std::remove_pointer_t<decltype(Attribute)>>, StaticAttribute>);
    auto constexpr attribute = StaticReference(*Attribute);
    return GetValue(owner, attribute);
}

///
/// @brief Get value of attribute.
///
/// @param[in] owner Reference to owner of AttributeNode.
/// @param[in] attribute Description of attribute.
///
/// @return Value of the attribute.
///
template <class T, class Owner>
auto AttributeNode::GetValue(Owner& owner, StaticAttributeRef<T> attribute) -> Optional<T>
{
    if (CheckReferenceLoop(attribute))
    {
        auto& node = owner.GetAttributeNode();

        node.AddAttributeSlot(attribute);

        auto const id = attribute.Get().GetId();

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
/// @tparam Attribute Attribute
///
/// @param[in] owner Reference to owner of AttributeNode 
/// @param[in] value Value of the attribute  
///
template <const auto* Attribute, class U, class Owner>
auto AttributeNode::SetValue(Owner& owner, U const& value) -> void
{
    static_assert(Concepts::SpecializationOf<std::remove_cv_t<std::remove_pointer_t<decltype(Attribute)>>, StaticAttribute>);
    auto constexpr attribute = StaticReference(*Attribute);
    return SetValue(owner, attribute, value);
}

///
/// @brief Set value of attribute.
///
/// @param[in] owner Reference to owner of AttributeNode.
/// @param[in] attribute Description of attribute.
/// @param[in] value Value of attribute.
///
template <class T, class U, class Owner>
auto AttributeNode::SetValue(Owner& owner, StaticAttributeRef<T> attribute, U const& value) -> void
{
    if (CheckReferenceLoop(attribute))
    {
        auto& node = owner.GetAttributeNode();
        node.AddAttributeSlot(attribute);
        node.SetAttributeSlotValue(attribute, AttributeValue(T(value)));
    }
}

///
/// @brief Set reference of attribute
///
/// @tparam Attribute Attribute
/// @tparam Reference Reference to other attribute
///
/// @param[in] owner Reference to owner of AttributeNode
///
template <const auto* Attribute, class Owner>
auto AttributeNode::SetReference(Owner& owner, StaticAttributeRef<ValueTypeOf<Attribute>> reference) -> void
{
    static_assert(Concepts::SpecializationOf<std::remove_cv_t<std::remove_pointer_t<decltype(Attribute)>>, StaticAttribute>);
    auto constexpr attribute = StaticReference(*Attribute);
    return SetReference(owner, attribute, reference);
}

///
/// @brief Set reference of attribute.
///
/// @param[in] owner Reference to owner of AttributeNode.
/// @param[in] attribute Description of attribute.
/// @param[in] reference Reference to another attribute.
///
template <class T, class Owner>
auto AttributeNode::SetReference(Owner& owner, StaticAttributeRef<T> attribute, StaticAttributeRef<T> reference) -> void
{
    if (CheckReferenceLoop(attribute))
    {
        auto& node = owner.GetAttributeNode();
        node.AddAttributeSlot(attribute);
        node.SetAttributeSlotReference(attribute, reference);
    }
}

///
/// @brief Set compute function of attribute.
///
/// @param owner Reference to owner of AttributeNode
/// @param f Compute function of attribute.
/// @param references Referenced attributes which are passed to compute function.
///
template <const auto* Attribute, class Owner, class F, class... Ts>
auto AttributeNode::SetFunction(Owner& owner, F&& f, StaticAttributeRef<Ts>... references) -> void
{
    static_assert(Concepts::SpecializationOf<std::remove_cv_t<std::remove_pointer_t<decltype(Attribute)>>, StaticAttribute>);
    auto constexpr attribute = StaticReference(*Attribute);
    return SetFunction(owner, std::forward<F>(f), attribute, references...);
}

///
/// @brief Set compute function of attribute.
///
/// @param owner Reference to owner of AttributeNode
/// @param f Compute function of attribute.
/// @param attribute Description of attribute.
/// @param references Referenced attributes which are passed to compute function.
///
template <class T, class Owner, class F, class... Ts>
auto AttributeNode::SetFunction(Owner& owner, F&& f, StaticAttributeRef<T> attribute, StaticAttributeRef<Ts>... references) -> void
{
    if (CheckReferenceLoop(attribute))
    {
        auto const args = std::array {StaticAttributeBaseRef(references)...};
        auto& node = owner.GetAttributeNode();
        node.AddAttributeSlot(attribute);
        node.SetAttributeSlotFunction(attribute, AttributeFunction::MakeComputeFunction<T, Ts...>(std::forward<F>(f)), args);
    }
}

///
/// @brief Clear value/reference of attribute.
///
/// @tparam Attribute Attribute
///
/// @param[in] owner Reference to owner of AttributeNode
///
template <const auto* Attribute, class Owner>
auto AttributeNode::Clear(Owner& owner) -> void
{
    static_assert(Concepts::SpecializationOf<std::remove_cv_t<std::remove_pointer_t<decltype(Attribute)>>, StaticAttribute>);
    auto constexpr attribute = StaticReference(*Attribute);
    return Clear(owner, attribute);
}

///
/// @brief Clear value/reference of attribute.
///
/// @param[in] owner Reference to owner of attribute node.
/// @param[in] attribute Description of attribute.
///
template <class T, class Owner>
auto AttributeNode::Clear(Owner& owner, StaticAttributeRef<T> attribute) -> void
{
    if (CheckReferenceLoop(attribute))
    {
        auto& node = owner.GetAttributeNode();
        node.ResetAttributeSlot(attribute);
    }
}

///
/// @brief Connect event to attribute.
///
/// @tparam Attribute Attribute
///
/// @param[in] owner Reference to owner of AttributeNode
/// @param[in] observer Event observer.
///
template <const auto* Attribute, class Owner, class Observer, class Function>
auto AttributeNode::ConnectAttributeEvent(Owner& owner, Observer&& observer, Function&& function) -> SignalConnection
{
    static_assert(Concepts::SpecializationOf<std::remove_cv_t<std::remove_pointer_t<decltype(Attribute)>>, StaticAttribute>);
    auto constexpr attribute = StaticReference(*Attribute);
    return ConnectAttributeEvent(owner, attribute, std::forward<Observer>(observer), std::forward<Function>(function));
}

///
/// @brief Connect event to attribute.
///
/// @param[in] owner Reference to owner of AttributeNode
/// @param[in] attribute Description of attribute.
/// @param[in] observer Event observer.
/// @param[in] function Event receiver function.
///
template <class T, class Owner, class Observer, class Function>
auto AttributeNode::ConnectAttributeEvent(Owner& owner, StaticAttributeRef<T> attribute, Observer&& observer, Function&& function) -> SignalConnection
{
    if (CheckReferenceLoop(attribute))
    {
        auto& node = owner.GetAttributeNode();
        if (auto slot = node.AddAttributeSlot(attribute))
        {
            return EventReceiver::Connect(*slot, std::forward<Observer>(observer), std::forward<Function>(function));
        }
    }
    return {};
}

///
/// @brief Get observer to attribute. 
///
/// @tparam Attribute Attribute
///
/// @param[in] owner Reference to owner of AttributeNode 
///
/// @return Observer of the attribute.
///
template <auto const* Attribute, class Owner>
auto AttributeNode::GetObserver(Owner& owner) -> Unique<AttributeObserver<ValueTypeOf<Attribute>>>
{
    static_assert(Concepts::SpecializationOf<std::remove_cv_t<std::remove_pointer_t<decltype(Attribute)>>, StaticAttribute>);
    auto constexpr attribute = StaticReference(*Attribute);
    return GetObserver(owner, attribute);
}

///
/// @brief Get observer for attribute.
///
/// @param[in] owner Reference to owner of attribute node.
/// @param[in] attribute Description of attribute to observe.
///
/// @return Observer of the attribute.
///
template <class T, class Owner>
auto AttributeNode::GetObserver(Owner& owner, StaticAttributeRef<T> attribute) -> Unique<AttributeObserver<T>>
{
    if (CheckReferenceLoop(attribute))
    {
        auto& node = owner.GetAttributeNode();
        node.AddAttributeSlot(attribute);
        return Unique<AttributeObserver<T>>::Make(PassKey<AttributeNode>(), node._self, attribute);
    }
    return Unique<AttributeObserver<T>>::Make(PassKey<AttributeNode>(), Weak<AttributeNode>(), attribute);
}
}
}

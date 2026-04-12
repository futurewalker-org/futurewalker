// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Attribute.AttributeSlotType.hpp"
#include "Futurewalker.Attribute.AttributeValue.hpp"
#include "Futurewalker.Attribute.StaticAttributeBaseType.hpp"
#include "Futurewalker.Attribute.AttributeNodeType.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

#include "Futurewalker.Core.Optional.hpp"
#include "Futurewalker.Core.StaticReference.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.SharedArray.hpp"

#include <boost/container/small_vector.hpp>

#include <span>

namespace FW_DETAIL_NS
{
///
/// @brief Attribute slot.
///
class AttributeSlot : NonCopyable
{
public:
    static auto Make(StaticAttributeBaseRef description) -> Shared<AttributeSlot>;

    AttributeSlot(PassKey<AttributeSlot>, StaticAttributeBaseRef description);

    auto GetOwner() -> Shared<AttributeNode>;
    auto SetOwner(Shared<AttributeNode> owner) -> void;

    auto GetEventReceiver() -> EventReceiver&;  

    auto AddUpdateNumber(UInt64 const number) -> void;
    auto RemoveUpdateNumber(UInt64 const number) -> Bool;

    auto HasEventConnection() const -> Bool;

    auto GetValueChanged() const -> Bool;
    auto SetValueChanged(Bool const changed) -> void;

    auto GetDescription() -> StaticAttributeBaseRef;
    auto SetDescription(StaticAttributeBaseRef const& description) -> void;

    auto HasValueCache() const -> Bool;
    auto GetValueCache() const -> Optional<AttributeValue> const&;
    auto SetValueCache(AttributeValue const& value) -> Bool;
    auto ClearValueCache() -> void;

    auto GetReferenceCache() const -> std::vector<StaticAttributeBaseRef> const&;
    auto SetReferenceCache(std::vector<StaticAttributeBaseRef> const& references) -> void;
    auto ClearReferenceCache() -> void;

    auto GetComputeFunctionCache() const -> StaticAttributeComputeFunction const&;
    auto SetComputeFunctionCache(StaticAttributeComputeFunction const& computeFunction) -> void;

    auto GetReferences() -> std::vector<StaticAttributeBaseRef> const&;
    auto GetComputeFunction() const -> StaticAttributeComputeFunction const&;
    auto SetValue(StaticAttributeComputeFunction const& computeFunction, std::span<StaticAttributeBaseRef const> const references) -> void;
    auto ClearValue() -> void;

    auto GetValueDependentSlots() const -> SharedArray<Weak<AttributeSlot>>;
    auto SetValueDependentSlots(std::span<Shared<AttributeSlot> const> const slots) -> void;
    auto DetachFromValueDependentSlots() -> void;

    auto GetSourceDependentSlot() -> Shared<AttributeSlot>;
    auto SetSourceDependentSlot(Shared<AttributeSlot> const& slot) -> void;
    auto DetachFromSourceDependentSlot() -> void;

    auto GetSourceDependantSlotCount() const -> SInt64;
    auto GetSourceDependantSlots() const -> SharedArray<Weak<AttributeSlot>>;
    auto GetValueDependantSlotCount() const -> SInt64;
    auto GetValueDependantSlots() const -> SharedArray<Weak<AttributeSlot>>;

    auto Reset() -> void;

private:
    auto SetSelf(Shared<AttributeSlot> const& self) -> void;
    auto GetSelf() -> Shared<AttributeSlot>;

private:
    Weak<AttributeSlot> _self;
    Weak<AttributeNode> _owner;
    boost::container::small_vector<UInt64, 4> _updateNumbers;
    Bool _valueChanged = false;
    StaticAttributeBaseRef _description;
    Optional<AttributeValue> _valueCache;
    std::vector<StaticAttributeBaseRef> _references;
    std::vector<StaticAttributeBaseRef> _referenceCache;
    StaticAttributeComputeFunction _computeFunction;
    StaticAttributeComputeFunction _computeFunctionCache;
    SharedArray<Weak<AttributeSlot>> _valueDependentSlots;
    SharedArray<Weak<AttributeSlot>> _valueDependantSlots;
    Weak<AttributeSlot> _sourceDependentSlot;
    SharedArray<Weak<AttributeSlot>> _sourceDependantSlots;
    Shared<EventReceiver> _eventReceiver;
};
}

// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Attribute.AttributeSlotType.hpp"
#include "Futurewalker.Attribute.AttributeComputeFunction.hpp"
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

    auto GetCacheUpdateNumber() const -> UInt64;
    auto SetCacheUpdateNumber(UInt64 const number) -> void;

    auto GetRewireUpdateNumber() const -> UInt64;
    auto SetRewireUpdateNumber(UInt64 number) -> void;

    auto HasEventConnection() const -> Bool;

    auto GetValueChanged() const -> Bool;
    auto SetValueChanged(Bool const changed) -> void;

    auto GetDescription() -> StaticAttributeBaseRef;
    auto SetDescription(StaticAttributeBaseRef const& description) -> void;

    auto HasValueCache() const -> Bool;
    auto GetValueCache() const -> Optional<AttributeValue> const&;
    auto SetValueCache(AttributeValue const& value) -> Bool;
    auto ClearValueCache() -> void;

    auto GetReferenceCache() const -> boost::container::small_vector<StaticAttributeBaseRef, 4> const&;
    auto SetReferenceCache(std::span<StaticAttributeBaseRef const> const references) -> Bool;
    auto ClearReferenceCache() -> void;

    auto GetComputeFunctionCache() const -> AttributeComputeFunction const&;
    auto SetComputeFunctionCache(AttributeComputeFunction const& computeFunction) -> Bool;

    auto GetReferences() -> boost::container::small_vector<StaticAttributeBaseRef, 4> const&;
    auto GetComputeFunction() const -> AttributeComputeFunction const&;
    auto SetValue(AttributeComputeFunction const& computeFunction, std::span<StaticAttributeBaseRef const> const references) -> void;
    auto ClearValue() -> void;

    auto GetValueDependentSlots() const -> SharedArray<Weak<AttributeSlot>>;
    auto SetValueDependentSlots(std::span<Shared<AttributeSlot> const> const slots) -> void;
    auto DetachFromValueDependentSlots() -> void;

    auto HasSourceDependentSlot() const noexcept -> Bool;
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
    UInt64 _cacheUpdateNumber = 0U;
    UInt64 _rewireUpdateNumber = 0U;
    Bool _valueChanged = false;
    StaticAttributeBaseRef _description;
    Optional<AttributeValue> _valueCache;
    boost::container::small_vector<StaticAttributeBaseRef, 4> _references;
    boost::container::small_vector<StaticAttributeBaseRef, 4> _referenceCache;
    AttributeComputeFunction _computeFunction;
    AttributeComputeFunction _computeFunctionCache;
    SharedArray<Weak<AttributeSlot>> _valueDependentSlots;
    SharedArray<Weak<AttributeSlot>> _valueDependantSlots;
    Weak<AttributeSlot> _sourceDependentSlot;
    SharedArray<Weak<AttributeSlot>> _sourceDependantSlots;
    EventReceiver _eventReceiver;
};
}

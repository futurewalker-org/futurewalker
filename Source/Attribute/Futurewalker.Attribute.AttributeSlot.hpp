// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Attribute.AttributeSlotType.hpp"
#include "Futurewalker.Attribute.AttributeValue.hpp"
#include "Futurewalker.Attribute.StaticAttributeBaseType.hpp"
#include "Futurewalker.Attribute.AttributeNodeType.hpp"

#include "Futurewalker.Core.Optional.hpp"
#include "Futurewalker.Core.StaticReference.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

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

    auto GetDescription() -> StaticAttributeBaseRef;

    auto GetValueCache() const -> Optional<AttributeValue>;
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

    auto GetValueDependentSlots() -> std::vector<Shared<AttributeSlot>>;
    auto SetValueDependentSlots(std::vector<Shared<AttributeSlot>> const& slots) -> void;
    auto DetachFromValueDependentSlots() -> void;

    auto GetSourceDependentSlot() -> Shared<AttributeSlot>;
    auto SetSourceDependentSlot(Shared<AttributeSlot> const& slot) -> void;
    auto DetachFromSourceDependentSlot() -> void;

    auto GetSourceDependantSlots() -> std::vector<Shared<AttributeSlot>>;
    auto GetValueDependantSlots() -> std::vector<Shared<AttributeSlot>>;

private:
    auto SetSelf(Shared<AttributeSlot> const& self) -> void;
    auto GetSelf() -> Shared<AttributeSlot>;

private:
    Weak<AttributeSlot> _self;
    Weak<AttributeNode> _owner;
    StaticAttributeBaseRef _description;
    Optional<AttributeValue> _valueCache;
    std::vector<StaticAttributeBaseRef> _references;
    std::vector<StaticAttributeBaseRef> _referenceCache;
    StaticAttributeComputeFunction _computeFunction;
    StaticAttributeComputeFunction _computeFunctionCache;
    std::vector<Weak<AttributeSlot>> _valueDependentSlots;
    std::vector<Weak<AttributeSlot>> _valueDependantSlots;
    Weak<AttributeSlot> _sourceDependentSlot;
    std::vector<Weak<AttributeSlot>> _sourceDependantSlots;
};
}

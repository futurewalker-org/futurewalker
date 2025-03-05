// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Attribute.AttributeSlotType.hpp"
#include "Futurewalker.Attribute.AttributeValue.hpp"
#include "Futurewalker.Attribute.StaticAttributeBaseType.hpp"
#include "Futurewalker.Attribute.AttributeNodeType.hpp"

#include "Futurewalker.Dependency.DependencyNode.hpp"

#include "Futurewalker.Core.Optional.hpp"
#include "Futurewalker.Core.StaticReference.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

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
    auto SetValueCache(const AttributeValue& value) -> Bool;
    auto ClearValueCache() -> void;

    auto GetReferenceCache() const -> Optional<StaticAttributeBaseRef>;
    auto SetReferenceCache(StaticAttributeBaseRef reference) -> Bool;
    auto ClearReferenceCache() -> void;

    auto GetValue() const -> Optional<AttributeValue>;
    auto SetValue(const AttributeValue& value) -> void;
    auto ClearValue() -> void;

    auto GetReference() const -> Optional<StaticAttributeBaseRef>;
    auto SetReference(StaticAttributeBaseRef reference) -> void;
    auto ClearReference() -> void;

    auto SetSourceDependentSlot(AttributeSlot& parent) -> void;
    auto SetValueDependentSlot(AttributeSlot& parent) -> void;

    auto DetachFromSourceDependentSlot() -> void;
    auto DetachFromValueDependentSlot() -> void;

    auto GetSourceDependentSlot() -> Shared<AttributeSlot>;
    auto GetValueDependentSlot() -> Shared<AttributeSlot>;

    auto GetSourceDependantSlots() -> std::vector<Shared<AttributeSlot>>;
    auto GetValueDependantSlots() -> std::vector<Shared<AttributeSlot>>;

private:
    auto SetSelf(const Shared<AttributeSlot>& self) -> void;
    auto GetSelf() -> Shared<AttributeSlot>;

    auto Initialize() -> void;

private:
    Weak<AttributeSlot> _self;
    Weak<AttributeNode> _owner;
    StaticAttributeBaseRef _description;
    Optional<AttributeValue> _valueCache;
    Optional<StaticAttributeBaseRef> _referenceCache;
    Optional<AttributeValue> _value;
    Optional<StaticAttributeBaseRef> _reference;
    Shared<DependencyNode> _sourceDependency;
    Shared<DependencyNode> _valueDependency;
};
}

// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Attribute.AttributeSlot.hpp"
#include "Futurewalker.Attribute.StaticAttributeBase.hpp"

#include "Futurewalker.Base.UniqueIdentifier.hpp"

#include "Futurewalker.Core.PropertyStore.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Make new slot.
///
/// @param description Description of an attribute.
///
auto AttributeSlot::Make(StaticAttributeBaseRef description) -> Shared<AttributeSlot>
{
    auto slot = Shared<AttributeSlot>::Make(PassKey<AttributeSlot>(), description);
    slot->SetSelf(slot);
    return slot;
}

///
/// @brief 
///
/// @param description 
///
AttributeSlot::AttributeSlot(PassKey<AttributeSlot>, StaticAttributeBaseRef description)
  : _description {description}
{
}

///
/// @brief
///
auto AttributeSlot::GetOwner() -> Shared<AttributeNode>
{
    return _owner.Lock();
}

///
/// @brief 
///
/// @param owner 
///
auto AttributeSlot::SetOwner(Shared<AttributeNode> owner) -> void
{
    _owner = owner;
}

///
/// @brief
///
auto AttributeSlot::GetDescription() -> StaticAttributeBaseRef
{
    return _description;
}

///
/// @brief
///
auto AttributeSlot::GetValueCache() const -> Optional<AttributeValue>
{
    return _valueCache;
}

///
/// @brief 
///
/// @param value 
///
auto AttributeSlot::SetValueCache(AttributeValue const& value) -> Bool
{
    if (!_valueCache || (*_valueCache != value))
    {
        _valueCache = value;
        return true;
    }
    return false;
}

///
/// @brief
///
auto AttributeSlot::ClearValueCache() -> void
{
    _valueCache.Reset();
}

auto AttributeSlot::GetReferenceCache() const -> std::vector<StaticAttributeBaseRef> const&
{
    return _referenceCache;
}

auto AttributeSlot::SetReferenceCache(std::vector<StaticAttributeBaseRef> const& references) -> void
{
    _referenceCache = references;
}

///
/// @brief 
///
auto AttributeSlot::ClearReferenceCache() -> void
{
    _referenceCache.clear();
}

auto AttributeSlot::GetComputeFunctionCache() const -> StaticAttributeComputeFunction const&
{
    return _computeFunctionCache;
}

auto AttributeSlot::SetComputeFunctionCache(StaticAttributeComputeFunction const& computeFunction) -> void
{
    _computeFunctionCache = computeFunction;
}

auto AttributeSlot::GetReferences() -> std::vector<StaticAttributeBaseRef> const&
{
    return _references;
}

auto AttributeSlot::GetComputeFunction() const -> StaticAttributeComputeFunction const&
{
    return _computeFunction;
}

auto AttributeSlot::SetValue(StaticAttributeComputeFunction const& computeFunction, std::span<StaticAttributeBaseRef const> const references) -> void
{
    _computeFunction = computeFunction;
    _references.assign(references.begin(), references.end());
}

///
/// @brief Clear value.
///
auto AttributeSlot::ClearValue() -> void
{
    _computeFunction = {};
    _references.clear();
}

///
/// @brief
///
auto AttributeSlot::GetValueDependentSlots() -> std::vector<Shared<AttributeSlot>>
{
    auto slots = std::vector<Shared<AttributeSlot>>();
    for (auto const& slot : _valueDependentSlots)
    {
        if (auto const locked = slot.Lock())
        {
            slots.push_back(locked);
        }
    }
    return slots;
}

auto AttributeSlot::SetValueDependentSlots(std::vector<Shared<AttributeSlot>> const& slots) -> void
{
    DetachFromValueDependentSlots();

    for (auto const& slot : slots)
    {
        slot->_valueDependantSlots.push_back(GetSelf());
        _valueDependentSlots.push_back(slot);
    }
}

auto AttributeSlot::DetachFromValueDependentSlots() -> void
{
    for (auto const& weakSlot : _valueDependentSlots)
    {
        if (auto const slot = weakSlot.Lock())
        {
            auto& dependantSlots = slot->_valueDependantSlots;
            auto self = GetSelf();
            dependantSlots.erase(std::remove_if(dependantSlots.begin(), dependantSlots.end(), [=](Weak<AttributeSlot> const& w) { return w.Lock() == self; }), dependantSlots.end());
        }
    }
    _valueDependentSlots.clear();
}

auto AttributeSlot::GetSourceDependentSlot() -> Shared<AttributeSlot>
{
    return _sourceDependentSlot.Lock();
}

auto AttributeSlot::SetSourceDependentSlot(Shared<AttributeSlot> const& slot) -> void
{
    DetachFromSourceDependentSlot();

    if (slot)
    {
        slot->_sourceDependantSlots.push_back(GetSelf());
        _sourceDependentSlot = slot;
    }
}

auto AttributeSlot::DetachFromSourceDependentSlot() -> void
{
    if (auto const slot = _sourceDependentSlot.Lock())
    {
        auto& dependantSlots = slot->_sourceDependantSlots;
        auto self = GetSelf();
        dependantSlots.erase(std::remove_if(dependantSlots.begin(), dependantSlots.end(), [=](Weak<AttributeSlot> const& w) { return w.Lock() == self; }), dependantSlots.end());
    }
    _sourceDependentSlot.Reset();
}

///
/// @brief 
///
auto AttributeSlot::GetValueDependantSlots() -> std::vector<Shared<AttributeSlot>>
{
    auto slots = std::vector<Shared<AttributeSlot>>();
    for (auto const& slot : _valueDependantSlots)
    {
        if (auto const locked = slot.Lock())
        {
            slots.push_back(locked);
        }
    }
    return slots;
}

///
/// @brief 
///
auto AttributeSlot::GetSourceDependantSlots() -> std::vector<Shared<AttributeSlot>>
{
    auto slots = std::vector<Shared<AttributeSlot>>();
    for (auto const& slot : _sourceDependantSlots)
    {
        if (auto const locked = slot.Lock())
        {
            slots.push_back(locked);
        }
    }
    return slots;
}


///
/// @brief 
///
/// @param self 
///
auto AttributeSlot::SetSelf(Shared<AttributeSlot> const& self) -> void
{
    _self = self;
}

///
/// @brief
///
auto AttributeSlot::GetSelf() -> Shared<AttributeSlot>
{
    return _self.Lock();
}
}

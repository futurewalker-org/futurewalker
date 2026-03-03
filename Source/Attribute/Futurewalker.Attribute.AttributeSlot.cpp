// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Attribute.AttributeSlot.hpp"
#include "Futurewalker.Attribute.StaticAttributeBase.hpp"
#include "Futurewalker.Attribute.AttributeNode.hpp"

#include "Futurewalker.Base.UniqueIdentifier.hpp"
#include "Futurewalker.Base.Debug.hpp"

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
auto AttributeSlot::GetEventReceiver() -> EventReceiver&
{
    if (!_eventReceiver)
    {
        _eventReceiver = EventReceiver::Make();
    }
    return *_eventReceiver;
}

///
/// @brief
///
/// @param number
///
auto AttributeSlot::AddUpdateNumber(UInt64 const number) -> void
{
    if (std::find(_updateNumbers.begin(), _updateNumbers.end(), number) == _updateNumbers.end())
    {
        _updateNumbers.push_back(number);
    }
}

///
/// @brief 
///
/// @param number 
///
auto AttributeSlot::RemoveUpdateNumber(UInt64 const number) -> Bool
{
    auto const it = std::find(_updateNumbers.begin(), _updateNumbers.end(), number);
    if (it != _updateNumbers.end())
    {
        _updateNumbers.erase(it);
        return true;
    }
    return false;
}

///
/// @brief
///
auto AttributeSlot::HasEventConnection() const -> Bool
{
    if (_eventReceiver)
    {
        return _eventReceiver->HasConnection();
    }
    return false;
}

///
/// @brief
///
auto AttributeSlot::GetValueChanged() const -> Bool
{
    return _valueChanged;
}

///
/// @brief
///
/// @param changed
///
auto AttributeSlot::SetValueChanged(Bool const changed) -> void
{
    _valueChanged = changed;
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
/// @param description
///
auto AttributeSlot::SetDescription(StaticAttributeBaseRef const& description) -> void
{
    _description = description;
}

///
/// @brief
///
auto AttributeSlot::HasValueCache() const -> Bool
{
    return _valueCache.HasValue();
}

///
/// @brief
///
auto AttributeSlot::GetValueCache() const -> Optional<AttributeValue> const&
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
    FW_DEBUG_ASSERT(computeFunction);
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
auto AttributeSlot::GetValueDependentSlots() -> std::vector<Weak<AttributeSlot>> const&
{
    return _valueDependentSlots;
}

auto AttributeSlot::SetValueDependentSlots(std::vector<Shared<AttributeSlot>> const& slots) -> void
{
    DetachFromValueDependentSlots();

    auto const self = GetSelf();
    for (auto const& slot : slots)
    {
        slot->_valueDependantSlots.push_back(self);
        _valueDependentSlots.push_back(slot);
    }
}

auto AttributeSlot::DetachFromValueDependentSlots() -> void
{
    auto const self = GetSelf();
    for (auto const& weakSlot : _valueDependentSlots)
    {
        if (auto const slot = weakSlot.Lock())
        {
            auto& dependantSlots = slot->_valueDependantSlots;
            auto const it = std::remove_if(dependantSlots.begin(), dependantSlots.end(), [&](Weak<AttributeSlot> const& w) { return w.Lock() == self; });
            if (it != dependantSlots.end())
            {
                dependantSlots.erase(it);
            }
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
        FW_DEBUG_ASSERT(_description.Get().GetId() == slot->GetDescription().Get().GetId());
        slot->_sourceDependantSlots.push_back(GetSelf());
        _sourceDependentSlot = slot;
    }
}

auto AttributeSlot::DetachFromSourceDependentSlot() -> void
{
    auto const self = GetSelf();
    if (auto const slot = _sourceDependentSlot.Lock())
    {
        auto& dependantSlots = slot->_sourceDependantSlots;
        auto const it = std::find_if(dependantSlots.begin(), dependantSlots.end(), [&](Weak<AttributeSlot> const& w) { return w.Lock() == self; });
        if (it != dependantSlots.end())
        {
            dependantSlots.erase(it);
        }
    }
    _sourceDependentSlot.Reset();
}

///
/// @brief
///
auto AttributeSlot::GetValueDependantSlotCount() const -> SInt64
{
    return static_cast<SInt64>(_valueDependantSlots.size());
}

///
/// @brief
///
auto AttributeSlot::GetValueDependantSlots() -> std::vector<Weak<AttributeSlot>>
{
    return _valueDependantSlots;
}

///
/// @brief
///
auto AttributeSlot::GetSourceDependantSlotCount() const -> SInt64
{
    return static_cast<SInt64>(_sourceDependantSlots.size());
}

///
/// @brief
///
auto AttributeSlot::GetSourceDependantSlots() -> std::vector<Weak<AttributeSlot>> 
{
    return _sourceDependantSlots;
}

///
/// @brief
///
auto AttributeSlot::Reset() -> void
{
    FW_DEBUG_ASSERT(_sourceDependentSlot.Lock() == nullptr);
    FW_DEBUG_ASSERT(_sourceDependantSlots.empty());
    FW_DEBUG_ASSERT(_valueDependentSlots.empty());
    FW_DEBUG_ASSERT(_valueDependantSlots.empty());

    ClearValue();
    ClearValueCache();
    ClearReferenceCache();
    _owner = nullptr;
    _updateNumbers.clear();
    _valueChanged = false;
    _sourceDependentSlot.Reset();
    _sourceDependantSlots.clear();
    _valueDependentSlots.clear();
    _valueDependantSlots.clear();

    if (_eventReceiver)
    {
        _eventReceiver->DisconnectAll();
    }
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

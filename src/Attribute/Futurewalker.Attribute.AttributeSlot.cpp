// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Attribute.AttributeSlot.hpp"
#include "Futurewalker.Attribute.StaticAttributeBase.hpp"
#include "Futurewalker.Attribute.AttributeNode.hpp"

#include "Futurewalker.Base.UniqueIdentifier.hpp"
#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Core.PropertyStore.hpp"

#include <memory_resource>

namespace FW_DETAIL_NS
{
///
/// @brief Make new slot.
///
/// @param description Description of an attribute.
///
auto AttributeSlot::Make(StaticAttributeBaseRef description) -> Shared<AttributeSlot>
{
    static auto _pool = std::pmr::synchronized_pool_resource();
    auto allocator = std::pmr::polymorphic_allocator<AttributeSlot>(&_pool);
    auto slot = Shared<AttributeSlot>::MakeWithAllocator(allocator, PassKey<AttributeSlot>(), description);
    slot->SetSelf(slot);
    return slot;
}

AttributeSlot::AttributeSlot(PassKey<AttributeSlot>, StaticAttributeBaseRef description)
  : _description {description}
{
}

auto AttributeSlot::GetOwner() -> Shared<AttributeNode>
{
    return _owner.Lock();
}

auto AttributeSlot::GetEventReceiver() -> EventReceiver&
{
    if (!_eventReceiver)
    {
        _eventReceiver = EventReceiver::Make();
    }
    return *_eventReceiver;
}

auto AttributeSlot::AddUpdateNumber(UInt64 const number) -> void
{
    if (std::find(_updateNumbers.begin(), _updateNumbers.end(), number) == _updateNumbers.end())
    {
        _updateNumbers.push_back(number);
    }
}

auto AttributeSlot::RemoveUpdateNumber(UInt64 const number) -> Bool
{
    auto const it = std::find(_updateNumbers.begin(), _updateNumbers.end(), number);
    if (it != _updateNumbers.end())
    {
        *it = _updateNumbers.back();
        _updateNumbers.pop_back();
        return true;
    }
    return false;
}

auto AttributeSlot::GetCacheUpdateNumber() const -> UInt64
{
    return _cacheUpdateNumber;
}

auto AttributeSlot::SetCacheUpdateNumber(UInt64 const number) -> void
{
    _cacheUpdateNumber = number;
}

auto AttributeSlot::GetRewireUpdateNumber() const -> UInt64
{
    return _rewireUpdateNumber;
}

auto AttributeSlot::SetRewireUpdateNumber(UInt64 number) -> void
{
    _rewireUpdateNumber = number;
}

auto AttributeSlot::HasEventConnection() const -> Bool
{
    if (_eventReceiver)
    {
        return _eventReceiver->HasConnection();
    }
    return false;
}

auto AttributeSlot::GetValueChanged() const -> Bool
{
    return _valueChanged;
}

auto AttributeSlot::SetValueChanged(Bool const changed) -> void
{
    _valueChanged = changed;
}

auto AttributeSlot::SetOwner(Shared<AttributeNode> owner) -> void
{
    _owner = owner;
}

auto AttributeSlot::GetDescription() -> StaticAttributeBaseRef
{
    return _description;
}

auto AttributeSlot::SetDescription(StaticAttributeBaseRef const& description) -> void
{
    _description = description;
}

auto AttributeSlot::HasValueCache() const -> Bool
{
    return _valueCache.HasValue();
}

auto AttributeSlot::GetValueCache() const -> Optional<AttributeValue> const&
{
    return _valueCache;
}

auto AttributeSlot::SetValueCache(AttributeValue const& value) -> Bool
{
    if (!_valueCache || (*_valueCache != value))
    {
        _valueCache = value;
        return true;
    }
    return false;
}

auto AttributeSlot::ClearValueCache() -> void
{
    _valueCache.Reset();
}

auto AttributeSlot::GetReferenceCache() const -> boost::container::small_vector<StaticAttributeBaseRef, 4> const&
{
    return _referenceCache;
}

auto AttributeSlot::SetReferenceCache(std::span<StaticAttributeBaseRef const> const references) -> Bool
{
    if (!std::ranges::equal(_referenceCache, references, [](auto l, auto r) { return l.GetPointer() == r.GetPointer(); }))
    {
        _referenceCache.assign(references.begin(), references.end());
        return true;
    }
    return false;
}

auto AttributeSlot::ClearReferenceCache() -> void
{
    _referenceCache.clear();
}

auto AttributeSlot::GetComputeFunctionCache() const -> AttributeComputeFunction const&
{
    return _computeFunctionCache;
}

auto AttributeSlot::SetComputeFunctionCache(AttributeComputeFunction const& computeFunction) -> Bool
{
    if (_computeFunctionCache != computeFunction)
    {
        _computeFunctionCache = computeFunction;
        return true;
    }
    return false;
}

auto AttributeSlot::GetReferences() -> boost::container::small_vector<StaticAttributeBaseRef, 4> const&
{
    return _references;
}

auto AttributeSlot::GetComputeFunction() const -> AttributeComputeFunction const&
{
    return _computeFunction;
}

auto AttributeSlot::SetValue(AttributeComputeFunction const& computeFunction, std::span<StaticAttributeBaseRef const> const references) -> void
{
    FW_DEBUG_ASSERT(computeFunction);
    _computeFunction = computeFunction;
    _references.assign(references.begin(), references.end());
}

auto AttributeSlot::ClearValue() -> void
{
    _computeFunction = {};
    _references.clear();
}

auto AttributeSlot::GetValueDependentSlots() const -> SharedArray<Weak<AttributeSlot>>
{
    return _valueDependentSlots;
}

auto AttributeSlot::SetValueDependentSlots(std::span<Shared<AttributeSlot> const> const slots) -> void
{
    DetachFromValueDependentSlots();

    _valueDependentSlots.Reserve(std::ssize(slots));

    for (auto const& slot : slots)
    {
        slot->_valueDependantSlots.PushBack(GetWeakSelf());
        _valueDependentSlots.PushBack(slot);
    }
}

auto AttributeSlot::DetachFromValueDependentSlots() -> void
{
    auto const self = GetSelf();
    for (auto i = SInt64(0); i < _valueDependentSlots.GetSize(); ++i)
    {
        if (auto const slot = _valueDependentSlots.GetValueAt(i).Lock())
        {
            auto& dependantSlots = slot->_valueDependantSlots;
            for (auto j = SInt64(0); j < dependantSlots.GetSize(); ++j)
            {
                if (dependantSlots.GetValueAt(j).Lock() == self)
                {
                    dependantSlots.EraseUnordered(j);
                    break;
                }
            }
        }
    }
    _valueDependentSlots.Clear();
}

auto AttributeSlot::HasSourceDependentSlot() const noexcept -> Bool
{
    return !_sourceDependentSlot.IsExpired();
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
        slot->_sourceDependantSlots.PushBack(GetWeakSelf());
        _sourceDependentSlot = slot;
    }
}

auto AttributeSlot::DetachFromSourceDependentSlot() -> void
{
    auto const self = GetSelf();
    if (auto const slot = _sourceDependentSlot.Lock())
    {
        auto& dependantSlots = slot->_sourceDependantSlots;
        for (auto i = SInt64(0); i < dependantSlots.GetSize(); ++i)
        {
            if (dependantSlots.GetValueAt(i).Lock() == self)
            {
                dependantSlots.EraseUnordered(i);
                break;
            }
        }
    }
    _sourceDependentSlot.Reset();
}

auto AttributeSlot::GetValueDependantSlotCount() const -> SInt64
{
    return _valueDependantSlots.GetSize();
}

auto AttributeSlot::GetValueDependantSlots() const -> SharedArray<Weak<AttributeSlot>>
{
    return _valueDependantSlots;
}

auto AttributeSlot::GetSourceDependantSlotCount() const -> SInt64
{
    return _sourceDependantSlots.GetSize();
}

auto AttributeSlot::GetSourceDependantSlots() const -> SharedArray<Weak<AttributeSlot>> 
{
    return _sourceDependantSlots;
}

auto AttributeSlot::UnsafeClearSourceDependantSlots() -> void
{
    _sourceDependantSlots.Clear();
}

auto AttributeSlot::UnsafeSetSourceDependantSlots(SharedArray<Weak<AttributeSlot>> slots) -> void
{
    _sourceDependantSlots = std::move(slots);
}

auto AttributeSlot::UnsafeSetSourceDependentSlot(Shared<AttributeSlot> const& slot) -> void
{
    FW_DEBUG_ASSERT(_description.Get().GetId() == slot->GetDescription().Get().GetId());
    slot->_sourceDependantSlots.PushBack(GetWeakSelf());
    _sourceDependentSlot = slot;
}

auto AttributeSlot::Reset() -> void
{
    FW_DEBUG_ASSERT(_sourceDependentSlot.Lock() == nullptr);
    FW_DEBUG_ASSERT(_sourceDependantSlots.IsEmpty());
    FW_DEBUG_ASSERT(_valueDependentSlots.IsEmpty());
    FW_DEBUG_ASSERT(_valueDependantSlots.IsEmpty());

    ClearValue();
    ClearValueCache();
    ClearReferenceCache();
    _owner = nullptr;
    _updateNumbers.clear();
    _valueChanged = false;
    _sourceDependentSlot.Reset();
    _sourceDependantSlots.Clear();
    _valueDependentSlots.Clear();
    _valueDependantSlots.Clear();

    if (_eventReceiver)
    {
        _eventReceiver->DisconnectAll();
    }
}

auto AttributeSlot::SetSelf(Shared<AttributeSlot> const& self) -> void
{
    _self = self;
}

auto AttributeSlot::GetSelf() -> Shared<AttributeSlot>
{
    return _self.Lock();
}

auto AttributeSlot::GetWeakSelf() -> Weak<AttributeSlot>
{
    return _self;
}
}

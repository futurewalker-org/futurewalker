// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Attribute.AttributeSlot.hpp"
#include "Futurewalker.Attribute.StaticAttributeBase.hpp"

#include "Futurewalker.Dependency.DependencyNode.hpp"

#include "Futurewalker.Base.UniqueIdentifier.hpp"

#include "Futurewalker.Core.PropertyStore.hpp"

namespace FW_DETAIL_NS
{
namespace
{
UniqueId DependencyNodePropertyKeyOwner;

auto SetOwnerSlot(Shared<DependencyNode> const& node, Shared<AttributeSlot> owner) -> void 
{
    if (node)
    {
        PropertyStore::SetValue(*node, DependencyNodePropertyKeyOwner, Weak(owner));
    }
}

auto GetOwnerSlot(Shared<DependencyNode> const& node) -> Shared<AttributeSlot>
{
    if (node)
    {
        if (auto const owner = PropertyStore::GetValue<Weak<AttributeSlot>>(*node, DependencyNodePropertyKeyOwner))
        {
            return owner->Lock();
        }
    }
    return nullptr;
}
}

///
/// @brief Make new slot.
///
/// @param description Description of an attribute.
///
auto AttributeSlot::Make(StaticAttributeBaseRef description) -> Shared<AttributeSlot>
{
    auto slot = Shared<AttributeSlot>::Make(PassKey<AttributeSlot>(), description);
    slot->SetSelf(slot);
    slot->Initialize();
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

///
/// @brief 
///
auto AttributeSlot::GetReferenceCache() const -> Optional<StaticAttributeBaseRef>
{
    return _referenceCache;
}

///
/// @brief 
///
/// @param reference 
///
auto AttributeSlot::SetReferenceCache(StaticAttributeBaseRef reference) -> Bool
{
    if (!_referenceCache || (_referenceCache->GetPointer() != reference.GetPointer()))
    {
        _referenceCache = reference;
        return true;
    }
    return false;
}

///
/// @brief 
///
auto AttributeSlot::ClearReferenceCache() -> void
{
    _referenceCache.Reset();
}

///
/// @brief
///
auto AttributeSlot::GetValue() const -> Optional<AttributeValue>
{
    return _value;
}

///
/// @brief 
///
/// @param value 
///
auto AttributeSlot::SetValue(AttributeValue const& value) -> void
{
    _value = value;
}

///
/// @brief Clear value.
///
auto AttributeSlot::ClearValue() -> void
{
    _value.Reset();
}

///
/// @brief Get attribute reference.
///
auto AttributeSlot::GetReference() const -> Optional<StaticAttributeBaseRef>
{
    return _reference;
}

///
/// @brief 
///
/// @param reference 
///
auto AttributeSlot::SetReference(StaticAttributeBaseRef reference) -> void
{
    _reference = reference;
}

///
/// @brief Clear reference.
///
auto AttributeSlot::ClearReference() -> void
{
    _reference.Reset();
}

///
/// @brief 
///
/// @param parent 
///
auto AttributeSlot::SetSourceDependentSlot(AttributeSlot& parent) -> void
{
    if (parent._sourceDependency)
    {
        parent._sourceDependency->AddChild(_sourceDependency, nullptr);
    }
}

///
/// @brief 
///
/// @param parent 
///
auto AttributeSlot::SetValueDependentSlot(AttributeSlot& parent) -> void
{
    if (parent._valueDependency)
    {
        parent._valueDependency->AddChild(_valueDependency, nullptr);
    }
}

///
/// @brief 
///
auto AttributeSlot::DetachFromSourceDependentSlot() -> void
{
    if (_sourceDependency)
    {
        if (auto const parent = _sourceDependency->GetParent())
        {
            parent->RemoveChild(_sourceDependency);
        }
    }
}

///
/// @brief 
///
auto AttributeSlot::DetachFromValueDependentSlot() -> void
{
    if (_valueDependency)
    {
        if (auto const parent = _valueDependency->GetParent())
        {
            parent->RemoveChild(_valueDependency);
        }
    }
}

///
/// @brief 
///
auto AttributeSlot::GetSourceDependentSlot() -> Shared<AttributeSlot>
{
    if (_sourceDependency)
    {
        if (auto const parent = _sourceDependency->GetParent())
        {
            return GetOwnerSlot(parent);
        }
    }
    return nullptr;
}

///
/// @brief
///
auto AttributeSlot::GetValueDependentSlot() -> Shared<AttributeSlot>
{
    if (_valueDependency)
    {
        if (auto const parent = _valueDependency->GetParent())
        {
            return GetOwnerSlot(parent);
        }
    }
    return nullptr;
}

///
/// @brief 
///
auto AttributeSlot::GetSourceDependantSlots() -> std::vector<Shared<AttributeSlot>>
{
    if (_sourceDependency)
    {
        std::vector<Shared<AttributeSlot>> slots;
        slots.reserve(static_cast<size_t>(_sourceDependency->GetChildCount()));
        for (auto i = SInt64(0); i < _sourceDependency->GetChildCount(); ++i)
        {
            slots.push_back(GetOwnerSlot(_sourceDependency->GetChildAt(i)));
        }
        return slots;
    }
    return {};
}

///
/// @brief 
///
auto AttributeSlot::GetValueDependantSlots() -> std::vector<Shared<AttributeSlot>>
{
    if (_valueDependency)
    {
        std::vector<Shared<AttributeSlot>> slots;
        slots.reserve(static_cast<size_t>(_valueDependency->GetChildCount()));
        for (auto i = SInt64(0); i < _valueDependency->GetChildCount(); ++i)
        {
            slots.push_back(GetOwnerSlot(_valueDependency->GetChildAt(i)));
        }
        return slots;
    }
    return {};
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

///
/// @brief 
///
auto AttributeSlot::Initialize() -> void
{
    _sourceDependency = DependencyNode::Make({});
    _valueDependency = DependencyNode::Make({});
    SetOwnerSlot(_sourceDependency, GetSelf());
    SetOwnerSlot(_valueDependency, GetSelf());
}
}

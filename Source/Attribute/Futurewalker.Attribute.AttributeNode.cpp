// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Attribute.AttributeNode.hpp"
#include "Futurewalker.Attribute.AttributeEvent.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Make attribute node.
///
auto AttributeNode::Make() -> Shared<AttributeNode>
{
    auto node = Shared<AttributeNode>::Make(PassKey<AttributeNode>());
    node->_self = node;
    return node;
}

///
/// @brief Constructor.
///
AttributeNode::AttributeNode(PassKey<AttributeNode>)
{
    _eventReceiver = EventReceiver::Make();
}

///
/// @brief Add child node.
///
/// @param[in] child Attribute node.
///
auto AttributeNode::AddChild(Shared<AttributeNode> const& child) -> void
{
    if (!child)
    {
        return;
    }

    if (child->_parent.Lock() == GetSelf())
    {
        return;
    }

    _children.push_back(child);
    child->_parent = _self;

    for (auto const& [id, slot] : child->_slots)
    {
        if (slot->GetValue() || slot->GetReference())
        {
            continue;
        }

        if (auto const sourceSlot = ResolveSource(slot->GetDescription()))
        {
            slot->SetSourceDependentSlot(*sourceSlot);
            child->UpdateSlotCacheRecursive(*slot);
        }
    }
}

///
/// @brief Remove child node.
///
/// @param[in] child Attribute node to remove.
///
auto AttributeNode::RemoveChild(Shared<AttributeNode> const& child) -> void
{
    if (!child)
    {
        return;
    }

    auto it = std::find(_children.begin(), _children.end(), child);
    if (it != _children.end())
    {
        (*it)->_parent.Reset();
        _children.erase(it);

        auto parent = GetSelf();
        while (parent)
        {
            auto const slots = parent->_slots;
            for (auto const& [id, slot] : slots)
            {
                for (auto const& dependantSlot : slot->GetSourceDependantSlots())
                {
                    if (auto const node = dependantSlot->GetOwner())
                    {
                        if (node != parent && !parent->IsAncestorOf(node))
                        {
                            dependantSlot->DetachFromSourceDependentSlot();

                            if (auto const nodeParent = node->GetParent())
                            {
                                if (auto const sourceSlot = nodeParent->ResolveSource(dependantSlot->GetDescription()))
                                {
                                    dependantSlot->SetSourceDependentSlot(*sourceSlot);
                                }
                            }
                            node->UpdateSlotCacheRecursive(*dependantSlot);
                        }
                    }
                }

                for (auto const& dep : slot->GetValueDependantSlots())
                {
                    auto const node = dep->GetOwner();

                    if (node && IsAncestorOf(node))
                    {
                        node->UpdateSlotCacheRecursive(*dep);
                    }
                }
            }
            parent = parent->GetParent();
        }
    }
}

///
/// @brief Get attribute node.
///
auto AttributeNode::GetAttributeNode() -> AttributeNode&
{
    return *this;
}

///
/// @brief Get attribute node.
///
auto AttributeNode::GetAttributeNode() const -> AttributeNode const&
{
    return *this;
}

///
/// @brief Get tracker.
///
auto AttributeNode::GetTracker() -> Tracker&
{
    return _eventReceiver->GetTracker();
}

///
/// @brief Get tracker.
///
auto AttributeNode::GetTracker() const -> Tracker const&
{
    return _eventReceiver->GetTracker();
}

///
/// @brief Get event receiver.
///
auto AttributeNode::GetEventReceiver() -> EventReceiver&
{
    return *_eventReceiver;
}

///
/// @brief Get event receiver.
///
auto AttributeNode::GetEventReceiver() const -> EventReceiver const&
{
    return *_eventReceiver;
}

///
/// @brief Get self node.
///
auto AttributeNode::GetSelf() -> Shared<AttributeNode>
{
    return _self.Lock();
}

///
/// @brief Get self node.
///
auto AttributeNode::GetSelf() const -> Shared<AttributeNode const>
{
    return _self.Lock();
}

///
/// @brief Get parent node.
///
auto AttributeNode::GetParent() -> Shared<AttributeNode>
{
    return _parent.Lock();
}

///
/// @brief Get parent node.
///
auto AttributeNode::GetParent() const -> Shared<AttributeNode const>
{
    return _parent.Lock();
}

///
/// @brief Get root node.
///
auto AttributeNode::GetRoot() -> Shared<AttributeNode>
{
    if (auto parent = GetParent())
    {
        return parent->GetRoot();
    }
    return GetSelf();
}

///
/// @brief Get root node.
///
auto AttributeNode::GetRoot() const -> Shared<AttributeNode const>
{
    if (auto parent = GetParent())
    {
        return parent->GetRoot();
    }
    return GetSelf();
}

///
/// @brief Check thisd node is ancestor of another node.
///
/// @param[in] node Attribute node.
///
auto AttributeNode::IsAncestorOf(ReferenceArg<AttributeNode const> node) const -> Bool
{
    if (node)
    {
        auto parent = node->GetSelf();
        while (parent)
        {
            if (this == parent.GetPointer())
            {
                return true;
            }
            parent = parent->GetParent();
        }
    }
    return false;
}

///
/// @brief Add attribute slot if not already exist.
///
/// @param description Reference to attribute description.
///
auto AttributeNode::AddAttributeSlot(StaticAttributeBaseRef description) -> void
{
    auto const id = description.Get().GetId();
    if (FindAttributeSlot(id))
    {
        return;
    }

    if (auto const sourceSlot = ResolveSource(description))
    {
        if (auto const newSlot = InsertAttributeSlot(description))
        {
            if (newSlot == sourceSlot)
            {
                FW_DEBUG_ASSERT(GetSelf() == GetRoot());
                return;
            }

            if (auto const referenceCache = sourceSlot->GetReferenceCache())
            {
                newSlot->SetReferenceCache(*referenceCache);

                if (auto const valueSlot = ResolveValue(*referenceCache))
                {
                    newSlot->SetValueCache(*valueSlot->GetValueCache());
                    newSlot->SetValueDependentSlot(*valueSlot);
                }
            }
            else if (auto const valueCache = sourceSlot->GetValueCache())
            {
                newSlot->SetValueCache(*valueCache);
            }

            for (auto const& slot : sourceSlot->GetSourceDependantSlots())
            {
                if (IsAncestorOf(slot->GetOwner()))
                {
                    slot->SetSourceDependentSlot(*newSlot);
                }
            }
            newSlot->SetSourceDependentSlot(*sourceSlot);
        }
    }
}

///
/// @brief Reset attribute slot.
///
/// @param description
///
auto AttributeNode::ResetAttributeSlot(StaticAttributeBaseRef description) -> void
{
    auto const id = description.Get().GetId();
    if (auto const slot = FindAttributeSlot(id))
    {
        slot->ClearValue();
        slot->ClearReference();
        slot->DetachFromSourceDependentSlot();

        if (auto const parent = GetParent())
        {
            if (auto const sourceSlot = parent->ResolveSource(description))
            {
                slot->SetSourceDependentSlot(*sourceSlot);
            }
        }
        UpdateSlotCacheRecursive(*slot);
    }
}

///
/// @brief 
///
/// @param description 
/// @param value 
///
auto AttributeNode::SetAttributeSlotValue(StaticAttributeBaseRef description, AttributeValue const& value) -> void
{
    auto const id = description.Get().GetId();
    if (auto const slot = FindAttributeSlot(id))
    {
        if (auto const sourceSlot = slot->GetSourceDependentSlot())
        {
            for (auto const& dependantSlot : sourceSlot->GetValueDependantSlots())
            {
                if (dependantSlot.GetPointer() != slot.GetPointer())
                {
                    if (auto const owner = dependantSlot->GetOwner())
                    {
                        if (owner == GetSelf() || IsAncestorOf(owner))
                        {
                            dependantSlot->SetSourceDependentSlot(*slot);
                        }
                    }
                }
            }
        }
        slot->SetValue(value);
        slot->ClearReference();
        slot->DetachFromSourceDependentSlot();

        UpdateSlotCacheRecursive(*slot);
    }
}

///
/// @brief 
///
/// @param description 
/// @param reference 
///
auto AttributeNode::SetAttributeSlotReference(StaticAttributeBaseRef description, StaticAttributeBaseRef const reference) -> void
{
    auto const id = description.Get().GetId();
    if (auto const slot = FindAttributeSlot(id))
    {
        slot->SetReference(reference);
        slot->ClearValue();
        slot->DetachFromSourceDependentSlot();

        UpdateSlotCacheRecursive(*slot);
    }
}

///
/// @brief Insert new attribute slot.
///
/// @param[in] description Description of attribute.
///
/// @return Return new or existing attribute slot for description.
///
auto AttributeNode::InsertAttributeSlot(StaticAttributeBaseRef description) -> Shared<AttributeSlot>
{
    auto const [it, succ] = _slots.emplace(description.Get().GetId(), AttributeSlot::Make(description));
    auto const [id, slot] = *it;
    slot->SetOwner(GetSelf());
    return slot;
}

///
/// @brief Find attribute slot from this node.
///
auto AttributeNode::FindAttributeSlot(AttributeId const& id) -> Shared<AttributeSlot>
{
    auto const it = _slots.find(id);
    if (it != _slots.end())
    {
        return it->second;
    }
    return nullptr;
}

///
/// @brief Find attribute slot from ancestor nodes.
///
auto AttributeNode::FindAncestorAttributeSlot(AttributeId const& id) -> Shared<AttributeSlot>
{
    auto parent = GetParent();
    while (parent)
    {
        if (auto const slot = parent->FindAttributeSlot(id))
        {
            return slot;
        }
        parent = parent->GetParent();
    }
    return nullptr;
}

///
/// @brief Resolve source of attribute.
///
/// @param[in] reference Reference to attribute description.
///
auto AttributeNode::ResolveSource(StaticAttributeBaseRef reference) -> Shared<AttributeSlot>
{
    return ResolveSourceCore(reference, 0);
}

///
/// @brief Resolve source of attribute.
///
/// @param[in] reference Reference to attribute description.
/// @param[in] depth Depth of recursion.
///
auto AttributeNode::ResolveSourceCore(StaticAttributeBaseRef reference, SInt64 const depth) -> Shared<AttributeSlot>
{
    // Defensive check for infinite loop.
    if (depth >= 256)
    {
        FW_DEBUG_ASSERT(false);
        throw Exception(ErrorCode::InvalidArgument);
    }

    if (auto const slot = FindAttributeSlot(reference.Get().GetId()))
    {
        return slot;
    }

    if (auto const parent = GetParent())
    {
        return parent->ResolveSourceCore(reference, depth);
    }

    if (auto const newSlot = InsertAttributeSlot(reference))
    {
        UpdateSlotCacheRecursive(*newSlot);
        return newSlot;
    }
    FW_DEBUG_ASSERT(false);
    return nullptr;
}

///
/// @brief Resolve value of attribute.
///
/// @param[in] reference Reference to attribute description.
///
/// @note Creates new slots for referenced attributes at root if necessary.
///
auto AttributeNode::ResolveValue(StaticAttributeBaseRef reference) -> Shared<AttributeSlot>
{
    return ResolveValueCore(reference, 0);
}

///
/// @brief Resolve value of attribute.
///
/// @param[in] reference Reference to attribute description.
/// @param[in] depth Depth of recursion.
///
auto AttributeNode::ResolveValueCore(StaticAttributeBaseRef reference, SInt64 const depth) -> Shared<AttributeSlot>
{
    // Defensive check for infinite loop.
    if (depth >= 256)
    {
        FW_DEBUG_ASSERT(false);
        throw Exception(ErrorCode::InvalidArgument);
    }

    if (auto const slot = FindAttributeSlot(reference.Get().GetId()))
    {
        if (slot->GetValueCache())
        {
            return slot;
        }

        if (auto const ref = slot->GetReferenceCache())
        {
            return ResolveValueCore(*ref, depth + 1);
        }
        FW_DEBUG_ASSERT(false);
    }

    if (auto const parent = GetParent())
    {
        return parent->ResolveValueCore(reference, depth);
    }

    if (auto const newSlot = InsertAttributeSlot(reference))
    {
        if (auto const defaultValue = reference.Get().GetDefaultValue())
        {
            newSlot->SetValueCache(*defaultValue);
            return newSlot;
        }
        else if (auto const defaultReference = reference.Get().GetDefaultReference())
        {
            newSlot->SetReferenceCache(*defaultReference);

            if (auto const ref = ResolveValueCore(*defaultReference, depth + 1))
            {
                newSlot->SetValueCache(*ref->GetValueCache());
                newSlot->SetValueDependentSlot(*ref);
                return ref;
            }
        }
    }
    FW_DEBUG_ASSERT(false);
    return nullptr;
}

///
/// @brief Recursively update value of attributes.
///
/// @param[in] slot A slot to update.
///
/// @note Source dependencies must be updated before calling this function.
///
auto AttributeNode::UpdateSlotCacheRecursive(AttributeSlot& slot) -> void
{
    auto valueChanged = False;

    slot.DetachFromValueDependentSlot();

    if (auto const reference = slot.GetReference())
    {
        if (slot.SetReferenceCache(*reference))
        {
            if (auto const resolved = ResolveValue(*reference))
            {
                slot.SetValueDependentSlot(*resolved);

                if (slot.SetValueCache(*resolved->GetValueCache()))
                {
                    valueChanged = true;
                }
            }
        }
    }
    else if (auto const value = slot.GetValue())
    {
        slot.ClearReferenceCache();

        if (slot.SetValueCache(*value))
        {
            valueChanged = true;
        }
    }
    else
    {
        if (auto const sourceSlot = slot.GetSourceDependentSlot())
        {
            if (auto const referenceCache = sourceSlot->GetReferenceCache())
            {
                slot.SetReferenceCache(*referenceCache);

                if (auto const valueSlot = ResolveValue(*referenceCache))
                {
                    slot.SetValueDependentSlot(*valueSlot);

                    if (slot.SetValueCache(*valueSlot->GetValueCache()))
                    {
                        valueChanged = true;
                    }
                }
            }
            else if (auto const valueCache = sourceSlot->GetValueCache())
            {
                slot.ClearReferenceCache();

                if (slot.SetValueCache(*valueCache))
                {
                    valueChanged = true;
                }
            }
        }
        else
        {
            auto const& description = slot.GetDescription().Get();

            if (auto const defaultValue = description.GetDefaultValue())
            {
                slot.ClearReferenceCache();

                if (slot.SetValueCache(*defaultValue))
                {
                    valueChanged = true;
                }
            }
            else if (auto const defaultReference = description.GetDefaultReference())
            {
                slot.SetReferenceCache(*defaultReference);

                if (auto const ref = ResolveValue(*defaultReference))
                {
                    slot.SetValueDependentSlot(*ref);

                    if (slot.SetValueCache(*ref->GetValueCache()))
                    {
                        valueChanged = true;
                    }
                }
            }
        }
    }

    for (auto const& sourceDependantSlot : slot.GetSourceDependantSlots())
    {
        UpdateSlotCacheRecursive(*sourceDependantSlot);
    }

    if (valueChanged)
    {
        for (auto const& valueDependantSlot : slot.GetValueDependantSlots())
        {
            UpdateSlotCacheRecursive(*valueDependantSlot);
        }

        if (auto const valueCache = slot.GetValueCache())
        {
            if (auto const owner = slot.GetOwner())
            {
                auto parameter = AttributeEvent::ValueChanged();
                parameter.SetId(slot.GetDescription().Get().GetId());
                parameter.SetValue(*valueCache);
                auto event = Event(parameter);
                owner->_eventReceiver->SendEventDetached(event);
            }
        }
    }
}

///
/// @brief Basic check for reference loop.
///
auto AttributeNode::CheckReferenceLoop(StaticAttributeBaseRef reference) -> Bool
{
    auto stack = std::vector<AttributeId>();
    return [](this auto self, auto const& reference, auto& stack) -> Bool {
        if (std::find_if(stack.begin(), stack.end(), [&](auto r) { return r == reference.Get().GetId(); }) == stack.end())
        {
            stack.push_back(reference.Get().GetId());
            if (auto const defaultReference = reference.Get().GetDefaultReference())
            {
                return self(*defaultReference, stack);
            }
            return true;
        }
        FW_DEBUG_ASSERT(false);
        return false;
    }(reference, stack);
}
}

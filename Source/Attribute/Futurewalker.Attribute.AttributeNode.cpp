// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Attribute.AttributeNode.hpp"
#include "Futurewalker.Attribute.AttributeEvent.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include <array>

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
        if (slot->GetComputeFunction())
        {
            continue;
        }

        if (auto const sourceSlot = ResolveSource(slot->GetDescription()))
        {
            slot->SetSourceDependentSlot(sourceSlot);
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
                                    dependantSlot->SetSourceDependentSlot(sourceSlot);
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

            auto const& references = sourceSlot->GetReferenceCache();
            auto const& computeFunction = sourceSlot->GetComputeFunctionCache();
            auto args = std::vector<AttributeValue>();
            auto dependentSlots = std::vector<Shared<AttributeSlot>>();
            args.reserve(references.size());
            dependentSlots.reserve(references.size());
            for (auto const& reference : references)
            {
                if (auto const valueSlot = ResolveValue(reference))
                {
                    if (auto const valueCache = valueSlot->GetValueCache())
                    {
                        args.push_back(*valueCache);
                        dependentSlots.push_back(valueSlot);
                    }
                }
            }
            newSlot->SetValueDependentSlots(dependentSlots);
            newSlot->SetReferenceCache(references);
            newSlot->SetComputeFunctionCache(computeFunction);

            FW_DEBUG_ASSERT(computeFunction);
            FW_DEBUG_ASSERT(args.size() == references.size());
            if (computeFunction && args.size() == references.size())
            {
                newSlot->SetValueCache(computeFunction(args));
            }

            for (auto const& slot : sourceSlot->GetSourceDependantSlots())
            {
                if (IsAncestorOf(slot->GetOwner()))
                {
                    slot->SetSourceDependentSlot(newSlot);
                }
            }
            newSlot->SetSourceDependentSlot(sourceSlot);
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
        slot->DetachFromSourceDependentSlot();

        if (auto const parent = GetParent())
        {
            if (auto const sourceSlot = parent->ResolveSource(description))
            {
                slot->SetSourceDependentSlot(sourceSlot);
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
                            dependantSlot->SetSourceDependentSlot(slot);
                        }
                    }
                }
            }
        }
        slot->SetValue([value](auto const&) -> AttributeValue { return value; }, {});
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
        slot->SetValue([](auto const& args) -> AttributeValue { return args[0]; }, {&reference, 1});
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
    if (auto const slot = FindAttributeSlot(reference.Get().GetId()))
    {
        return slot;
    }

    if (auto const parent = GetParent())
    {
        return parent->ResolveSource(reference);
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
    if (auto const slot = FindAttributeSlot(reference.Get().GetId()))
    {
        if (slot->GetValueCache())
        {
            return slot;
        }
        // All existing slots must have value cache.
        FW_DEBUG_ASSERT(false);
    }
    else
    {
        if (auto const parent = GetParent())
        {
            return parent->ResolveValue(reference);
        }
    }

    if (auto const newSlot = InsertAttributeSlot(reference))
    {
        auto const& references = reference.Get().GetReferences();
        auto const& computeFunction = reference.Get().GetComputeFunction();

        auto args = std::vector<AttributeValue>();
        auto dependentSlots = std::vector<Shared<AttributeSlot>>();
        args.reserve(references.size());
        dependentSlots.reserve(references.size());
        for (auto const& ref : references)
        {
            if (auto const slot = ResolveValue(ref))
            {
                if (auto const value = slot->GetValueCache())
                {
                    args.push_back(*value);
                    dependentSlots.push_back(slot);
                }
            }
        }
        newSlot->SetValueDependentSlots(dependentSlots);
        newSlot->SetReferenceCache(references);
        newSlot->SetComputeFunctionCache(computeFunction);

        FW_DEBUG_ASSERT(computeFunction);
        FW_DEBUG_ASSERT(args.size() == references.size());
        if (computeFunction && args.size() == references.size())
        {
            newSlot->SetValueCache(computeFunction(args));
        }
        return newSlot;
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
    FW_DEBUG_ASSERT(slot.GetOwner() == GetSelf());

    auto valueChanged = False;

    slot.DetachFromValueDependentSlots();

    if (auto const function = slot.GetComputeFunction())
    {
        auto const& references = slot.GetReferences();

        auto args = std::vector<AttributeValue>();
        auto dependentSlots = std::vector<Shared<AttributeSlot>>();
        args.reserve(references.size());
        dependentSlots.reserve(references.size());
        for (auto const& reference : references)
        {
            if (auto const resolved = ResolveValue(reference))
            {
                if (auto const value = resolved->GetValueCache())
                {
                    args.push_back(*value);
                    dependentSlots.push_back(resolved);
                }
            }
        }
        FW_DEBUG_ASSERT(args.size() == references.size());

        slot.SetReferenceCache(references);
        slot.SetComputeFunctionCache(function);
        slot.SetValueDependentSlots(dependentSlots);

        if (args.size() == references.size())
        {
            if (slot.SetValueCache(function(args)))
            {
                valueChanged = true;
            }
        }
    }
    else
    {
        if (auto const sourceSlot = slot.GetSourceDependentSlot())
        {
            auto const& references = sourceSlot->GetReferenceCache();
            auto const& computeFunction = sourceSlot->GetComputeFunctionCache();

            auto args = std::vector<AttributeValue>();
            auto dependentSlots = std::vector<Shared<AttributeSlot>>();
            args.reserve(references.size());
            dependentSlots.reserve(references.size());
            for (auto const& reference : references)
            {
                if (auto const valueSlot = ResolveValue(reference))
                {
                    if (auto const value = valueSlot->GetValueCache())
                    {
                        args.push_back(*value);
                        dependentSlots.push_back(valueSlot);
                    }
                }
            }
            FW_DEBUG_ASSERT(args.size() == references.size());

            slot.SetReferenceCache(references);
            slot.SetComputeFunctionCache(computeFunction);
            slot.SetValueDependentSlots(dependentSlots);

            if (computeFunction && args.size() == references.size())
            {
                if (slot.SetValueCache(computeFunction(args)))
                {
                    valueChanged = true;
                }
            }
        }
        else
        {
            auto const& description = slot.GetDescription().Get();
            auto const& references = description.GetReferences();
            auto const& computeFunction = description.GetComputeFunction();

            auto args = std::vector<AttributeValue>();
            auto dependentSlots = std::vector<Shared<AttributeSlot>>();
            args.reserve(references.size());
            dependentSlots.reserve(references.size());
            for (auto const& reference : references)
            {
                if (auto const valueSlot = ResolveValue(reference))
                {
                    if (auto const value = valueSlot->GetValueCache())
                    {
                        args.push_back(*value);
                        dependentSlots.push_back(valueSlot);
                    }
                }
            }

            slot.SetReferenceCache(references);
            slot.SetComputeFunctionCache(computeFunction);
            slot.SetValueDependentSlots(dependentSlots);

            FW_DEBUG_ASSERT(args.size() == references.size());
            FW_DEBUG_ASSERT(computeFunction);
            if (computeFunction && references.size() == args.size())
            {
                if (slot.SetValueCache(computeFunction(args)))
                {
                    valueChanged = true;
                }
            }
        }
    }

    for (auto const& sourceDependantSlot : slot.GetSourceDependantSlots())
    {
        if (auto const node = sourceDependantSlot->GetOwner())
        {
            node->UpdateSlotCacheRecursive(*sourceDependantSlot);
        }
    }

    if (valueChanged)
    {
        for (auto const& valueDependantSlot : slot.GetValueDependantSlots())
        {
            if (auto const node = valueDependantSlot->GetOwner())
            {
                node->UpdateSlotCacheRecursive(*valueDependantSlot);
            }
        }

        if (auto const valueCache = slot.GetValueCache())
        {
            if (auto const owner = slot.GetOwner())
            {
                auto parameter = Event<AttributeEvent::ValueChanged>();
                parameter->SetId(slot.GetDescription().Get().GetId());
                auto event = Event<>(parameter);
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

            auto const& references = reference.Get().GetReferences();
            for (auto const& ref : references)
            {
                if (!self(ref, stack))
                {
                    FW_DEBUG_ASSERT(false);
                    return false;
                }
            }
            return true;
        }
        FW_DEBUG_ASSERT(false);
        return false;
    }(reference, stack);
}
}

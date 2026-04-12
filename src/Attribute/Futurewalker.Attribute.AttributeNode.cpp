// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Attribute.AttributeNode.hpp"
#include "Futurewalker.Attribute.AttributeEvent.hpp"
#include "Futurewalker.Attribute.AttributeSlotCache.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include <array>
#include <atomic>
#include <boost/container/small_vector.hpp>

namespace FW_DETAIL_NS
{
namespace
{
auto GetNextUpdateNumber() -> UInt64
{
    static std::atomic<uint64_t> s_nextUpdateNumber = 0U;
    return ++s_nextUpdateNumber;
}
}

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
    _slotCache = AttributeSlotCache::GetSharedInstance();
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

    auto const updateNumber = GetNextUpdateNumber();
    auto notifySlots = std::vector<Weak<AttributeSlot>>();

    while (true)
    {
        Bool processed = false;
        for (auto const& pair : child->_slots)
        {
            auto slot = pair.second;
            if (RewireSlotOnAddChild(slot, slot, child, updateNumber, notifySlots))
            {
                processed = true;
                break;
            }
        }

        if (processed)
        {
            continue;
        }
        else
        {
            break;
        }
    }

    for (auto const& weakSlot : notifySlots)
    {
        if (auto const slot = weakSlot.Lock())
        {
            if (auto const owner = slot->GetOwner())
            {
                owner->NotifyValueChangedRecursive(*slot, updateNumber);
            }
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
            auto const updateNumber = GetNextUpdateNumber();
            auto const slots = std::vector(std::from_range, parent->_slots);
            auto notifySlots = std::vector<Shared<AttributeSlot>>();
            for (auto const& [id, slot] : slots)
            {
                {
                    auto const sourceDependantSlots = slot->GetSourceDependantSlots();
                    for (auto i = SInt64(0); i < sourceDependantSlots.GetSize(); ++i)
                    {
                        if (auto const dependantSlot = sourceDependantSlots.GetValueAt(i).Lock())
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

                                    if (node->UpdateSlotCacheRecursive(*dependantSlot, updateNumber) || dependantSlot->GetValueChanged())
                                    {
                                        notifySlots.push_back(dependantSlot);
                                    }
                                }
                            }
                        }
                    }
                }
                {
                    auto const valueDependantSlots = slot->GetValueDependantSlots();
                    for (auto i = SInt64(0); i < valueDependantSlots.GetSize(); ++i)
                    {
                        if (auto const valueDependantSlot = valueDependantSlots.GetValueAt(i).Lock())
                        {
                            if (auto const node = valueDependantSlot->GetOwner())
                            {
                                if (node != parent && !parent->IsAncestorOf(node))
                                {
                                    if (node->UpdateSlotCacheRecursive(*valueDependantSlot, updateNumber) || valueDependantSlot->GetValueChanged())
                                    {
                                        notifySlots.push_back(valueDependantSlot);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            for (auto const& slot : notifySlots)
            {
                if (auto const owner = slot->GetOwner())
                {
                    owner->NotifyValueChangedRecursive(*slot, updateNumber);
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
    auto node = GetSelf();
    while (auto parent = node->GetParent())
    {
        node = parent;
    }
    return node;
}

///
/// @brief Get root node.
///
auto AttributeNode::GetRoot() const -> Shared<AttributeNode const>
{
    auto node = GetSelf();
    while (auto parent = node->GetParent())
    {
        node = parent;
    }
    return node;
}

///
/// @brief Check if this node is ancestor of another node.
///
/// @param[in] node Attribute node.
///
auto AttributeNode::IsAncestorOf(ReferenceArg<AttributeNode const> node) const -> Bool
{
    if (node)
    {
        auto parent = node->GetParent();
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
auto AttributeNode::AddAttributeSlot(StaticAttributeBaseRef description) -> Shared<AttributeSlot>
{
    auto const id = description.Get().GetId();
    if (auto slot = FindAttributeSlot(id))
    {
        return slot;
    }

    if (auto const sourceSlot = ResolveSource(description))
    {
        if (auto const newSlot = InsertAttributeSlot(description))
        {
            if (newSlot == sourceSlot)
            {
                FW_DEBUG_ASSERT(GetSelf() == GetRoot());
                return newSlot;
            }

            auto notifySlots = std::vector<Shared<AttributeSlot>>();
            auto const updateNumber = GetNextUpdateNumber();

            {
                auto const sourceDependantSlots = sourceSlot->GetSourceDependantSlots();
                for (auto i = SInt64(0); i < sourceDependantSlots.GetSize(); ++i)
                {
                    if (auto const slot = sourceDependantSlots.GetValueAt(i).Lock())
                    {
                        if (IsAncestorOf(slot->GetOwner()))
                        {
                            slot->SetSourceDependentSlot(newSlot);
                        }
                    }
                }
                newSlot->SetSourceDependentSlot(sourceSlot);
            }

            if (UpdateSlotCacheRecursive(*newSlot, updateNumber))
            {
                notifySlots.push_back(newSlot);
            }

            {
                auto const valueDependantSlots = sourceSlot->GetValueDependantSlots();
                for (auto i = SInt64(0); i < valueDependantSlots.GetSize(); ++i)
                {
                    if (auto const dependantSlot = valueDependantSlots.GetValueAt(i).Lock())
                    {
                        if (auto const owner = dependantSlot->GetOwner())
                        {
                            if (owner->UpdateSlotCacheRecursive(*dependantSlot, updateNumber))
                            {
                                notifySlots.push_back(dependantSlot);
                            }
                        }
                    }
                }
            }

            for (auto const& slot : notifySlots)
            {
                if (auto const owner = slot->GetOwner())
                {
                    owner->NotifyValueChangedRecursive(*slot, updateNumber);
                }
            }
            return newSlot;
        }
    }
    FW_DEBUG_ASSERT(false);
    return {};
}

///
/// @brief Add attribute slot with attribute value.
///
/// @param description Description of the attribute.
/// @param computeFunction Compute function of the attribute.
/// @param references References of the attribute.
///
auto AttributeNode::AddAttributeSlotWithValue(StaticAttributeBaseRef description, StaticAttributeComputeFunction const& computeFunction, std::span<StaticAttributeBaseRef const> const references)
  -> Shared<AttributeSlot>
{
    auto const id = description.Get().GetId();
    if (auto slot = FindAttributeSlot(id))
    {
        FW_DEBUG_ASSERT(false);
        return slot;
    }

    if (auto const newSlot = InsertAttributeSlot(description))
    {
        newSlot->SetValue(computeFunction, references);

        auto const sourceSlot = FindAncestorAttributeSlot(id);

        if (sourceSlot)
        {
            auto const sourceDependantSlots = sourceSlot->GetSourceDependantSlots();
            for (auto i = SInt64(0); i < sourceDependantSlots.GetSize(); ++i)
            {
                if (auto const slot = sourceDependantSlots.GetValueAt(i).Lock())
                {
                    if (IsAncestorOf(slot->GetOwner()))
                    {
                        slot->SetSourceDependentSlot(newSlot);
                    }
                }
            }
        }

        auto const updateNumber = GetNextUpdateNumber();
        auto notifySlots = std::vector<Shared<AttributeSlot>>();
        if (UpdateSlotCacheRecursive(*newSlot, updateNumber))
        {
            notifySlots.push_back(newSlot);
        }

        if (sourceSlot)
        {
            auto const valueDependantSlots = sourceSlot->GetValueDependantSlots();
            for (auto i = SInt64(0); i < valueDependantSlots.GetSize(); ++i)
            {
                if (auto const dependantSlot = valueDependantSlots.GetValueAt(i).Lock())
                {
                    if (auto const owner = dependantSlot->GetOwner())
                    {
                        if (owner->UpdateSlotCacheRecursive(*dependantSlot, updateNumber))
                        {
                            notifySlots.push_back(dependantSlot);
                        }
                    }
                }
            }
        }

        for (auto const& slot : notifySlots)
        {
            if (auto const owner = slot->GetOwner())
            {
                owner->NotifyValueChangedRecursive(*slot, updateNumber);
            }
        }
        return newSlot;
    }
    FW_DEBUG_ASSERT(false);
    return {};
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

        auto const updateNumber = GetNextUpdateNumber();
        if (UpdateSlotCacheRecursive(*slot, updateNumber))
        {
            NotifyValueChangedRecursive(*slot, updateNumber);
        }
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
        slot->SetValue([value](auto const&) -> AttributeValue { return value; }, {});
        slot->DetachFromSourceDependentSlot();

        auto const updateNumber = GetNextUpdateNumber();
        if (UpdateSlotCacheRecursive(*slot, updateNumber))
        {
            NotifyValueChangedRecursive(*slot, updateNumber);
        }
    }
    else
    {
        AddAttributeSlotWithValue(description, [value](auto const&) -> AttributeValue { return value; }, {});
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

        auto const updateNumber = GetNextUpdateNumber();
        if (UpdateSlotCacheRecursive(*slot, updateNumber))
        {
            NotifyValueChangedRecursive(*slot, updateNumber);
        }
    }
    else
    {
        AddAttributeSlotWithValue(description, [](auto const& args) -> AttributeValue { return args[0]; }, {&reference, 1});
    }
}

///
/// @brief
///
/// @param description
/// @param computeFunction
/// @param references
///
auto AttributeNode::SetAttributeSlotFunction(StaticAttributeBaseRef description, StaticAttributeComputeFunction const& computeFunction, std::span<StaticAttributeBaseRef const> const references)
  -> void
{
    auto const id = description.Get().GetId();
    if (auto const slot = FindAttributeSlot(id))
    {
        FW_DEBUG_ASSERT(computeFunction);
        slot->SetValue(computeFunction, references);
        slot->DetachFromSourceDependentSlot();

        auto const updateNumber = GetNextUpdateNumber();
        if (UpdateSlotCacheRecursive(*slot, updateNumber))
        {
            NotifyValueChangedRecursive(*slot, updateNumber);
        }
    }
    else
    {
        AddAttributeSlotWithValue(description, computeFunction, references);
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
    auto const position = _slots.find(description.Get().GetId());
    if (position != _slots.end())
    {
        return position->second;
    }
    auto const it = _slots.try_emplace(position, description.Get().GetId(), _slotCache->AllocateSlot(description));
    it->second->SetOwner(GetSelf());
    return it->second;
}

///
/// @brief Erase attribute slot.
///
/// @param description Description of attribute.
///
auto AttributeNode::EraseAttributeSlot(StaticAttributeBaseRef description) -> void
{
    if (auto const it = _slots.find(description.Get().GetId()); it != _slots.end())
    {
        auto const slot = it->second;
        slot->SetOwner(nullptr);
        _slots.erase(it);
    }
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
/// @brief Find attribute slot from this node.
///
auto AttributeNode::FindAttributeSlot(AttributeId const& id) const -> Shared<AttributeSlot const>
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
/// @brief Measure minimum distance to referenced slots of an attribute.
///
/// @param[in] reference Target attribute to measure.
/// @param[in,out] minDistance Minimum distance from this node to closest node which have a slot in reference chain.
///
auto AttributeNode::MeasureMinDistanceToReferencedSlots(StaticAttributeBaseRef reference, SInt32& minDistance) const -> void
{
    if (minDistance > 0)
    {
        auto depth = SInt32(0);
        auto const& refs = ResolveReferences(reference, depth);
        minDistance = std::min(minDistance, depth);
        for (auto const& ref : refs)
        {
            MeasureMinDistanceToReferencedSlots(ref, minDistance);
        }
    }
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
        auto const updateNumber = GetNextUpdateNumber();
        if (UpdateSlotCacheRecursive(*newSlot, updateNumber))
        {
            NotifyValueChangedRecursive(*newSlot, updateNumber);
        }
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
/// @note Creates new slots for referenced attributes if necessary.
///
auto AttributeNode::ResolveValue(StaticAttributeBaseRef reference) -> Shared<AttributeSlot>
{
    auto depth = SInt32(std::numeric_limits<int32_t>::max());
    MeasureMinDistanceToReferencedSlots(reference, depth);
    auto node = GetSelf();
    for (auto i = SInt32(0); i < depth; ++i)
    {
        node = node->GetParent();
    }
    return node->ResolveValueCore(reference);
}

///
/// @brief Resolve value of attribute.
///
/// @param[in] reference Reference to attribute description.
///
/// @note Creates new slots for referenced attributes if necessary.
///
auto AttributeNode::ResolveValueCore(StaticAttributeBaseRef reference) -> Shared<AttributeSlot>
{
    if (auto const slot = FindAttributeSlot(reference.Get().GetId()))
    {
        if (slot->HasValueCache())
        {
            return slot;
        }
        // All existing slots must have value cache.
        FW_DEBUG_ASSERT(false);
    }
    else if (!reference.Get().GetReferences().empty())
    {
        // Referenced attributes might be set in this node.
        // TODO: We could avoid adding unnecessary slots by traversing reference chain first.
        return AddAttributeSlot(reference);
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

        auto args = boost::container::small_vector<AttributeValue, 4>();
        auto dependentSlots = boost::container::small_vector<Shared<AttributeSlot>, 4>();
        args.reserve(references.size());
        dependentSlots.reserve(references.size());
        for (auto const& ref : references)
        {
            if (auto const slot = ResolveValue(ref))
            {
                if (auto const& value = slot->GetValueCache())
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
/// @brief Get list of referenced attributes.
///
/// @param reference Target attribute.
/// @param depth Distance to attribute slot.
///
auto AttributeNode::ResolveReferences(StaticAttributeBaseRef reference, SInt32& depth) const -> std::vector<StaticAttributeBaseRef> const&
{
    if (auto const slot = FindAttributeSlot(reference.Get().GetId()))
    {
        return slot->GetReferenceCache();
    }
    else
    {
        if (auto const parent = GetParent())
        {
            return parent->ResolveReferences(reference, ++depth);
        }
    }
    return reference.Get().GetReferences();
}

///
/// @brief Rewire slot when adding child node.
///
/// @param slot Slot to rewire.
/// @param start Starting slot for traversal.
/// @param child Child node being added.
/// @param updateNumber Update number for tracking changes.
/// @param notifySlots List of slots to notify after rewiring.
///
/// @return Return true if rewiring is done.
///
auto AttributeNode::RewireSlotOnAddChild(
  Shared<AttributeSlot>& slot,
  Shared<AttributeSlot> const& start,
  Shared<AttributeNode> const& child,
  UInt64 const updateNumber,
  std::vector<Weak<AttributeSlot>>& notifySlots) -> Bool
{
    if (!slot)
    {
        return false;
    }

    if (slot->GetSourceDependentSlot())
    {
        return false;
    }

    if (slot->GetComputeFunction())
    {
        return false;
    }

    {
        auto const valueDependantSlots = slot->GetValueDependantSlots();
        for (auto i = SInt64(0); i < valueDependantSlots.GetSize(); ++i)
        {
            if (auto valueDependantSlot = valueDependantSlots.GetValueAt(i).Lock())
            {
                if (!valueDependantSlot->GetSourceDependentSlot())
                {
                    if (valueDependantSlot->GetOwner() == child && valueDependantSlot != start)
                    {
                        if (RewireSlotOnAddChild(valueDependantSlot, start, child, updateNumber, notifySlots))
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }

    if (auto const sourceSlot = ResolveSource(slot->GetDescription()))
    {
        if (slot->HasEventConnection() || slot->GetValueDependantSlotCount() != 0)
        {
            slot->SetSourceDependentSlot(sourceSlot);

            if (child->UpdateSlotCacheRecursive(*slot, updateNumber) || slot->GetValueChanged())
            {
                notifySlots.push_back(slot);
            }
        }
        else
        {
            slot->DetachFromSourceDependentSlot();
            slot->DetachFromValueDependentSlots();
            child->EraseAttributeSlot(slot->GetDescription());

            auto const sourceDependantSlots = slot->GetSourceDependantSlots();
            auto const valueDependantSlots = slot->GetValueDependantSlots();

            for (auto i = SInt64(0); i< sourceDependantSlots.GetSize(); ++i)
            {
                if (auto const sourceDependantSlot = sourceDependantSlots.GetValueAt(i).Lock())
                {
                    sourceDependantSlot->SetSourceDependentSlot(sourceSlot);
                }
            }

            for (auto i = SInt64(0); i < sourceDependantSlots.GetSize(); ++i)
            {
                if (auto const sourceDependantSlot = sourceDependantSlots.GetValueAt(i).Lock())
                {
                    if (auto const owner = sourceDependantSlot->GetOwner())
                    {
                        if (owner->UpdateSlotCacheRecursive(*sourceDependantSlot, updateNumber) || sourceDependantSlot->GetValueChanged())
                        {
                            notifySlots.push_back(sourceDependantSlot);
                        }
                    }
                }
            }

            for (auto i = SInt64(0); i < valueDependantSlots.GetSize(); ++i)
            {
                if (auto const valueDependantSlot = valueDependantSlots.GetValueAt(i).Lock())
                {
                    if (auto const owner = valueDependantSlot->GetOwner())
                    {
                        if (owner->UpdateSlotCacheRecursive(*valueDependantSlot, updateNumber) || valueDependantSlot->GetValueChanged())
                        {
                            notifySlots.push_back(valueDependantSlot);
                        }
                    }
                }
            }
            FW_DEBUG_ASSERT(slot->GetSourceDependantSlotCount() == 0);
            FW_DEBUG_ASSERT(slot->GetValueDependantSlotCount() == 0);

            if (slot.GetUseCount() == 1)
            {
                _slotCache->RecycleSlot(std::move(slot));
            }
            else
            {
                FW_DEBUG_ASSERT(false); // TODO: Does this case happen?
            }
        }
        return true;
    }
    return false;
}

///
/// @brief Recursively update value of attributes.
///
/// @param[in] slot A slot to update.
/// @param[in] updateNumber Update number.
///
/// @return Return true if value changed during update.
///
/// @note Source dependencies must be updated before calling this function.
///
auto AttributeNode::UpdateSlotCacheRecursive(AttributeSlot& slot, UInt64 const updateNumber) -> Bool
{
    FW_DEBUG_ASSERT(slot.GetOwner() == GetSelf());

    auto valueChanged = False;

    slot.DetachFromValueDependentSlots();

    if (auto const function = slot.GetComputeFunction())
    {
        auto const& references = slot.GetReferences();

        auto args = boost::container::small_vector<AttributeValue, 4>();
        auto dependentSlots = boost::container::small_vector<Shared<AttributeSlot>, 4>();
        args.reserve(references.size());
        dependentSlots.reserve(references.size());
        for (auto const& reference : references)
        {
            if (auto const resolved = ResolveValue(reference))
            {
                if (auto const& value = resolved->GetValueCache())
                {
                    args.push_back(*value);
                    dependentSlots.push_back(resolved);
                }
            }
        }
        FW_DEBUG_ASSERT(function);
        FW_DEBUG_ASSERT(args.size() == references.size());

        slot.SetReferenceCache(references);
        slot.SetComputeFunctionCache(function);
        slot.SetValueDependentSlots(dependentSlots);

        if (function && args.size() == references.size())
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

            auto args = boost::container::small_vector<AttributeValue, 4>();
            auto dependentSlots = boost::container::small_vector<Shared<AttributeSlot>, 4>();
            args.reserve(references.size());
            dependentSlots.reserve(references.size());
            for (auto const& reference : references)
            {
                if (auto const valueSlot = ResolveValue(reference))
                {
                    if (auto const& value = valueSlot->GetValueCache())
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

            auto args = boost::container::small_vector<AttributeValue, 4>();
            auto dependentSlots = boost::container::small_vector<Shared<AttributeSlot>, 4>();
            args.reserve(references.size());
            dependentSlots.reserve(references.size());
            for (auto const& reference : references)
            {
                if (auto const valueSlot = ResolveValue(reference))
                {
                    if (auto const& value = valueSlot->GetValueCache())
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

    auto result = valueChanged;

    {
        auto const sourceDependantSlots = slot.GetSourceDependantSlots();
        for (auto i = SInt64(0); i < sourceDependantSlots.GetSize(); ++i)
        {
            if (auto const sourceDependantSlot = sourceDependantSlots.GetValueAt(i).Lock())
            {
                if (auto const node = sourceDependantSlot->GetOwner())
                {
                    if (node->UpdateSlotCacheRecursive(*sourceDependantSlot, updateNumber))
                    {
                        result = true;
                    }
                }
            }
        }
    }

    if (valueChanged)
    {
        slot.SetValueChanged(true);

        auto const valueDependantSlots = slot.GetValueDependantSlots();
        for (auto i = SInt64(0); i < valueDependantSlots.GetSize(); ++i)
        {
            if (auto const valueDependantSlot = valueDependantSlots.GetValueAt(i).Lock())
            {
                if (auto const node = valueDependantSlot->GetOwner())
                {
                    node->UpdateSlotCacheRecursive(*valueDependantSlot, updateNumber);
                }
            }
        }
    }

    if (result)
    {
        slot.AddUpdateNumber(updateNumber);
        return true;
    }
    return false;
}

auto AttributeNode::NotifyValueChangedRecursive(AttributeSlot& slot, UInt64 const updateNumber) -> void
{
    if (slot.RemoveUpdateNumber(updateNumber))
    {
        if (slot.GetValueChanged())
        {
            slot.SetValueChanged(false);

            if (auto const owner = slot.GetOwner())
            {
                auto parameter = Event<AttributeEvent::ValueChanged>();
                parameter->SetId(slot.GetDescription().Get().GetId());
                auto event = Event<>(std::move(parameter));
                slot.GetEventReceiver().SendEventDetached(event);
            }
        }

        {
            auto const sourceDependantSlots = slot.GetSourceDependantSlots();
            for (auto i = SInt64(0); i < sourceDependantSlots.GetSize(); ++i)
            {
                if (auto const sourceDependantSlot = sourceDependantSlots.GetValueAt(i).Lock())
                {
                    if (auto const node = sourceDependantSlot->GetOwner())
                    {
                        node->NotifyValueChangedRecursive(*sourceDependantSlot, updateNumber);
                    }
                }
            }
        }
        {
            auto const valueDependantSlots = slot.GetValueDependantSlots();
            for (auto i = SInt64(0); i < valueDependantSlots.GetSize(); ++i)
            {
                if (auto const valueDependantSlot = valueDependantSlots.GetValueAt(i).Lock())
                {
                    if (auto const node = valueDependantSlot->GetOwner())
                    {
                        node->NotifyValueChangedRecursive(*valueDependantSlot, updateNumber);
                    }
                }
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
            for (auto const& ref : reference.Get().GetReferences())
            {
                if (!self(ref, stack))
                {
                    FW_DEBUG_ASSERT(false);
                    return false;
                }
            }
            stack.pop_back();
            return true;
        }
        FW_DEBUG_ASSERT(false);
        return false;
    }(reference, stack);
}
}

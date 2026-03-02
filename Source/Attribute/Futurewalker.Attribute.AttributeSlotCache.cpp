// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Attribute.AttributeSlotCache.hpp"
#include "Futurewalker.Attribute.AttributeSlot.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
auto AttributeSlotCache::AllocateSlot(StaticAttributeBaseRef const& description) -> Shared<AttributeSlot>
{
    if (_cache.empty())
    {
        return AttributeSlot::Make(description);
    }
    auto slot = std::move(_cache.back());
    _cache.pop_back();
    slot->SetDescription(description);
    return slot;
}

auto AttributeSlotCache::RecycleSlot(Shared<AttributeSlot> slot) -> void
{
    if (slot.GetUseCount() == 1)
    {
        slot->Reset();
        _cache.push_back(std::move(slot));
    }
}
}

// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Attribute.AttributeSlotCache.hpp"
#include "Futurewalker.Attribute.AttributeSlot.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
auto AttributeSlotCache::GetSharedInstance() -> Shared<AttributeSlotCache>
{
    static Weak<AttributeSlotCache> _cache;
    static std::mutex _mutex;
    auto lock = std::unique_lock(_mutex);
    if (auto cache = _cache.Lock())
    {
        return cache;
    }
    auto newCache = Shared<AttributeSlotCache>::Make();
    _cache = newCache;
    return newCache;
}

auto AttributeSlotCache::AllocateSlot(StaticAttributeBaseRef const& description) -> Shared<AttributeSlot>
{
    auto lock = std::unique_lock(_mutex);
    if (_cache.empty())
    {
        lock.unlock();
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
        auto lock = std::unique_lock(_mutex);
        _cache.push_back(std::move(slot));
    }
}
}

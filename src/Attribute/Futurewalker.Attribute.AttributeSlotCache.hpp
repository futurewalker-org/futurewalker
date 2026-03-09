// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Attribute.AttributeSlotCacheType.hpp"
#include "Futurewalker.Attribute.AttributeSlotType.hpp"
#include "Futurewalker.Attribute.StaticAttributeBaseType.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

#include <mutex>

namespace FW_DETAIL_NS
{
class AttributeSlotCache : NonCopyable
{
public:
    static auto GetSharedInstance() -> Shared<AttributeSlotCache>;

    auto AllocateSlot(StaticAttributeBaseRef const& description) -> Shared<AttributeSlot>;
    auto RecycleSlot(Shared<AttributeSlot> slot) -> void;

private:
    mutable std::mutex _mutex;
    std::vector<Shared<AttributeSlot>> _cache;
};
}

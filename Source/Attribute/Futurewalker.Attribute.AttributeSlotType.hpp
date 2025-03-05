// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Attribute.AttributeId.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.HashMap.hpp"

namespace FW_DETAIL_NS
{
class AttributeSlot;
using AttributeSlotMap = HashMap<AttributeId, Shared<AttributeSlot>>;
}

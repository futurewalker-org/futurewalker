// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Attribute.Prelude.hpp"

#include "Futurewalker.Core.Memory.hpp"

#include <list>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class AttributeNode;
using AttributeNodeList = std::list<Shared<AttributeNode>>;
}
}

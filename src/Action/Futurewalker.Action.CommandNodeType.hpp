// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Action.Prelude.hpp"

#include "Futurewalker.Core.Memory.hpp"

#include <list> 

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class CommandNode;
using CommandNodeList = std::list<Shared<CommandNode>>;
}
}

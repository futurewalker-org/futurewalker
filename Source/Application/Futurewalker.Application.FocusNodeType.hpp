// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

#include "Futurewalker.Core.Memory.hpp"

#include <list>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class FocusNode;
using FocusNodeList = std::list<Shared<FocusNode>>;
}
}

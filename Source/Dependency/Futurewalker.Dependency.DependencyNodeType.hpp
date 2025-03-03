// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Dependency.Prelude.hpp"

#include "Futurewalker.Core.Memory.hpp"

#include <list>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class DependencyNode;
using DependencyNodeList = std::list<Shared<DependencyNode>>;
}
}

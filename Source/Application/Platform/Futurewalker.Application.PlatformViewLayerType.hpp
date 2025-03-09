// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.Platform.Prelude.hpp"

#include "Futurewalker.Core.Memory.hpp"

#include <list>
#include <vector> 

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformViewLayer;
using PlatformViewLayerList = std::list<Shared<PlatformViewLayer>>;
using PlatformViewLayerArray = std::vector<Shared<PlatformViewLayer>>;
}
}

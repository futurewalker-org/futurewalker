// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.Win.Prelude.hpp"

#include "Futurewalker.Core.Memory.hpp"

#include <list>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformViewLayerWin;
using PlatformViewLayerWinList = std::list<Shared<PlatformViewLayerWin>>;
}
}

// SPDX-License-Identifier: MPL-2.0
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

// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.Prelude.hpp"

#include "Futurewalker.Core.Memory.hpp"

#include <vector>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformViewLayerVisualWin;
using PlatformViewLayerVisualWinArray = std::vector<Shared<PlatformViewLayerVisualWin>>;
}
}

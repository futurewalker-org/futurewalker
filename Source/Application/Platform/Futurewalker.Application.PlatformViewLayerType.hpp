// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Platform.Prelude.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Identifier.hpp"
#include "Futurewalker.Core.EnumClassFlag.hpp"

#include <list>
#include <vector> 

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformViewLayer;
using PlatformViewLayerList = std::list<Shared<PlatformViewLayer>>;
using PlatformViewLayerArray = std::vector<Shared<PlatformViewLayer>>;
using PlatformViewLayerId = Identifier<UInt64, PlatformViewLayer>;

///
/// @brief Flags for PlatformViewLayer.
///
enum class PlatformViewLayerRenderFlags : uint64_t
{
    None = 0,
    Rasterize = 1 << 0, ///< Layer should be rasterized to a bitmap.
};
FW_ENABLE_ENUM_CLASS_BITWISE_OPERATORS(PlatformViewLayerRenderFlags);
}
}

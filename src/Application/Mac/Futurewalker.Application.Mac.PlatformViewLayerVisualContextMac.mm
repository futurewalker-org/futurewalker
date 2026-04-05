// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Mac.PlatformViewLayerVisualContextMac.hpp"
#include "Futurewalker.Application.Mac.PlatformViewLayerVisualMac.hpp"

namespace FW_DETAIL_NS
{
auto PlatformViewLayerVisualContextMac::Make() -> Shared<PlatformViewLayerVisualContextMac>
{
    return Shared<PlatformViewLayerVisualContextMac>::Make();
}

auto PlatformViewLayerVisualContextMac::CreateVisual() -> Shared<PlatformViewLayerVisual>
{
    return PlatformViewLayerVisualMac::Make();
}
}

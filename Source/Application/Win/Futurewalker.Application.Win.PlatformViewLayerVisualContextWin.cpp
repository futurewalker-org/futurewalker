// SPDX-License-Identifier: MPL-2.0
#include "Futurewalker.Application.Win.PlatformViewLayerVisualContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerVisualWin.hpp"

namespace FW_DETAIL_NS
{
PlatformViewLayerVisualContextWin::PlatformViewLayerVisualContextWin(Shared<PlatformDCompositionDeviceWin> const& device)
  : _device {device}
{
}

auto PlatformViewLayerVisualContextWin::CreateVisual() -> Shared<PlatformViewLayerVisual>
{
    return PlatformViewLayerVisualWin::Make(_device);
}
}

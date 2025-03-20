// SPDX-License-Identifier: MIT

#include "Futurewalker.Graphics.Win.PlatformGraphicsDeviceWin.hpp"
#include "Futurewalker.Graphics.Win.PlatformSkiaGraphicsDeviceWin.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
///
/// @brief Create graphics device for Direct3D 12.
///
/// @param d3d12Device D3D12 device
///
auto PlatformGraphicsDeviceWin::MakeForD3D12(Shared<PlatformD3D12DeviceWin> const& d3d12Device) -> Shared<PlatformGraphicsDeviceWin>
{
    return PlatformSkiaGraphicsDeviceWin::Make(d3d12Device);
}
}

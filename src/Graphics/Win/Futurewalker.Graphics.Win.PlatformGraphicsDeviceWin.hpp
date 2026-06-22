// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.PixelGeometry.hpp"
#include "Futurewalker.Graphics.Win.PlatformGraphicsDeviceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformSwapChainSurfaceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformD3D12DeviceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformGraphicsDeviceObjectWin.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.Memory.hpp"

#include <d3d12.h>
#include <dxgi.h>
#include <wrl/client.h>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Graphics device.
///
class PlatformGraphicsDeviceWin : public PlatformGraphicsDeviceObjectWin
{
public:
    static auto MakeForD3D12(Shared<PlatformD3D12DeviceWin> const& d3d12Device) -> Shared<PlatformGraphicsDeviceWin>;

    ///
    /// @brief Make swap chain based surface.
    ///
    /// @param[in] width New width of buffer. Should be >= 1.
    /// @param[in] height New height of buffer. Should be >= 1.
    /// @param[in] pixelGeometry Pixel geometry of target screen.
    /// @param[in] textGamma Text rendering gamma.
    /// @param[in] textContrast Text rendering contrast.
    ///
    /// @return New surface object, or nullptr on failure.
    ///
    virtual auto MakeSwapChainSurface(
      Shared<PlatformDCompositionDeviceWin> const& dcompDevice,
      IntPx const width,
      IntPx const height,
      PixelGeometry const pixelGeometry,
      Float64 const textGamma,
      Float64 const textContrast) -> Shared<PlatformSwapChainSurfaceWin> = 0;
};
}
}

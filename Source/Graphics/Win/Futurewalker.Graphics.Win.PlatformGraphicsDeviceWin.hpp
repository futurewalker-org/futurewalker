// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.Win.PlatformGraphicsDeviceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformSwapChainSurfaceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformD3D12DeviceWinType.hpp"

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
class PlatformGraphicsDeviceWin : NonCopyable
{
public:
    static auto MakeForD3D12(Shared<PlatformD3D12DeviceWin> const& device) -> Shared<PlatformGraphicsDeviceWin>;

    ///
    /// @brief Make swap chain based surface.
    ///
    /// @param width Width of surface. Should be >= 1.
    /// @param height Height of surface. Should be >= 1.
    ///
    /// @return New surface object, or nullptr on failure.
    ///
    virtual auto MakeSwapChainSurface(IntPx const width, IntPx const height) -> Shared<PlatformSwapChainSurfaceWin> = 0;

    ///
    /// @brief Notify operation was failed due to device loss.
    ///
    virtual auto NotifyDeviceLost() -> void = 0;

    ///
    /// @brief Handle device loss.
    ///
    virtual auto HandleDeviceLost() -> void = 0;
};
}
}

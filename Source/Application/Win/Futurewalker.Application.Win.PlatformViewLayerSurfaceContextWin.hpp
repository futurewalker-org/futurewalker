// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformViewLayerSurfaceContextWinType.hpp"

#include "Futurewalker.Graphics.Win.PlatformGraphicsDeviceWin.hpp"
#include "Futurewalker.Graphics.Win.PlatformD3D11DeviceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformD3D12DeviceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWinType.hpp"

#include "Futurewalker.Base.Win.PlatformInstanceHandleWin.hpp"

#include "Futurewalker.Core.PassKey.hpp"

#include <dxgi1_4.h>
#include <wrl/client.h>

#include <include/gpu/ganesh/GrDirectContext.h>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Context object of platform view layer surfaces.
///
class PlatformViewLayerSurfaceContextWin : NonCopyable
{
public:
    static auto Make(Shared<PlatformDCompositionDeviceWin> dcompDevice, Shared<PlatformD3D12DeviceWin> d3d12Device) -> Shared<PlatformViewLayerSurfaceContextWin>;

    PlatformViewLayerSurfaceContextWin(PassKey<PlatformViewLayerSurfaceContextWin>, Shared<PlatformDCompositionDeviceWin> dcompDevice, Shared<PlatformD3D12DeviceWin> d3d12Device);

    auto MakeSwapChainSurface(IntPx const width, IntPx const height) -> Shared<Graphics::PlatformSwapChainSurfaceWin>;

private:
    auto GetSelf() -> Shared<PlatformViewLayerSurfaceContextWin>;

private:
    Weak<PlatformViewLayerSurfaceContextWin> _self;
    Shared<PlatformDCompositionDeviceWin> _dcompDevice;
    Shared<PlatformD3D12DeviceWin> _d3d12Device;
    Shared<Graphics::PlatformGraphicsDeviceWin> _graphicsDevice;
};

template <>
struct Locator::Resolver<PlatformViewLayerSurfaceContextWin>
{
    using Interface = PlatformViewLayerSurfaceContextWin;
    static auto Resolve() -> Shared<PlatformViewLayerSurfaceContextWin>;
};
}
}

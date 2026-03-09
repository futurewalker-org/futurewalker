// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformViewLayerSurfaceContextWin.hpp"

#include "Futurewalker.Graphics.Win.PlatformD3D11DeviceWin.hpp"
#include "Futurewalker.Graphics.Win.PlatformD3D12DeviceWin.hpp"
#include "Futurewalker.Graphics.Win.PlatformGraphicsDeviceWin.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Core.StringFunction.hpp"
#include "Futurewalker.Core.Win.PlatformStringFunctionWin.hpp"

#include <include/gpu/ganesh/d3d/GrD3DBackendContext.h>

namespace FW_DETAIL_NS
{
///
/// @brief
///
/// @param instanceHandle
/// @param dcompDevice
/// @param d3d12Device
///
/// @return
///
auto PlatformViewLayerSurfaceContextWin::Make(Shared<PlatformDCompositionDeviceWin> dcompDevice, Shared<PlatformD3D12DeviceWin> d3d12Device) -> Shared<PlatformViewLayerSurfaceContextWin>
{
    auto key = PassKey<PlatformViewLayerSurfaceContextWin>();
    auto context = Shared<PlatformViewLayerSurfaceContextWin>::Make(key, dcompDevice, d3d12Device);
    context->_self = context;
    return context;
}

///
/// @brief
///
/// @param instanceHandle
/// @param dcompDevice
/// @param d3d12Device
///
PlatformViewLayerSurfaceContextWin::PlatformViewLayerSurfaceContextWin(
  PassKey<PlatformViewLayerSurfaceContextWin>,
  Shared<PlatformDCompositionDeviceWin> dcompDevice,
  Shared<PlatformD3D12DeviceWin> d3d12Device)
  : _dcompDevice {dcompDevice}
  , _d3d12Device {d3d12Device}
{
    _graphicsDevice = Graphics::PlatformGraphicsDeviceWin::MakeForD3D12(_d3d12Device);
}

///
/// @brief 
///
auto PlatformViewLayerSurfaceContextWin::MakeSwapChainSurface(IntPx const width, IntPx const height) -> Shared<Graphics::PlatformSwapChainSurfaceWin>
{
    if (_graphicsDevice)
    {
        return _graphicsDevice->MakeSwapChainSurface(_dcompDevice, width, height);
    }
    return {};
}

///
/// @brief
///
auto PlatformViewLayerSurfaceContextWin::GetSelf() -> Shared<PlatformViewLayerSurfaceContextWin>
{
    return _self.Lock();
}

///
/// @brief
///
auto Locator::Resolver<PlatformViewLayerSurfaceContextWin>::Resolve() -> Shared<PlatformViewLayerSurfaceContextWin>
{
    const auto dcompDevice = Locator::Resolve<PlatformDCompositionDeviceWin>();
    const auto d3d12Device = Locator::Resolve<PlatformD3D12DeviceWin>();
    return PlatformViewLayerSurfaceContextWin::Make(dcompDevice, d3d12Device);
}
}

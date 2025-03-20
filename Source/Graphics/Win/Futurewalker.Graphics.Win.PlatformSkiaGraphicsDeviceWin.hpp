// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Graphics.Win.PlatformSkiaGraphicsDeviceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformGraphicsDeviceWin.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.Function.hpp"

#include <include/core/SkSurface.h>
#include <include/gpu/GrDirectContext.h>

#include <dxgi1_4.h>
#include <d3d12.h>
#include <wrl/client.h>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Graphics device.
///
class PlatformSkiaGraphicsDeviceWin : public PlatformGraphicsDeviceWin
{
public:
    static auto Make(Shared<PlatformD3D12DeviceWin> const& d3d12Device) -> Shared<PlatformSkiaGraphicsDeviceWin>;

public:
    PlatformSkiaGraphicsDeviceWin(PassKey<PlatformSkiaGraphicsDeviceWin>, Shared<PlatformD3D12DeviceWin> const& d3d12Device);

    auto MakeSwapChainSurface(IntPx const width, IntPx const height) -> Shared<PlatformSwapChainSurfaceWin> override;

    auto NotifyDeviceLost() -> void override;
    auto HandleDeviceLost() -> void override;

    auto CreateSwapChain(IntPx const width, IntPx const height) -> Microsoft::WRL::ComPtr<IDXGISwapChain3>;
    auto CreateFence() -> Microsoft::WRL::ComPtr<ID3D12Fence>;

    auto CreateBackendTextureSurface(GrBackendTexture const& backendTexture) -> sk_sp<SkSurface>;

    auto FlushAndSubmitSurface(SkSurface* surface, SkSurfaces::BackendSurfaceAccess access, GrSyncCpu sync) -> Bool;
    auto FlushAndSubmit(GrSyncCpu sync) -> Bool;

    auto SignalFence(ID3D12Fence* fence, UINT64 nextValue) -> Bool;

private:
    auto BuildCommandQueue() -> Bool;
    auto BuildGrContext() -> Bool;

private:
    Shared<PlatformD3D12DeviceWin> _d3d12Device;
    Weak<PlatformSkiaGraphicsDeviceWin> _self;
    Microsoft::WRL::ComPtr<IDXGIAdapter1> _adapter;
    Microsoft::WRL::ComPtr<ID3D12Device> _device;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> _commandQueue;
    sk_sp<GrDirectContext> _grContext;
    std::vector<Weak<PlatformSwapChainSurfaceWin>> _surfaces;
};
}
}

// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.Win.PlatformSkiaGraphicsDeviceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformGraphicsDeviceWin.hpp"
#include "Futurewalker.Graphics.Win.PlatformD3D12DeviceWin.hpp"
#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWinType.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.Function.hpp"

#include <include/core/SkSurface.h>
#include <include/gpu/ganesh/GrDirectContext.h>

#include <dxgi1_4.h>
#include <d3d12.h>
#include <presentation.h>
#include <wrl/client.h>
#include <wrl/wrappers/corewrappers.h>

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

    auto MakeSwapChainSurface(Shared<PlatformDCompositionDeviceWin> const& dcompDevice, IntPx const width, IntPx const height) -> Shared<PlatformSwapChainSurfaceWin> override;

    auto NotifyDeviceLost() -> void override;
    auto HandleDeviceLost() -> void override;
    auto AddDeviceObject(Shared<PlatformGraphicsDeviceObjectWin> const& deviceObject) -> void;

    auto CreateTexture(IntPx const width, IntPx const height) -> Microsoft::WRL::ComPtr<ID3D12Resource>;
    auto CreateSwapChain(IntPx const width, IntPx const height) -> Microsoft::WRL::ComPtr<IDXGISwapChain3>;
    auto CreateFence() -> Microsoft::WRL::ComPtr<ID3D12Fence>;
    auto CreateSharedHandle(Microsoft::WRL::ComPtr<ID3D12Resource> const& resource) -> Microsoft::WRL::Wrappers::HandleT<Microsoft::WRL::Wrappers::HandleTraits::HANDLENullTraits>;

    auto CreateBackendTextureSurface(GrBackendTexture const& backendTexture) -> sk_sp<SkSurface>;

    auto FlushAndSubmitSurface(SkSurface* surface, SkSurfaces::BackendSurfaceAccess access, GrSyncCpu sync) -> Bool;
    auto FlushAndSubmit(GrSyncCpu sync) -> Bool;

    auto SignalFence(ID3D12Fence* fence, UINT64 nextValue) -> Bool;

private:
    auto GetSelf() -> Shared<PlatformSkiaGraphicsDeviceWin>;
    auto BuildCommandQueue() -> Bool;
    auto BuildGrContext() -> Bool;
    auto ClearResources() -> void;
    auto BuildResources() -> void;

private:
    Shared<PlatformD3D12DeviceWin> _d3d12Device;
    Weak<PlatformSkiaGraphicsDeviceWin> _self;
    Microsoft::WRL::ComPtr<IDXGIAdapter1> _adapter;
    Microsoft::WRL::ComPtr<ID3D12Device> _device;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> _commandQueue;
    sk_sp<GrDirectContext> _grContext;
    std::vector<Weak<PlatformGraphicsDeviceObjectWin>> _deviceObjects;
};
}
}

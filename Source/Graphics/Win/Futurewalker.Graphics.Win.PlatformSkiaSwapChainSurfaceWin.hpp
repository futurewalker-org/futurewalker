// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Graphics.Win.PlatformSkiaSwapChainSurfaceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformSkiaGraphicsDeviceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformSwapChainSurfaceWin.hpp"

#include <include/core/SkSurface.h>

#include <dxgi1_4.h>
#include <wrl/client.h>
#include <d3d12.h>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Swap chain surface. 
///
class PlatformSkiaSwapChainSurfaceWin : public PlatformSwapChainSurfaceWin
{
public:
    PlatformSkiaSwapChainSurfaceWin(Shared<PlatformSkiaGraphicsDeviceWin> const& device, IntPx const width, IntPx const height);

    auto Resize(IntPx const width, IntPx const height) -> Bool override;
    auto Draw(Function<void(Scene& scene)> func) -> Bool override;
    auto NotifyDeviceLost() -> void override;
    auto HandleDeviceLost() -> void override;
    auto GetSwapChain() -> Microsoft::WRL::ComPtr<IDXGISwapChain1> override;
    auto SetDelegate(Delegate delegate) -> void override;

private:
    auto Rebuild() -> Bool;
    auto HasSwapChain() const -> Bool;
    auto BuildSwapChain() -> Bool;
    auto ResizeBuffers() -> Bool;
    auto RebuildSurfaces() -> Bool;
    auto WaitIdle() -> void;
    auto ReleaseResources() -> void;

private:
    Delegate _delegate;
    Shared<PlatformSkiaGraphicsDeviceWin> _device;
    Microsoft::WRL::ComPtr<ID3D12Fence> _fence;
    UINT64 _currentFenceValue = 0U;
    IntPx _width = 0;
    IntPx _height = 0;

    struct SwapChainSurface
    {
        Microsoft::WRL::ComPtr<ID3D12Resource> resource;
        sk_sp<SkSurface> surface;
        UINT64 fenceValue = 0U;
    };
    Microsoft::WRL::ComPtr<IDXGISwapChain3> _swapChain;
    std::vector<SwapChainSurface> _swapChainSurfaces;
};
}
}

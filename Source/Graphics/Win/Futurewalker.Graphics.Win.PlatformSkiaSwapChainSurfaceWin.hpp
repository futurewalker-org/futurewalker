// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.Win.PlatformSkiaSwapChainSurfaceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformSkiaGraphicsDeviceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformSwapChainSurfaceWin.hpp"

#include "Futurewalker.Core.PassKey.hpp"

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
    static auto Make(Shared<PlatformSkiaGraphicsDeviceWin> const& device, IntPx const width, IntPx const height) -> Shared<PlatformSkiaSwapChainSurfaceWin>;

    PlatformSkiaSwapChainSurfaceWin(PassKey<PlatformSkiaSwapChainSurfaceWin>, Shared<PlatformSkiaGraphicsDeviceWin> const& device, IntPx const width, IntPx const height);

    auto Resize(IntPx const width, IntPx const height) -> Bool override;
    auto Draw(Function<void(Scene& scene)> func) -> Bool override;
    auto NotifyDeviceLost() -> void override;
    auto HandleDeviceLost() -> void override;
    auto GetSwapChain() -> Microsoft::WRL::ComPtr<IUnknown> override;
    auto SetDelegate(Delegate delegate) -> void override;

private:
    auto GetSelf() -> Shared<PlatformSkiaSwapChainSurfaceWin>;
    auto HasSwapChain() const -> Bool;
    auto BuildSwapChain() -> Bool;
    auto ResizeBuffers() -> Bool;
    auto RebuildSurfaces() -> Bool;
    auto WaitIdle() -> void;
    auto ClearResources() -> void;
    auto BuildResources() -> Bool;

private:
    Weak<PlatformSkiaSwapChainSurfaceWin> _self;
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

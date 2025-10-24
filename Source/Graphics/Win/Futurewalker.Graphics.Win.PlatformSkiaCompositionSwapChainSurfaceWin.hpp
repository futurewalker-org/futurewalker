// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.Win.PlatformSkiaCompositionSwapChainSurfaceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformSkiaGraphicsDeviceWin.hpp"
#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformSwapChainSurfaceWin.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.PassKey.hpp"

#include <presentation.h>
#include <d3d11_3.h>
#include <dcomp.h>
#include <wrl/client.h>
#include <winrt/base.h>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Swap chain surface using composition swapchain API .
///
class PlatformSkiaCompositionSwapChainSurfaceWin : public PlatformSwapChainSurfaceWin
{
public:
    static auto Make(Shared<PlatformSkiaGraphicsDeviceWin> const& device, Shared<PlatformDCompositionDeviceWin> const& dcompDevice, IntPx const width, IntPx const height) -> Shared<PlatformSkiaCompositionSwapChainSurfaceWin>;

    PlatformSkiaCompositionSwapChainSurfaceWin(PassKey<PlatformSkiaCompositionSwapChainSurfaceWin> ,Shared<PlatformSkiaGraphicsDeviceWin> const& device, Shared<PlatformDCompositionDeviceWin> const& dcompDevice, IntPx const width, IntPx const height);

    auto Resize(IntPx const width, IntPx const height) -> Bool override;
    auto Draw(Function<void(Scene& scene)> func) -> Bool override;
    auto NotifyDeviceLost() -> void override;
    auto HandleDeviceLost() -> void override;
    auto GetSwapChain() -> Microsoft::WRL::ComPtr<IUnknown> override;
    auto SetDelegate(Delegate delegate) -> void override;

    auto GetPresentationManager() -> Microsoft::WRL::ComPtr<IPresentationManager>;
    auto GetPresentationSurface() -> Microsoft::WRL::ComPtr<IPresentationSurface>;

private:
    struct PresentationBuffer
    {
        Microsoft::WRL::ComPtr<ID3D12Resource> resource;
        Microsoft::WRL::ComPtr<IPresentationBuffer> buffer;
        winrt::handle bufferAvailableEvent;
        sk_sp<SkSurface> surface;
    };
    auto GetSelf() -> Shared<PlatformSkiaCompositionSwapChainSurfaceWin>;
    auto AllocPresentationBuffer() -> PresentationBuffer;
    auto UpdateSourceRect() -> void;
    auto ClearResources() -> void;
    auto BuildResources() -> void;

    Weak<PlatformSkiaCompositionSwapChainSurfaceWin> _self;
    IntPx _width = 0;
    IntPx _height = 0;
    Delegate _delegate;
    Shared<PlatformSkiaGraphicsDeviceWin> _device;
    Shared<PlatformDCompositionDeviceWin> _dcompDevice;
    Microsoft::WRL::ComPtr<IUnknown> _dcompSurface;
    Microsoft::WRL::ComPtr<IPresentationManager> _presentationManager;
    Microsoft::WRL::ComPtr<IPresentationSurface> _presentationSurface;
    Microsoft::WRL::ComPtr<ID3D11Fence> _fence;
    std::vector<PresentationBuffer> _presentationBuffers;
};
}
}

// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformDrawableViewLayerContextWinType.hpp"
#include "Futurewalker.Application.Win.PlatformDCompositionDeviceWinType.hpp"
#include "Futurewalker.Application.PlatformDrawableViewLayerContext.hpp"

#include "Futurewalker.Graphics.Win.PlatformGraphicsDeviceWin.hpp"
#include "Futurewalker.Graphics.Win.PlatformD3D11DeviceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformD3D12DeviceWinType.hpp"

#include "Futurewalker.Base.Win.PlatformInstanceHandleWin.hpp"

#include "Futurewalker.Core.PassKey.hpp"

#include <dxgi1_4.h>
#include <wrl/client.h>

#include <include/gpu/GrDirectContext.h>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Context object of platform view layers.
///
class PlatformDrawableViewLayerContextWin : public PlatformDrawableViewLayerContext
{
public:
    static auto Make(Shared<PlatformInstanceHandleWin> instanceHandle, Shared<PlatformDCompositionDeviceWin> dcompDevice, Shared<PlatformD3D12DeviceWin> d3d12Device)
      -> Shared<PlatformDrawableViewLayerContextWin>;

    PlatformDrawableViewLayerContextWin(
      PassKey<PlatformDrawableViewLayerContextWin>,
      Shared<PlatformInstanceHandleWin> instanceHandle,
      Shared<PlatformDCompositionDeviceWin> dcompDevice,
      Shared<PlatformD3D12DeviceWin> d3d12Device);

    ~PlatformDrawableViewLayerContextWin();

    auto MakeDrawableLayer() -> Shared<PlatformViewLayer> override;

    auto MakeSwapChainSurface(IntPx const width, IntPx const height) -> Shared<Graphics::PlatformSwapChainSurfaceWin>;

    auto MakeChildWindowHandle(HWND parent) -> HWND;
    auto DestroyChildWindowHandle(HWND hwnd) -> void;


private:
    auto GetSelf() -> Shared<PlatformDrawableViewLayerContextWin>;

private:
    static auto CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;

private:
    Weak<PlatformDrawableViewLayerContextWin> _self;
    Shared<PlatformInstanceHandleWin> _instanceHandle;
    Shared<PlatformDCompositionDeviceWin> _dcompDevice;
    Shared<PlatformD3D12DeviceWin> _d3d12Device;
    Shared<Graphics::PlatformGraphicsDeviceWin> _graphicsDevice;
    ATOM _windowClassAtom;
};

///
/// @brief
///
template <>
struct Locator::Resolver<PlatformDrawableViewLayerContextWin>
{
    using Interface = PlatformDrawableViewLayerContext;
    static auto Resolve() -> Shared<PlatformDrawableViewLayerContextWin>;
};
}
}

// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformDrawableViewLayerWinType.hpp"
#include "Futurewalker.Application.Win.PlatformDrawableViewLayerContextWinType.hpp"
#include "Futurewalker.Application.Win.PlatformDrawableViewLayerControlWinType.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerWin.hpp"
#include "Futurewalker.Application.Win.PlatformDCompositionDeviceWinType.hpp"
#include "Futurewalker.Application.PlatformDrawableViewLayerContextType.hpp" 

#include "Futurewalker.Graphics.Win.PlatformD3D11DeviceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformSwapChainSurfaceWin.hpp"
#include "Futurewalker.Graphics.DisplayListType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.Memory.hpp"

#include <dcomp.h>
#include <dxgi.h>
#include <d3d12.h>
#include <wrl/client.h>

#include <include/core/SkSurface.h>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformDrawableViewLayerWin : public PlatformViewLayerWin
{
public:
    static auto Make(Shared<PlatformDrawableViewLayerContextWin> context, Shared<PlatformDCompositionDeviceWin> dcompDevice) -> Shared<PlatformDrawableViewLayerWin>;

    PlatformDrawableViewLayerWin(PassKey<PlatformViewLayer> key, Shared<PlatformDrawableViewLayerContextWin> context, Shared<PlatformDCompositionDeviceWin> dcompDevice);

    auto SetSize(Size<Dp> const& size) -> void override;
    auto GetControl() -> Shared<PlatformViewLayerControl> override;

    auto Draw(Shared<Graphics::DisplayList> const& displayList, Offset<Dp> const& offset, Float64 const scale) -> Bool;

protected:
    auto Initialize() -> void override;

private:
    auto ResizeSurface() -> Bool;
    auto ChangeSwapChain(IDXGISwapChain1* swapChain) -> void;

private:
    auto CreateVisual() -> Microsoft::WRL::ComPtr<IDCompositionVisual3> override;
    auto DestroyVisual(Microsoft::WRL::ComPtr<IDCompositionVisual3> visual) -> void override;
    auto DestroySurface() -> void;
    auto GetSurfaceWidth() const -> IntPx;
    auto GetSurfaceHeight() const -> IntPx; 

private:
    Shared<PlatformDrawableViewLayerContextWin> _context;
    Shared<PlatformDrawableViewLayerControlWin> _control;
    Microsoft::WRL::ComPtr<IDCompositionVisual3> _visual;
    Shared<Graphics::PlatformSwapChainSurfaceWin> _surface;
    Size<Dp> _size;
};
}
}

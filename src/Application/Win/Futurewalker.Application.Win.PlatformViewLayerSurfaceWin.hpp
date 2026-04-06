// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformViewLayerSurfaceWinType.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerSurfaceContextWinType.hpp"

#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformD3D11DeviceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformSwapChainSurfaceWin.hpp"
#include "Futurewalker.Graphics.DisplayListType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

#include <dcomp.h>
#include <wrl/client.h>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief 
///
class PlatformViewLayerSurfaceWin : NonCopyable
{
public:
    PlatformViewLayerSurfaceWin();

    auto SetSize(Size<Dp> const& size) -> void;
    auto SetOffset(Vector<Dp> const& offset) -> void;
    auto SetDisplayScale(DisplayScale const scale) -> void;
    auto SetBackingScale(BackingScale const scale) -> void;
    auto SetVisual(Microsoft::WRL::ComPtr<IDCompositionVisual3> const& visual) -> void;

    auto Draw(Function<void(Graphics::Scene& scene)> const& function) -> void;
    auto Clear() -> void;

private:
    auto GetDisplayScale() const -> DisplayScale;
    auto GetBackingScale() const -> BackingScale;
    auto GetSurfaceWidth() const -> IntPx;
    auto GetSurfaceHeight() const -> IntPx;
    auto GetOffsetX() const -> IntPx;
    auto GetOffsetY() const -> IntPx;

    auto ResizeSurface() -> Bool;
    auto ChangeSwapChain(Microsoft::WRL::ComPtr<IUnknown> const& swapChain) -> void;

private:
    Size<Dp> _size;
    DisplayScale _displayScale = 1.0;
    BackingScale _backingScale = 1.0;
    Vector<Dp> _offset;
    Shared<PlatformViewLayerSurfaceContextWin> _context;
    Microsoft::WRL::ComPtr<IDCompositionVisual3> _visual;
    Shared<Graphics::PlatformSwapChainSurfaceWin> _surface;
};
}
}

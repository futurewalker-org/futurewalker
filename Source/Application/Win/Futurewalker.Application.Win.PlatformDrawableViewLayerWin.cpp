// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformDrawableViewLayerWin.hpp"
#include "Futurewalker.Application.Win.PlatformDrawableViewLayerControlWin.hpp"
#include "Futurewalker.Application.Win.PlatformDrawableViewLayerContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformDCompositionDeviceWin.hpp"

#include "Futurewalker.Graphics.Win.PlatformD3D11DeviceWin.hpp"
#include "Futurewalker.Graphics.Win.PlatformGraphicsDeviceWin.hpp"
#include "Futurewalker.Graphics.Scene.hpp."

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include <include/core/SkSurface.h>
#include <include/core/SkColorSpace.h>
#include <include/gpu/GrDirectContext.h>
#include <include/gpu/GrBackendSurface.h>
#include <include/gpu/d3d/GrD3DTypes.h>
#include <include/gpu/ganesh/SkSurfaceGanesh.h>

namespace FW_DETAIL_NS
{
///
/// @brief
///
auto PlatformDrawableViewLayerWin::Make(Shared<PlatformDrawableViewLayerContextWin> context, Shared<PlatformDCompositionDeviceWin> dcompDevice) -> Shared<PlatformDrawableViewLayerWin>
{
    return PlatformViewLayer::MakeDerived<PlatformDrawableViewLayerWin>(context, dcompDevice);
}

///
/// @brief
///
/// @param hwnd
/// @param dcompDevice
///
PlatformDrawableViewLayerWin::PlatformDrawableViewLayerWin(PassKey<PlatformViewLayer> key, Shared<PlatformDrawableViewLayerContextWin> context, Shared<PlatformDCompositionDeviceWin> dcompDevice)
  : PlatformViewLayerWin(key, dcompDevice)
  , _context {context}
{
}

///
/// @brief
///
/// @param size
///
auto PlatformDrawableViewLayerWin::SetSize(Size<Dp> const& size) -> void
{
    if (_size != size)
    {
        _size = size;
    }
    PlatformViewLayerWin::SetSize(size);
}

///
/// @brief
///
auto PlatformDrawableViewLayerWin::GetControl() -> Shared<PlatformViewLayerControl>
{
    return _control;
}

///
/// @brief 
///
/// @param displayList 
///
auto PlatformDrawableViewLayerWin::Draw(Shared<Graphics::DisplayList> const& displayList, Offset<Dp> const& offset, Float64 const scale) -> Bool
{
    if (displayList)
    {
        if (ResizeSurface())
        {
            if (_surface)
            {
                return _surface->Draw([&](Graphics::Scene& scene) {
                    scene.PushTranslate({.x = offset.GetDeltaX(), .y = offset.GetDeltaY()});
                    scene.PushScale({.x = scale, .y = scale});
                    scene.AddDisplayList({.displayList = displayList});
                    scene.Pop({});
                    scene.Pop({});
                });
            }
        }
    }
    else
    {
        DestroySurface();
        return true;
    }
    return false;
}

///
/// @brief
///
auto PlatformDrawableViewLayerWin::Initialize() -> void
{
    _control = Shared<PlatformDrawableViewLayerControlWin>::Make(GetSelf());
}

///
/// @brief 
///
auto PlatformDrawableViewLayerWin::ResizeSurface() -> Bool
{
    const auto width = GetSurfaceWidth();
    const auto height = GetSurfaceHeight();
    if (width <= 0 || height <= 0)
    {
        _surface.Reset();
        return false;
    }

    if (_surface)
    {
        if (_surface->Resize(width, height))
        {
            return true;
        }
        _surface.Reset();
    }
    else
    {
        if (_context && _visual)
        {
            _surface = _context->MakeSwapChainSurface(width, height);
            if (_surface)
            {
                _visual->SetContent(_surface->GetSwapChain().Get());

                _surface->SetDelegate({
                  .deviceLost = [this](auto&) { ChangeSwapChain(nullptr); },
                  .deviceRestored = [this](auto& surface) { ChangeSwapChain(surface.GetSwapChain().Get()); },
                });

                return true;
            }
        }
    }
    return false;
}

///
/// @brief
///
auto PlatformDrawableViewLayerWin::ChangeSwapChain(IDXGISwapChain1* swapChain) -> void
{
    if (_visual)
    {
        _visual->SetContent(swapChain);
    }
}

///
/// @brief
///
auto PlatformDrawableViewLayerWin::CreateVisual() -> Microsoft::WRL::ComPtr<IDCompositionVisual3>
{
    if (const auto dcompDevice = GetDCompositionDevice())
    {
        if (const auto visual = dcompDevice->CreateVisual())
        {
            _visual = visual;
            return _visual;
        }
    }
    return nullptr;
}

///
/// @brief
///
auto PlatformDrawableViewLayerWin::DestroyVisual(Microsoft::WRL::ComPtr<IDCompositionVisual3> visual) -> void
{
    (void)visual;
    _surface = nullptr;
    _visual = nullptr;
}

///
/// @brief 
///
auto PlatformDrawableViewLayerWin::DestroySurface() -> void
{
    ChangeSwapChain(nullptr);
    _surface.Reset();
}

///
/// @brief 
///
auto PlatformDrawableViewLayerWin::GetSurfaceWidth() const -> IntPx
{
    DisplayScale const displayScale = GetDisplayScale();
    BackingScale const backingScale = GetBackingScale();
    return static_cast<IntPx>(Px::Round(UnitFunction::ConvertDpToPx(_size.GetWidth(), displayScale, backingScale)));
}

///
/// @brief
///
auto PlatformDrawableViewLayerWin::GetSurfaceHeight() const -> IntPx
{
    DisplayScale const displayScale = GetDisplayScale();
    BackingScale const backingScale = GetBackingScale();
    return static_cast<IntPx>(Px::Round(UnitFunction::ConvertDpToPx(_size.GetHeight(), displayScale, backingScale)));
}
}

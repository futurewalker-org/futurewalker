// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformViewLayerSurfaceWin.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerSurfaceContextWin.hpp"

#include "Futurewalker.Graphics.Win.PlatformSwapChainSurfaceWin.hpp"
#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWin.hpp"

#include "Futurewalker.Base.Debug.hpp"
#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
PlatformViewLayerSurfaceWin::PlatformViewLayerSurfaceWin()
{
    _context = Locator::Resolve<PlatformViewLayerSurfaceContextWin>();
}

auto PlatformViewLayerSurfaceWin::SetSize(Size<Dp> const& size) -> void
{
    _size = size;
}

auto PlatformViewLayerSurfaceWin::SetOffset(Vector<Dp> const& offset) -> void
{
    _offset = offset;
}

auto PlatformViewLayerSurfaceWin::SetDisplayScale(DisplayScale const scale) -> void
{
    _displayScale = scale;
}

auto PlatformViewLayerSurfaceWin::SetBackingScale(BackingScale const scale) -> void
{
    _backingScale = scale;
}

auto PlatformViewLayerSurfaceWin::SetVisual(Microsoft::WRL::ComPtr<IDCompositionVisual3> const& visual) -> void
{
    if (_visual != visual)
    {
        if (_visual)
        {
            _visual->SetContent(nullptr);
        }

        _visual = visual;

        if (_visual && _surface)
        {
            ChangeSwapChain(_surface->GetSwapChain());
        }
        else
        {
            _visual->SetContent(nullptr);
        }
    }
}

auto PlatformViewLayerSurfaceWin::Draw(Function<void(Graphics::Scene& scene)> const& function) -> void
{
    if (ResizeSurface())
    {
        if (_surface)
        {
            _surface->SetOffset(GetOffsetX(), GetOffsetY());
            _surface->Draw(function);
        }
    }
}

///
/// @brief Sets the size to zero and destroys the surface.
///
auto PlatformViewLayerSurfaceWin::Clear() -> void
{
    SetSize({});
    ResizeSurface();
}

auto PlatformViewLayerSurfaceWin::GetDisplayScale() const -> DisplayScale
{
    return _displayScale;
}

auto PlatformViewLayerSurfaceWin::GetBackingScale() const -> BackingScale
{
    return _backingScale;
}

auto PlatformViewLayerSurfaceWin::GetSurfaceWidth() const -> IntPx
{
    DisplayScale const displayScale = GetDisplayScale();
    BackingScale const backingScale = GetBackingScale();
    return static_cast<IntPx>(Px::Round(UnitFunction::ConvertDpToPx(_size.width, displayScale, backingScale)));
}

auto PlatformViewLayerSurfaceWin::GetSurfaceHeight() const -> IntPx
{
    DisplayScale const displayScale = GetDisplayScale();
    BackingScale const backingScale = GetBackingScale();
    return static_cast<IntPx>(Px::Round(UnitFunction::ConvertDpToPx(_size.height, displayScale, backingScale)));
}

auto PlatformViewLayerSurfaceWin::GetOffsetX() const -> IntPx
{
    DisplayScale const displayScale = GetDisplayScale();
    BackingScale const backingScale = GetBackingScale();
    return static_cast<IntPx>(Px::Round(UnitFunction::ConvertDpToPx(_offset.x, displayScale, backingScale)));
}

auto PlatformViewLayerSurfaceWin::GetOffsetY() const -> IntPx
{
    DisplayScale const displayScale = GetDisplayScale();
    BackingScale const backingScale = GetBackingScale();
    return static_cast<IntPx>(Px::Round(UnitFunction::ConvertDpToPx(_offset.y, displayScale, backingScale)));
}

auto PlatformViewLayerSurfaceWin::ResizeSurface() -> Bool
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
                ChangeSwapChain(_surface->GetSwapChain());

                _surface->SetDelegate({
                    .deviceLost = [this](auto&) { ChangeSwapChain(nullptr); },
                    .deviceRestored = [this](auto& surface) { ChangeSwapChain(surface.GetSwapChain()); },
                });

                return true;
            }
        }
    }
    return false;
}

auto PlatformViewLayerSurfaceWin::ChangeSwapChain(Microsoft::WRL::ComPtr<IUnknown> const& swapChain) -> void
{
    if (_visual)
    {
        _visual->SetContent(swapChain.Get());
    }
}
}
}

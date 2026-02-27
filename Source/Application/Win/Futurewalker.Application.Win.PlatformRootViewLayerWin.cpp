// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformRootViewLayerWin.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerVisualWin.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerVisualContextWin.hpp"
#include "Futurewalker.Application.PlatformViewLayerVisualRenderer.hpp"

#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWin.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include <chrono>

namespace FW_DETAIL_NS
{
///
/// @brief
///
/// @param hwnd
/// @param dcompDevice
///
auto PlatformRootViewLayerWin::Make(HWND hwnd, Shared<PlatformDCompositionDeviceWin> dcompDevice) -> Shared<PlatformRootViewLayerWin>
{
    return PlatformViewLayer::MakeDerived<PlatformRootViewLayerWin>(hwnd, dcompDevice);
}

///
/// @brief 
///
/// @param key 
/// @param hwnd 
/// @param dcompDevice 
///
PlatformRootViewLayerWin::PlatformRootViewLayerWin(PassKey<PlatformViewLayer> key, HWND hwnd, Shared<PlatformDCompositionDeviceWin> dcompDevice)
  : PlatformViewLayerWin(key, dcompDevice)
  , _hwnd {hwnd}
{
    _dcompTarget = MakeTarget(_hwnd);
    _dcompVisual = dcompDevice->CreateVisual();

    if (_dcompTarget && _dcompVisual)
    {
        _dcompTarget->SetRoot(_dcompVisual.Get());
    }
}

///
/// @brief
///
auto PlatformRootViewLayerWin::GetControl() -> Shared<PlatformViewLayerControl>
{
    return {};
}

auto PlatformRootViewLayerWin::Render() -> void
{
    if (_renderer)
    {
        _renderer->Render();
    }
}

auto PlatformRootViewLayerWin::Initialize() -> void
{
    PlatformViewLayerWin::Initialize();

    SetClipMode(ViewClipMode::Bounds);

    auto const layer = GetSelf();
    auto const context = Shared<PlatformViewLayerVisualContextWin>::Make(GetCompositionDevice());
    _renderer = Unique<PlatformViewLayerVisualRenderer>::Make(
      PlatformViewLayerVisualRenderer::Delegate {
          .renderBegin = [&]() -> void { _dcompVisual->RemoveAllVisuals(); },
          .renderEnd = []() -> void {},
          .renderVisual = [&](Shared<PlatformViewLayerVisual> const& visual) -> void {
              if (auto const visualWin = visual.TryAs<PlatformViewLayerVisualWin>())
              {
                  if (auto const dcompVisual = visualWin->GetVisual())
                  {
                      _dcompVisual->AddVisual(dcompVisual.Get(), FALSE, nullptr);
                  }
                  visualWin->Render();
              }
          },
      },
      layer,
      context);
}

///
/// @brief
///
auto PlatformRootViewLayerWin::RootGetWindowHandle() const -> HWND
{
    return _hwnd;
}

///
/// @brief
///
auto PlatformRootViewLayerWin::RootGetDisplayScale() const -> DisplayScale
{
    if (_hwnd)
    {
        auto const dpi = ::GetDpiForWindow(_hwnd);
        return DisplayScale(dpi) / DisplayScale(USER_DEFAULT_SCREEN_DPI);
    }
    return 1.0;
}

///
/// @brief
///
auto PlatformRootViewLayerWin::RootGetBackingScale() const -> BackingScale
{
    return 1.0;
}

///
/// @brief Notify offset change.
///
auto PlatformRootViewLayerWin::RootOffsetChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
    if (_renderer)
    {
        _renderer->RequestUpdateLayer();
    }
}

///
/// @brief Notify size change.
///
auto PlatformRootViewLayerWin::RootSizeChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
    if (_renderer)
    {
        _renderer->RequestUpdateLayer();
    }
}

///
/// @brief Notify clip path change.
///
auto PlatformRootViewLayerWin::RootClipPathChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
    if (_renderer)
    {
        _renderer->RequestUpdateLayer();
    }
}

///
/// @brief Notify clip mode change.
///
auto PlatformRootViewLayerWin::RootClipModeChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
    if (_renderer)
    {
        _renderer->RequestUpdateLayer();
    }
}

///
/// @brief Notify opacity change.
///
auto PlatformRootViewLayerWin::RootOpacityChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
    if (_renderer)
    {
        _renderer->RequestUpdateLayer();
    }
}

///
/// @brief Notify render flags change.
///
auto PlatformRootViewLayerWin::RootRenderFlagsChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
    if (_renderer)
    {
        _renderer->RequestRebuildLayer();
    }
}

///
/// @brief Notify display list change.
///
auto PlatformRootViewLayerWin::RootDisplayListChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
    // TODO: Optimize to only update changed parts.
    if (_renderer)
    {
        _renderer->RequestUpdateLayer();
    }
}

///
/// @brief Notify display list offset change.
///
auto PlatformRootViewLayerWin::RootDisplayListOffsetChanged(Shared<PlatformViewLayer> const& layer) -> void
{
    (void)layer;
    // TODO: Optimize to only update changed parts.
    if (_renderer)
    {
        _renderer->RequestUpdateLayer();
    }
}

///
/// @brief Notify child added.
///
auto PlatformRootViewLayerWin::RootChildAdded(Shared<PlatformViewLayer> const& child) -> void
{
    (void)child;
    if (_renderer)
    {
        _renderer->RequestRebuildLayer();
    }
}

///
/// @brief Notify child removed.
///
auto PlatformRootViewLayerWin::RootChildRemoved(Shared<PlatformViewLayer> const& parent) -> void
{
    (void)parent;
    if (_renderer)
    {
        _renderer->RequestRebuildLayer();
    }
}

///
/// @brief
///
/// @param hwnd
///
auto PlatformRootViewLayerWin::MakeTarget(HWND hwnd) -> Microsoft::WRL::ComPtr<IDCompositionTarget>
{
    if (const auto dcompDevice = GetCompositionDevice())
    {
        return dcompDevice->CreateTargetForHwnd(hwnd);
    }
    return {};
}
}

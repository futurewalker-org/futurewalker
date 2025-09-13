// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformRootViewLayerWinType.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerWin.hpp"
#include "Futurewalker.Application.Win.PlatformDCompositionDeviceWinType.hpp"

#include "Futurewalker.Graphics.Win.PlatformD3D11DeviceWinType.hpp"

#include "Futurewalker.Core.Memory.hpp"

#include <dcomp.h>
#include <dxgi.h>
#include <wrl/client.h>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Windows implementation of root PlatformViewLayerWin.
///
class PlatformRootViewLayerWin : public PlatformViewLayerWin
{
public:
    static auto Make(HWND hwnd, Shared<PlatformDCompositionDeviceWin> dcompDevice) -> Shared<PlatformRootViewLayerWin>;

    PlatformRootViewLayerWin(PassKey<PlatformViewLayer> key, HWND hwnd, Shared<PlatformDCompositionDeviceWin> dcompDevice);

    auto GetControl() -> Shared<PlatformViewLayerControl> override;

    using PlatformViewLayerWin::NotifyDisplayScaleChanged;

private:
    auto CreateVisual() -> Microsoft::WRL::ComPtr<IDCompositionVisual3> override;
    auto DestroyVisual(Microsoft::WRL::ComPtr<IDCompositionVisual3> visual) -> void override;

private:
    auto RootGetWindowHandle() const -> HWND override;
    auto RootGetVisual() const -> Microsoft::WRL::ComPtr<IDCompositionVisual3> override;
    auto RootGetDisplayScale() const -> DisplayScale override;

private:
    auto MakeTarget(HWND hwnd) -> Microsoft::WRL::ComPtr<IDCompositionTarget>;
    auto MakeVisual() -> Microsoft::WRL::ComPtr<IDCompositionVisual3>;

private:
    HWND _hwnd = NULL;
    Microsoft::WRL::ComPtr<IDCompositionTarget> _target;
    Microsoft::WRL::ComPtr<IDCompositionVisual3> _visual;
};
}
}

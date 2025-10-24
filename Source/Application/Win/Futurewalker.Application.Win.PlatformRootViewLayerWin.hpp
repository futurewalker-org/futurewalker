// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformRootViewLayerWinType.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerWin.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerVisualWin.hpp"

#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWinType.hpp"
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

    auto Render() -> void;

    using PlatformViewLayerWin::NotifyDisplayScaleChanged;

protected:
    auto Initialize() -> void override;

private:
    auto RootGetWindowHandle() const -> HWND override;
    auto RootGetDisplayScale() const -> DisplayScale override;

private:
    auto RootOffsetChanged(Shared<PlatformViewLayerWin> const& layer) -> void override;
    auto RootSizeChanged(Shared<PlatformViewLayerWin> const& layer) -> void override;
    auto RootClipModeChanged(Shared<PlatformViewLayerWin> const& layer) -> void override;
    auto RootOpacityChanged(Shared<PlatformViewLayerWin> const& layer) -> void override;
    auto RootRenderFlagsChanged(Shared<PlatformViewLayerWin> const& layer) -> void override;
    auto RootDisplayListChanged(Shared<PlatformViewLayerWin> const& layer) -> void override;
    auto RootDisplayListOffsetChanged(Shared<PlatformViewLayerWin> const& layer) -> void override;
    auto RootChildAdded(Shared<PlatformViewLayerWin> const& child) -> void override;
    auto RootChildRemoved(Shared<PlatformViewLayerWin> const& parent) -> void override;

private:
    auto MakeTarget(HWND hwnd) -> Microsoft::WRL::ComPtr<IDCompositionTarget>;

    auto FindVisualByBaseLayerId(PlatformViewLayerId const layerId) -> Shared<PlatformViewLayerVisualWin>;
    auto FindVisualByLayerId(PlatformViewLayerId const layerId) -> Shared<PlatformViewLayerVisualWin>;

    auto RebuildVisual(Shared<PlatformViewLayerWin> const& baseLayer) -> void;
    auto RebuildVisualCore(auto& builder, Shared<PlatformViewLayerWin> const& layer) -> void;

    auto UpdateVisual(Shared<PlatformViewLayerWin> const& layer) -> void;
    auto UpdateVisualCore(Shared<PlatformViewLayerWin> layer, Shared<PlatformViewLayerVisualWin> currentVisual) -> void;

private:
    HWND _hwnd = NULL;
    Microsoft::WRL::ComPtr<IDCompositionTarget> _target;
    Microsoft::WRL::ComPtr<IDCompositionVisual3> _visual;
    PlatformViewLayerVisualWinArray _visuals;
};
}
}

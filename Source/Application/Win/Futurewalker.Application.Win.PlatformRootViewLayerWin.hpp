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

protected:
    auto Initialize() -> void override;

private:
    auto RootGetWindowHandle() const -> HWND override;
    auto RootGetDisplayScale() const -> DisplayScale override;
    auto RootGetBackingScale() const -> BackingScale override;

private:
    auto RootOffsetChanged(Shared<PlatformViewLayer> const& layer) -> void override;
    auto RootSizeChanged(Shared<PlatformViewLayer> const& layer) -> void override;
    auto RootClipModeChanged(Shared<PlatformViewLayer> const& layer) -> void override;
    auto RootOpacityChanged(Shared<PlatformViewLayer> const& layer) -> void override;
    auto RootRenderFlagsChanged(Shared<PlatformViewLayer> const& layer) -> void override;
    auto RootDisplayListChanged(Shared<PlatformViewLayer> const& layer) -> void override;
    auto RootDisplayListOffsetChanged(Shared<PlatformViewLayer> const& layer) -> void override;
    auto RootChildAdded(Shared<PlatformViewLayer> const& child) -> void override;
    auto RootChildRemoved(Shared<PlatformViewLayer> const& parent) -> void override;

private:
    auto MakeTarget(HWND hwnd) -> Microsoft::WRL::ComPtr<IDCompositionTarget>;

    auto FindBaseVisualByBaseLayerId(PlatformViewLayerId const layerId) -> Shared<PlatformViewLayerVisualWin>;

    auto RebuildVisual(Shared<PlatformViewLayerWin> const& baseLayer) -> void;
    auto UpdateVisual() -> void;

    auto QueueRebuildLayer(PlatformViewLayerId const layerId) -> void;
    auto QueueUpdateLayer() -> void;

private:
    HWND _hwnd = NULL;
    Microsoft::WRL::ComPtr<IDCompositionTarget> _dcompTarget;
    Microsoft::WRL::ComPtr<IDCompositionVisual3> _dcompVisual;
    Shared<PlatformViewLayerVisualWin> _visual;
    std::vector<PlatformViewLayerId> _layersToRebuild;
    Bool _shouldUpdateLayer = false;
};
}
}

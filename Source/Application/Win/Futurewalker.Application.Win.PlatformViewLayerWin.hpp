// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformViewLayerWinType.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerSurfaceWin.hpp"
#include "Futurewalker.Application.PlatformViewLayer.hpp"

#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformSwapChainSurfaceWinType.hpp"
#include "Futurewalker.Graphics.SceneType.hpp"

#include "Futurewalker.Core.Function.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.ReferenceArg.hpp"

#include <wrl/client.h>
#include <dcomp.h>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Windows implementation of PlatformViewLayer
///
/// ### Design overview
///
/// PlatformViewLayerWin utilizes Direct Composition to present content of layers efficiently.
/// Each layer must provide `IDCompositionVisual` object representing content of layer.
///
/// Conceptually, PlatformViewLayerWin manages two different kind of trees: `IDCompositionVisual` tree and `HWND` tree.
/// The former is used to display content of layers and the latter is for handling input (like mouse and keyboards).
///
/// See PlatformRootViewLayerWin for more details.
///
/// ### HWND based layers
///
/// In addition to Direct Composition based API, it also supports HWND based layers.
/// Target HWND must be layered child window and its content should be redirected to `IDCompositionVisual` by `IDCompositionDevice::CreateSurfaceFromHwnd()`.
///
class PlatformViewLayerWin : public PlatformViewLayer
{
public:
    static auto Make(Shared<PlatformDCompositionDeviceWin> const& dcompDevice) -> Shared<PlatformViewLayerWin>;

    PlatformViewLayerWin(PassKey<PlatformViewLayer> key, Shared<PlatformDCompositionDeviceWin> const& dcompDevice);

protected:
    auto Initialize() -> void override;
    auto OnAttach() -> void override;
    auto OnDetach() -> void override;
    auto GetCompositionDevice() const -> Shared<PlatformDCompositionDeviceWin>;

private:
    auto NotifyRootWindowHandleChanged(HWND const rootHwnd) -> void;

    auto GetBelowWindowHandle() const -> HWND;
    auto GetBelowWindowHandleCore(Shared<PlatformViewLayerWin const> child) const -> HWND;

private:
    virtual auto CreateWindowHandle(HWND parent) -> HWND;
    virtual auto DestroyWindowHandle(HWND handle) -> void;
    virtual auto CreateVisual() -> Microsoft::WRL::ComPtr<IDCompositionVisual3>;
    virtual auto DestroyVisual(Microsoft::WRL::ComPtr<IDCompositionVisual3> visual) -> void;

private:
    virtual auto RootGetWindowHandle() const -> HWND;

private:
    auto InternalGetWindowHandle() const -> HWND;

private:
    Shared<PlatformDCompositionDeviceWin> _dcompDevice;
    Weak<PlatformViewLayerWin> _parent;
    PlatformViewLayerWinList _children;
    PlatformViewLayerRenderFlags _renderFlags = PlatformViewLayerRenderFlags::None;
    Offset<Dp> _offset;
    Size<Dp> _size;
    ViewClipMode _clipMode = ViewClipMode::None;
    Float64 _opacity = 1.0;
    HWND _rootHwnd = NULL;
    HWND _hwnd = NULL;
    Shared<Graphics::DisplayList> _displayList;
    Offset<Dp> _displayListOffset;
};
}
}

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

    auto AddChild(Shared<PlatformViewLayer> child, Shared<PlatformViewLayer> after) -> void final override;
    auto RemoveFromParent() -> void final override;
    auto GetChildren() -> PlatformViewLayerArray final override;
    auto GetControl() -> Shared<PlatformViewLayerControl> override;
    auto SetOffset(Offset<Dp> const& offset) -> void final override;
    auto SetSize(Size<Dp> const& size) -> void final override;
    auto SetClipMode(ViewClipMode const clipMode) -> void final override;
    auto SetOpacity(Float64 const opacity) -> void final override;
    auto SetRenderFlags(PlatformViewLayerRenderFlags const renderFlags) -> void override;
    auto SetDisplayList(Shared<Graphics::DisplayList> const& displayList) -> void override;
    auto SetDisplayListOffset(Offset<Dp> const& offset) -> void override;

    auto GetSize() const -> Size<Dp>;
    auto GetOffset() const -> Offset<Dp>;
    auto GetClipMode() const -> ViewClipMode;
    auto GetOpacity() const -> Float64;
    auto GetDisplayList() const -> Shared<Graphics::DisplayList>;
    auto GetDisplayListOffset() const -> Offset<Dp>;
    auto GetRenderFlags() const -> PlatformViewLayerRenderFlags;

    auto GetDisplayScale() const -> DisplayScale;
    auto GetBackingScale() const -> BackingScale;

    auto GetParent() -> Shared<PlatformViewLayerWin>;
    auto GetParent() const -> Shared<PlatformViewLayerWin const>;

    auto IsRoot() const -> Bool;
    auto GetRoot() -> Shared<PlatformViewLayerWin>;
    auto GetRoot() const -> Shared<PlatformViewLayerWin const>;

    auto ShouldRasterize() const -> Bool;

protected:
    auto Initialize() -> void override;
    auto NotifyRootChanged() -> void;
    auto NotifyDisplayScaleChanged() -> void;

    auto GetCompositionDevice() const -> Shared<PlatformDCompositionDeviceWin>;

private:
    auto NotifyRootWindowHandleChanged(HWND const rootHwnd) -> void;
    auto NotifyRootDisplayScaleChanged(DisplayScale const rootDisplayScale) -> void;

    auto GetBelowWindowHandle() const -> HWND;
    auto GetBelowWindowHandleCore(Shared<PlatformViewLayerWin const> child) const -> HWND;

private:
    virtual auto CreateWindowHandle(HWND parent) -> HWND;
    virtual auto DestroyWindowHandle(HWND handle) -> void;
    virtual auto CreateVisual() -> Microsoft::WRL::ComPtr<IDCompositionVisual3>;
    virtual auto DestroyVisual(Microsoft::WRL::ComPtr<IDCompositionVisual3> visual) -> void;

private:
    virtual auto RootGetWindowHandle() const -> HWND;
    virtual auto RootGetDisplayScale() const -> DisplayScale;
    virtual auto RootOffsetChanged(Shared<PlatformViewLayerWin> const& layer) -> void;
    virtual auto RootSizeChanged(Shared<PlatformViewLayerWin> const& layer) -> void;
    virtual auto RootClipModeChanged(Shared<PlatformViewLayerWin> const& layer) -> void;
    virtual auto RootOpacityChanged(Shared<PlatformViewLayerWin> const& layer) -> void;
    virtual auto RootRenderFlagsChanged(Shared<PlatformViewLayerWin> const& layer) -> void;
    virtual auto RootDisplayListChanged(Shared<PlatformViewLayerWin> const& layer) -> void;
    virtual auto RootDisplayListOffsetChanged(Shared<PlatformViewLayerWin> const& layer) -> void;
    virtual auto RootChildAdded(Shared<PlatformViewLayerWin> const& child) -> void;
    virtual auto RootChildRemoved(Shared<PlatformViewLayerWin> const& parent) -> void;

private:
    auto InternalGetWindowHandle() const -> HWND;
    auto InternalGetDisplayScale() const -> DisplayScale;
    auto InternalGetParentWindowHandle() const -> HWND;
    auto InternalAttach() -> void;
    auto InternalDetach() -> void;

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
    DisplayScale _rootDisplayScale = 1.0;
    HWND _hwnd = NULL;
    DisplayScale _displayScale = 1.0;
    Shared<Graphics::DisplayList> _displayList;
    Offset<Dp> _displayListOffset;
};
}
}

// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.Win.PlatformViewLayerWinType.hpp"
#include "Futurewalker.Application.Win.PlatformDCompositionDeviceWinType.hpp"
#include "Futurewalker.Application.PlatformViewLayer.hpp"

#include "Futurewalker.Core.PassKey.hpp"

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
    PlatformViewLayerWin(PassKey<PlatformViewLayer> key, Shared<PlatformDCompositionDeviceWin> const& dcompDevice);

    auto AddChild(Shared<PlatformViewLayer> child, Shared<PlatformViewLayer> after) -> void override;
    auto RemoveFromParent() -> void override;
    auto GetChildren() -> PlatformViewLayerArray override;
    auto SetOffset(Offset<Dp> const& offset) -> void override;
    auto SetSize(Size<Dp> const& size) -> void override;
    auto SetClipMode(ViewClipMode const clipMode) -> void override;
    auto SetOpacity(Float64 const opacity) -> void override;

protected:
    auto GetDisplayScale() const -> DisplayScale;
    auto GetBackingScale() const -> BackingScale;

    auto GetParent() -> Shared<PlatformViewLayerWin>;
    auto GetParent() const -> Shared<PlatformViewLayerWin const>;

    auto GetRoot() -> Shared<PlatformViewLayerWin>;
    auto GetRoot() const -> Shared<PlatformViewLayerWin const>;

    auto IsRoot() const -> Bool;
    auto NotifyRootChanged() -> void;

    auto GetDCompositionDevice() -> Shared<PlatformDCompositionDeviceWin>;

private:
    auto NotifyRootWindowHandleChanged(HWND const rootHwnd) -> void;
    auto NotifyRootVisualChanged(Microsoft::WRL::ComPtr<IDCompositionVisual3> rootVisual) -> void;

    auto GetBelowWindowHandle() const -> HWND;
    auto GetBelowWindowHandleCore(Shared<PlatformViewLayerWin const> child) const -> HWND;

    auto GetPrevChildVisual(Shared<PlatformViewLayer> child) -> Microsoft::WRL::ComPtr<IDCompositionVisual3>;

private:
    ///
    /// @brief Create visual node for this layer.
    ///
    /// @return Newly created visual node. Should not be null.
    ///
    virtual auto CreateVisual() -> Microsoft::WRL::ComPtr<IDCompositionVisual3> = 0;

    ///
    /// @brief Destroy previously created visual.
    ///
    /// @param[in] visual Visual object created by CreateVisual().
    ///
    virtual auto DestroyVisual(Microsoft::WRL::ComPtr<IDCompositionVisual3> visual) -> void = 0;

private:
    virtual auto CreateWindowHandle(HWND parent) -> HWND;
    virtual auto DestroyWindowHandle(HWND handle) -> void;

private:
    virtual auto RootGetVisual() const -> Microsoft::WRL::ComPtr<IDCompositionVisual3>;
    virtual auto RootGetWindowHandle() const -> HWND;

private:
    auto InternalGetVisual() const -> Microsoft::WRL::ComPtr<IDCompositionVisual3>;
    auto InternalGetWindowHandle() const -> HWND;
    auto InternalGetParentVisual() const -> Microsoft::WRL::ComPtr<IDCompositionVisual3>;
    auto InternalGetParentWindowHandle() const -> HWND;
    auto InternalAttach() -> void;
    auto InternalDetach() -> void;

    auto InternalUpdateClip() -> void;
    auto InternalUpdateOffset() -> void;
    auto InternalUpdateOpacity() -> void;

private:
    Shared<PlatformDCompositionDeviceWin> _dcompDevice;
    Weak<PlatformViewLayerWin> _parent;
    PlatformViewLayerWinList _children;
    Offset<Dp> _offset;
    Size<Dp> _size;
    ViewClipMode _clipMode = ViewClipMode::None;
    Float64 _opacity = 1.0;
    Microsoft::WRL::ComPtr<IDCompositionVisual3> _rootVisual;
    HWND _rootHwnd = NULL;
    Microsoft::WRL::ComPtr<IDCompositionVisual3> _visual;
    HWND _hwnd = NULL;
};
}
}

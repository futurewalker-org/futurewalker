// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformViewLayerWin.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerSurfaceWin.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayerVisualWin.hpp"

#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWin.hpp"
#include "Futurewalker.Graphics.Win.PlatformSwapChainSurfaceWin.hpp"
#include "Futurewalker.Graphics.Scene.hpp"

#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
/// @param dcompDevice 
///
auto PlatformViewLayerWin::Make(Shared<PlatformDCompositionDeviceWin> const& dcompDevice) -> Shared<PlatformViewLayerWin>
{
    return PlatformViewLayer::MakeDerived<PlatformViewLayerWin>(dcompDevice);
}

///
/// @brief
///
PlatformViewLayerWin::PlatformViewLayerWin(PassKey<PlatformViewLayer> key, Shared<PlatformDCompositionDeviceWin> const& dcompDevice)
  : PlatformViewLayer(key)
  , _dcompDevice {dcompDevice}
{
    FW_DEBUG_ASSERT(_dcompDevice);
}

///
/// @brief 
///
/// @param child 
/// @param after 
///
auto PlatformViewLayerWin::AddChild(Shared<PlatformViewLayer> child, Shared<PlatformViewLayer> after) -> void
{
    if (auto childWin = child.Maybe<PlatformViewLayerWin>())
    {
        child->RemoveFromParent();

        auto const it = std::find(_children.begin(), _children.end(), after);
        _children.insert(it, childWin);

        childWin->_parent = GetSelf();
        childWin->InternalAttach();

        GetRoot()->RootChildAdded(childWin);
    }
}

///
/// @brief 
///
auto PlatformViewLayerWin::RemoveFromParent() -> void
{
    if (auto parent = _parent.Lock())
    {
        parent->InternalDetach();
        auto const it = std::remove(parent->_children.begin(), parent->_children.end(), GetSelf());
        parent->_children.erase(it, parent->_children.end());
        _parent = nullptr;

        parent->GetRoot()->RootChildRemoved(parent);
    }
}

///
/// @brief 
///
auto PlatformViewLayerWin::GetChildren() -> PlatformViewLayerArray
{
    return PlatformViewLayerArray(_children.begin(), _children.end());
}

auto PlatformViewLayerWin::GetControl() -> Shared<PlatformViewLayerControl>
{
    return {};
}

///
/// @brief 
///
/// @param pos 
///
auto PlatformViewLayerWin::SetOffset(Offset<Dp> const& offset) -> void
{
    if (_offset != offset)
    {
        _offset = offset;
        GetRoot()->RootOffsetChanged(GetSelf());
    }
}

///
/// @brief
///
/// @param size
///
auto PlatformViewLayerWin::SetSize(Size<Dp> const& size) -> void
{
    if (_size != size)
    {
        _size = size;
        GetRoot()->RootSizeChanged(GetSelf());
    }
}

///
/// @brief 
///
/// @param clipMode 
///
auto PlatformViewLayerWin::SetClipMode(ViewClipMode const clipMode) -> void
{
    if (_clipMode != clipMode)
    {
        _clipMode = clipMode;
        GetRoot()->RootClipModeChanged(GetSelf());
    }
}

///
/// @brief
///
/// @param opacity
///
auto PlatformViewLayerWin::SetOpacity(Float64 const opacity) -> void
{
    if (_opacity != opacity)
    {
        _opacity = opacity;
        GetRoot()->RootOpacityChanged(GetSelf());
    }
}

///
/// @brief
///
auto PlatformViewLayerWin::SetRenderFlags(PlatformViewLayerRenderFlags const renderFlags) -> void
{
    if (_renderFlags != renderFlags)
    {
        _renderFlags = renderFlags;
        GetRoot()->RootRenderFlagsChanged(GetSelf());
    }
}

///
/// @brief
///
auto PlatformViewLayerWin::SetDisplayList(Shared<Graphics::DisplayList> const& displayList) -> void
{
    if (_displayList != displayList)
    {
        _displayList = displayList;
        GetRoot()->RootDisplayListChanged(GetSelf());
    }
}

///
/// @brief
///
auto PlatformViewLayerWin::SetDisplayListOffset(Offset<Dp> const& offset) -> void
{
    if (_displayListOffset != offset)
    {
        _displayListOffset = offset;
        GetRoot()->RootDisplayListOffsetChanged(GetSelf());
    }
}

///
/// @brief
///
auto PlatformViewLayerWin::Initialize() -> void
{
    NotifyRootChanged();
}

///
/// @brief
///
auto PlatformViewLayerWin::GetSize() const -> Size<Dp>
{
    return _size;
}

///
/// @brief
///
auto PlatformViewLayerWin::GetOffset() const -> Offset<Dp>
{
    return _offset;
}

///
/// @brief 
///
auto PlatformViewLayerWin::GetClipMode() const -> ViewClipMode
{
    return _clipMode;
}

///
/// @brief
///
auto PlatformViewLayerWin::GetOpacity() const -> Float64
{
    return _opacity;
}

///
/// @brief
///
auto PlatformViewLayerWin::GetDisplayList() const -> Shared<Graphics::DisplayList>
{
    return _displayList;
}

///
/// @brief
///
auto PlatformViewLayerWin::GetDisplayListOffset() const -> Offset<Dp>
{
    return _displayListOffset;
}

///
/// @brief
///
auto PlatformViewLayerWin::GetRenderFlags() const -> PlatformViewLayerRenderFlags
{
    return _renderFlags;
}

///
/// @brief Get display scale.
///
auto PlatformViewLayerWin::GetDisplayScale() const -> DisplayScale
{
    return InternalGetDisplayScale();
}

///
/// @brief Get backing scale.
///
auto PlatformViewLayerWin::GetBackingScale() const -> BackingScale
{
    return 1.0;
}

///
/// @brief
///
auto PlatformViewLayerWin::GetParent() -> Shared<PlatformViewLayerWin>
{
    return _parent.Lock();
}

///
/// @brief
///
auto PlatformViewLayerWin::GetParent() const -> Shared<PlatformViewLayerWin const>
{
    return _parent.Lock();
}

///
/// @brief 
///
auto PlatformViewLayerWin::IsRoot() const -> Bool
{
    return _parent.IsExpired();
}

///
/// @brief
///
auto PlatformViewLayerWin::GetRoot() -> Shared<PlatformViewLayerWin>
{
    if (auto parent = GetParent())
    {
        return parent->GetRoot();
    }
    return GetSelf();
}

///
/// @brief 
///
auto PlatformViewLayerWin::GetRoot() const -> Shared<PlatformViewLayerWin const>
{
    if (auto parent = GetParent())
    {
        return parent->GetRoot();
    }
    return GetSelf();
}

///
/// @brief 
///
auto PlatformViewLayerWin::ShouldRasterize() const -> Bool
{
    auto const renderFlags = GetRenderFlags();
    auto const shouldRasterize = (renderFlags & PlatformViewLayerRenderFlags::Rasterize) == PlatformViewLayerRenderFlags::Rasterize;
    return IsRoot() || shouldRasterize;
}

///
/// @brief
///
auto PlatformViewLayerWin::NotifyRootChanged() -> void
{
    if (IsRoot())
    {
        NotifyRootWindowHandleChanged(InternalGetWindowHandle());
        NotifyRootDisplayScaleChanged(InternalGetDisplayScale());
    }
}

///
/// @brief
///
auto PlatformViewLayerWin::NotifyDisplayScaleChanged() -> void
{
    if (IsRoot())
    {
        NotifyRootDisplayScaleChanged(InternalGetDisplayScale());
    }
}

///
/// @brief 
///
auto PlatformViewLayerWin::GetCompositionDevice() const -> Shared<PlatformDCompositionDeviceWin>
{
    return _dcompDevice;
}

///
/// @brief 
///
/// @param rootHwnd 
///
auto PlatformViewLayerWin::NotifyRootWindowHandleChanged(HWND const rootHwnd) -> void
{
    if (_rootHwnd != rootHwnd)
    {
        _rootHwnd = rootHwnd;

        if (_rootHwnd)
        {
            _hwnd = CreateWindowHandle(rootHwnd);

            if (_hwnd)
            {
                ::SetParent(_hwnd, _rootHwnd);
                ::SetWindowPos(_hwnd, GetBelowWindowHandle(), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOOWNERZORDER);
            }
        }

        for (auto const& child : _children)
        {
            child->NotifyRootWindowHandleChanged(rootHwnd);
        }

        if (!_rootHwnd)
        {
            if (_hwnd)
            {
                DestroyWindowHandle(_hwnd);
                _hwnd = NULL;
            }
        }
    }
}

///
/// @brief
///
auto PlatformViewLayerWin::NotifyRootDisplayScaleChanged(DisplayScale const rootDisplayScale) -> void
{
    if (_rootDisplayScale != rootDisplayScale)
    {
        _rootDisplayScale = rootDisplayScale;

        _displayScale = rootDisplayScale;

        for (auto const& child : _children)
        {
            child->NotifyRootDisplayScaleChanged(rootDisplayScale);
        }
    }
}

///
/// @brief Get window handle below this layer.
///
auto PlatformViewLayerWin::GetBelowWindowHandle() const -> HWND
{
    if (!_rootHwnd || !_hwnd)
    {
        return NULL;
    }

    if (auto parent = GetParent())
    {
        return parent->GetBelowWindowHandleCore(GetSelf());
    }
    return NULL;
}

///
/// @brief 
///
/// @param layer 
///
auto PlatformViewLayerWin::GetBelowWindowHandleCore(Shared<PlatformViewLayerWin const> layer) const -> HWND
{
    auto it = _children.rbegin();

    if (layer != GetParent())
    {
        for (; it != _children.rend(); ++it)
        {
            if (*it == layer)
            {
                it = std::next(it);
                break;
            }
        }
    }

    for (; it != _children.rend(); ++it)
    {
        if (auto hwnd = (*it)->GetBelowWindowHandleCore(GetSelf()))
        {
            return hwnd;
        }
    }

    if (auto hwnd = InternalGetWindowHandle())
    {
        return hwnd;
    }

    if (auto parent = GetParent())
    {
        if (parent != layer)
        {
            return parent->GetBelowWindowHandleCore(GetSelf());
        }
    }
    return NULL;
}

///
/// @brief Make window handle of this view.
///
/// @param parent Window handle of parent window.
///
/// @return Handle of newly created window.
///
auto PlatformViewLayerWin::CreateWindowHandle(HWND parent) -> HWND
{
    (void)parent;
    return NULL;
}

///
/// @brief Destroy window handle of this view.
///
/// @param handle Window handle previously returned by CreateWindowHandle().
///
auto PlatformViewLayerWin::DestroyWindowHandle(HWND handle) -> void
{
    (void)handle;
}

///
/// @brief Create visual node for this layer.
///
/// @return Newly created visual node. Should not be null.
///
auto PlatformViewLayerWin::CreateVisual() -> Microsoft::WRL::ComPtr<IDCompositionVisual3>
{
    return nullptr;
}

///
/// @brief Destroy previously created visual.
///
/// @param[in] visual Visual object created by CreateVisual().
///
auto PlatformViewLayerWin::DestroyVisual(Microsoft::WRL::ComPtr<IDCompositionVisual3> visual) -> void
{
    (void)visual;
}

///
/// @brief Get window handle of root view.
///
/// @note For internal use (PlatformRootViewWin).
///
auto PlatformViewLayerWin::RootGetWindowHandle() const -> HWND
{
    return _hwnd;
}

///
/// @brief
///
auto PlatformViewLayerWin::RootGetDisplayScale() const -> DisplayScale
{
    return _displayScale;
}

///
/// @brief 
///
/// @param layer 
///
auto PlatformViewLayerWin::RootOffsetChanged(Shared<PlatformViewLayerWin> const& layer) -> void
{
    (void)layer;
}

///
/// @brief 
///
/// @param layer 
///
auto PlatformViewLayerWin::RootSizeChanged(Shared<PlatformViewLayerWin> const& layer) -> void
{
    (void)layer;
}

///
/// @brief 
///
/// @param layer 
///
auto PlatformViewLayerWin::RootClipModeChanged(Shared<PlatformViewLayerWin> const& layer) -> void
{
    (void)layer;
}

///
/// @brief 
///
/// @param layer 
///
auto PlatformViewLayerWin::RootOpacityChanged(Shared<PlatformViewLayerWin> const& layer) -> void
{
    (void)layer;
}

///
/// @brief 
///
/// @param layer 
///
auto PlatformViewLayerWin::RootRenderFlagsChanged(Shared<PlatformViewLayerWin> const& layer) -> void
{
    (void)layer;
}

///
/// @brief 
///
/// @param layer 
///
auto PlatformViewLayerWin::RootDisplayListChanged(Shared<PlatformViewLayerWin> const& layer) -> void
{
    (void)layer;
}

///
/// @brief 
///
/// @param layer 
///
auto PlatformViewLayerWin::RootDisplayListOffsetChanged(Shared<PlatformViewLayerWin> const& layer) -> void
{
    (void)layer;
}

///
/// @brief
///
/// @param child
///
auto PlatformViewLayerWin::RootChildAdded(Shared<PlatformViewLayerWin> const& child) -> void
{
    (void)child;
}

///
/// @brief
///
/// @param parent
///
auto PlatformViewLayerWin::RootChildRemoved(Shared<PlatformViewLayerWin> const& parent) -> void
{
    (void)parent;
}

///
/// @brief
///
auto PlatformViewLayerWin::InternalGetWindowHandle() const -> HWND
{
    if (IsRoot())
    {
        return RootGetWindowHandle();
    }
    return _hwnd;
}

///
/// @brief
///
auto PlatformViewLayerWin::InternalGetDisplayScale() const -> DisplayScale
{
    if (IsRoot())
    {
        return RootGetDisplayScale();
    }
    return _displayScale;
}

///
/// @brief 
///
auto PlatformViewLayerWin::InternalGetParentWindowHandle() const -> HWND
{
    return InternalGetWindowHandle();
}

///
/// @brief
///
auto PlatformViewLayerWin::InternalAttach() -> void
{
    auto root = IsRoot() ? GetSelf() : GetRoot();
    NotifyRootWindowHandleChanged(root->InternalGetWindowHandle());
    NotifyRootDisplayScaleChanged(root->InternalGetDisplayScale());
}

///
/// @brief
///
auto PlatformViewLayerWin::InternalDetach() -> void
{
    NotifyRootWindowHandleChanged(NULL);
    NotifyRootDisplayScaleChanged(1.0);
}
}
}

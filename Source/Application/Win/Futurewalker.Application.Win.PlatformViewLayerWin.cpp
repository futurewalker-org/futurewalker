// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformViewLayerWin.hpp"
#include "Futurewalker.Application.Win.PlatformDCompositionDeviceWin.hpp"

#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
PlatformViewLayerWin::PlatformViewLayerWin(PassKey<PlatformViewLayer> key, Shared<PlatformDCompositionDeviceWin> const& dcompDevice)
  : PlatformViewLayer(key)
  , _dcompDevice {dcompDevice}
{
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
    }
}

///
/// @brief 
///
auto PlatformViewLayerWin::GetChildren() -> PlatformViewLayerArray
{
    return PlatformViewLayerArray(_children.begin(), _children.end());
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
        InternalUpdateOffset();
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
        InternalUpdateClip();
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
        InternalUpdateClip();
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
        InternalUpdateOpacity();
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
auto PlatformViewLayerWin::IsRoot() const -> Bool
{
    return _parent.IsExpired();
}

///
/// @brief
///
auto PlatformViewLayerWin::NotifyRootChanged() -> void
{
    if (IsRoot())
    {
        NotifyRootWindowHandleChanged(InternalGetWindowHandle());
        NotifyRootVisualChanged(InternalGetVisual());
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
auto PlatformViewLayerWin::GetDCompositionDevice() -> Shared<PlatformDCompositionDeviceWin>
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
/// @param rootVisual
///
auto PlatformViewLayerWin::NotifyRootVisualChanged(Microsoft::WRL::ComPtr<IDCompositionVisual3> rootVisual) -> void
{
    if (_rootVisual != rootVisual)
    {
        _rootVisual = rootVisual;

        if (_rootVisual)
        {
            if (auto parent = GetParent())
            {
                if (auto parentVisual = parent->InternalGetVisual())
                {
                    _visual = CreateVisual();
                    parentVisual->AddVisual(_visual.Get(), TRUE, parent->GetPrevChildVisual(GetSelf()).Get());
                    InternalUpdateOffset();
                    InternalUpdateOpacity();
                    InternalUpdateClip();
                }
            }
        }

        for (auto const& child : _children)
        {
            child->NotifyRootVisualChanged(rootVisual);
        }

        if (!rootVisual)
        {
            if (auto parent = GetParent())
            {
                if (_visual)
                {
                    if (auto parentVisual = parent->InternalGetVisual())
                    {
                        parentVisual->RemoveVisual(_visual.Get());
                        _visual.Reset();
                    }
                }
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

        InternalUpdateOffset();
        InternalUpdateOpacity();
        InternalUpdateClip();

        OnDisplayScaleChange();

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
/// @brief Get visual of previous child layer.
///
auto PlatformViewLayerWin::GetPrevChildVisual(Shared<PlatformViewLayer> child) -> Microsoft::WRL::ComPtr<IDCompositionVisual3>
{
    for (auto it = _children.rbegin(); it != _children.rend(); ++it)
    {
        if (*it == child)
        {
            auto const next = std::next(it);
            if (next != _children.rend())
            {
                if (auto const& layer = *next)
                {
                    return layer->InternalGetVisual();
                }
            }
            return {};
        }
    }
    return {};
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
/// @brief
///
auto PlatformViewLayerWin::OnDisplayScaleChange() -> void
{
}

///
/// @brief Get visual of root view.
///
/// @note For internal use (PlatformRootViewWin).
///
auto PlatformViewLayerWin::RootGetVisual() const -> Microsoft::WRL::ComPtr<IDCompositionVisual3>
{
    return _visual;
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
auto PlatformViewLayerWin::InternalGetVisual() const -> Microsoft::WRL::ComPtr<IDCompositionVisual3>
{
    if (IsRoot())
    {
        return RootGetVisual();
    }
    return _visual;
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
auto PlatformViewLayerWin::InternalGetParentVisual() const -> Microsoft::WRL::ComPtr<IDCompositionVisual3>
{
    return InternalGetVisual();
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
    NotifyRootVisualChanged(root->InternalGetVisual());
    NotifyRootDisplayScaleChanged(root->InternalGetDisplayScale());
}

///
/// @brief
///
auto PlatformViewLayerWin::InternalDetach() -> void
{
    NotifyRootWindowHandleChanged(NULL);
    NotifyRootVisualChanged(nullptr);
    NotifyRootDisplayScaleChanged(1.0);
}

///
/// @brief 
///
auto PlatformViewLayerWin::InternalUpdateClip() -> void
{
    if (auto const visual = InternalGetVisual())
    {
        if (_clipMode == ViewClipMode::Bounds)
        {
            if (auto const device = GetDCompositionDevice())
            {
                auto const clip = device->CreateRectangleClip();
                auto const width = UnitFunction::ConvertDpToVp(_size.GetWidth(), GetDisplayScale());
                auto const height = UnitFunction::ConvertDpToVp(_size.GetHeight(), GetDisplayScale());
                clip->SetLeft(0.f);
                clip->SetTop(0.f);
                clip->SetRight(static_cast<float>(width));
                clip->SetBottom(static_cast<float>(height));
                visual->SetClip(clip.Get());
            }
        }
        else
        {
            visual->SetClip(nullptr);
        }
    }
}

///
/// @brief 
///
auto PlatformViewLayerWin::InternalUpdateOffset() -> void
{
    if (auto const visual = InternalGetVisual())
    {
        auto const x = UnitFunction::ConvertDpToVp(_offset.GetDeltaX(), GetDisplayScale());
        auto const y = UnitFunction::ConvertDpToVp(_offset.GetDeltaY(), GetDisplayScale());
        visual->SetOffsetX(static_cast<float>(x));
        visual->SetOffsetY(static_cast<float>(y));
    }
}

///
/// @brief 
///
auto PlatformViewLayerWin::InternalUpdateOpacity() -> void
{
    if (auto const visual = InternalGetVisual())
    {
        Microsoft::WRL::ComPtr<IDCompositionVisual3> visual3;
        if (SUCCEEDED(visual.As(&visual3)) && visual3)
        {
            visual3->SetOpacity(static_cast<float>(_opacity));
        }
    }
}
}
}

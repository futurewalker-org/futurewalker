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
auto PlatformViewLayerWin::Initialize() -> void
{
}

auto PlatformViewLayerWin::OnAttach() -> void
{
    if (auto const root = GetRoot().TryAs<PlatformViewLayerWin>())
    {
        NotifyRootWindowHandleChanged(root->InternalGetWindowHandle());
    }
}

auto PlatformViewLayerWin::OnDetach() -> void
{
    NotifyRootWindowHandleChanged(NULL);
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
/// @brief Get window handle below this layer.
///
auto PlatformViewLayerWin::GetBelowWindowHandle() const -> HWND
{
    if (!_rootHwnd || !_hwnd)
    {
        return NULL;
    }

    if (auto const parent = GetParent().TryAs<PlatformViewLayerWin const>())
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

    if (auto const parent = GetParent().TryAs<PlatformViewLayerWin const>())
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
auto PlatformViewLayerWin::InternalGetWindowHandle() const -> HWND
{
    if (IsRoot())
    {
        return RootGetWindowHandle();
    }
    return _hwnd;
}
}
}

// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.Win.PlatformRootViewLayerWin.hpp"
#include "Futurewalker.Application.Win.PlatformDCompositionDeviceWin.hpp"

#include "Futurewalker.Base.Debug.hpp"

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
    _target = MakeTarget(_hwnd);
    _visual = MakeVisual();

    if (_target && _visual)
    {
        _target->SetRoot(_visual.Get());
    }
}

///
/// @brief
///
auto PlatformRootViewLayerWin::GetControl() -> Shared<PlatformViewLayerControl>
{
    return {};
}

///
/// @brief
///
auto PlatformRootViewLayerWin::CreateVisual() -> Microsoft::WRL::ComPtr<IDCompositionVisual3>
{
    return {};
}

///
/// @brief
///
auto PlatformRootViewLayerWin::DestroyVisual(Microsoft::WRL::ComPtr<IDCompositionVisual3> visual) -> void
{
    (void)visual;
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
auto PlatformRootViewLayerWin::RootGetVisual() const -> Microsoft::WRL::ComPtr<IDCompositionVisual3>
{
    return _visual;
}

///
/// @brief 
///
/// @param hwnd 
///
auto PlatformRootViewLayerWin::MakeTarget(HWND hwnd) -> Microsoft::WRL::ComPtr<IDCompositionTarget>
{
    if (const auto dcompDevice = GetDCompositionDevice())
    {
        return dcompDevice->CreateTargetForHwnd(hwnd);
    }
    return {};
}

///
/// @brief
///
auto PlatformRootViewLayerWin::MakeVisual() -> Microsoft::WRL::ComPtr<IDCompositionVisual3>
{
    if (const auto dcompDevice = GetDCompositionDevice())
    {
        return dcompDevice->CreateVisual();
    }
    return {};
}
}

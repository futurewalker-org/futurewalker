// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.Win.PlatformDCompositionDeviceWin.hpp"

#include "Futurewalker.Graphics.Win.PlatformD3D11DeviceWin.hpp"

#include <dxgi.h>

namespace FW_DETAIL_NS
{
///
/// @brief Constructor.
///
/// @param d3d11Device
///
PlatformDCompositionDeviceWin::PlatformDCompositionDeviceWin(Shared<PlatformD3D11DeviceWin> d3d11Device)
  : _d3d11Device {d3d11Device}
{
    if (_d3d11Device)
    {
        _dcompDevice = CreateDCompositionDevice(_d3d11Device->GetDevice());
    }

    if (!_d3d11Device || !_dcompDevice)
    {
        throw Exception(ErrorCode::Failure, "Failed to create IDCompositionDesktopDevice");
    }
}

///
/// @brief Commit changes.
///
auto PlatformDCompositionDeviceWin::Commit() -> void
{
    if (_dcompDevice)
    {
        _dcompDevice->Commit();
    }
}

///
/// @brief Create visual.
///
auto PlatformDCompositionDeviceWin::CreateVisual() -> Microsoft::WRL::ComPtr<IDCompositionVisual3>
{
    if (_dcompDevice)
    {
        auto visual = Microsoft::WRL::ComPtr<IDCompositionVisual2>();
        auto hr = _dcompDevice->CreateVisual(visual.GetAddressOf());
        if (SUCCEEDED(hr) && visual)
        {
            Microsoft::WRL::ComPtr<IDCompositionVisual3> visual3;
            hr = visual.As(&visual3);
            if (SUCCEEDED(hr) && visual3)
            {
                return visual3;
            }
        }
    }
    return {};
}

///
/// @brief Create target for HWND.
///
/// @param hwnd 
///
auto PlatformDCompositionDeviceWin::CreateTargetForHwnd(HWND hwnd) -> Microsoft::WRL::ComPtr<IDCompositionTarget>
{
    if (_dcompDevice)
    {
        auto target = Microsoft::WRL::ComPtr<IDCompositionTarget>();
        const auto hr = _dcompDevice->CreateTargetForHwnd(hwnd, FALSE, target.GetAddressOf());
        if (SUCCEEDED(hr) && target)
        {
            return target;
        }
    }
    return {};
}

///
/// @brief Create rectangle clip.
///
auto PlatformDCompositionDeviceWin::CreateRectangleClip() -> Microsoft::WRL::ComPtr<IDCompositionRectangleClip>
{
    if (_dcompDevice)
    {
        auto clip = Microsoft::WRL::ComPtr<IDCompositionRectangleClip>();
        const auto hr = _dcompDevice->CreateRectangleClip(clip.GetAddressOf());
        if (SUCCEEDED(hr))
        {
            return clip;
        }
    }
    return {};
}

///
/// @brief Get composition device.
///
auto PlatformDCompositionDeviceWin::GetDevice() -> Microsoft::WRL::ComPtr<IDCompositionDesktopDevice>
{
    return _dcompDevice;
}

///
/// @brief Create IDCompositionDevice instance.
///
/// @param d3d11Device
///
auto PlatformDCompositionDeviceWin::CreateDCompositionDevice(Microsoft::WRL::ComPtr<ID3D11Device> d3d11Device) -> Microsoft::WRL::ComPtr<IDCompositionDesktopDevice>
{
    Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
    HRESULT hr = d3d11Device.As(&dxgiDevice);
    if (FAILED(hr) || !dxgiDevice)
    {
        return {};
    }

    Microsoft::WRL::ComPtr<IDCompositionDesktopDevice> dcompDevice;
    hr = ::DCompositionCreateDevice3(dxgiDevice.Get(), IID_PPV_ARGS(dcompDevice.GetAddressOf()));
    if (FAILED(hr) || !dcompDevice)
    {
        return {};
    }
    return dcompDevice;
}

///
/// @brief
///
auto Locator::Resolver<PlatformDCompositionDeviceWin>::Resolve() -> Shared<PlatformDCompositionDeviceWin>
{
    auto d3d11Device = Locator::Resolve<PlatformD3D11DeviceWin>();
    return Shared<PlatformDCompositionDeviceWin>::Make(d3d11Device);
}
}

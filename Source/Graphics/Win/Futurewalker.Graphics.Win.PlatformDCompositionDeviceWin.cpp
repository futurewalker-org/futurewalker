// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWin.hpp"

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
        _presentationFactory = CreatePresentationFactory(_d3d11Device->GetDevice()); 
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
/// @brief Queries whether presentation API is supported.
///
auto PlatformDCompositionDeviceWin::IsPresentationSupported() const -> Bool
{
    if (_presentationFactory && _presentationFactory->IsPresentationSupported())
    {
        return true;
    }
    return false;
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

auto PlatformDCompositionDeviceWin::CreateSurfaceFromHandle(HANDLE handle) -> Microsoft::WRL::ComPtr<IUnknown>
{
    if (_dcompDevice)
    {
        auto surface = Microsoft::WRL::ComPtr<IUnknown>();
        auto hr = _dcompDevice->CreateSurfaceFromHandle(handle, &surface);
        if (SUCCEEDED(hr) && surface)
        {
            return surface;
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
/// @brief
///
auto PlatformDCompositionDeviceWin::CreatePresentationManager() -> Microsoft::WRL::ComPtr<IPresentationManager>
{
    if (IsPresentationSupported())
    {
        Microsoft::WRL::ComPtr<IPresentationManager> presentationManager;
        auto const hr = _presentationFactory->CreatePresentationManager(&presentationManager);
        if (SUCCEEDED(hr) && presentationManager)
        {
            return presentationManager;
        }
    }
    return {};
}

///
/// @brief 
///
auto PlatformDCompositionDeviceWin::CreateSurfaceHandle() -> Microsoft::WRL::Wrappers::HandleT<Microsoft::WRL::Wrappers::HandleTraits::HANDLENullTraits>
{
    auto handle = HANDLE();
    auto const hr = ::DCompositionCreateSurfaceHandle(COMPOSITIONOBJECT_ALL_ACCESS, nullptr, &handle);
    if (SUCCEEDED(hr))
    {
        return Microsoft::WRL::Wrappers::HandleT<Microsoft::WRL::Wrappers::HandleTraits::HANDLENullTraits>(handle);
    }
    return Microsoft::WRL::Wrappers::HandleT<Microsoft::WRL::Wrappers::HandleTraits::HANDLENullTraits>();
}

///
/// @brief Open resource from shared handle.
///
/// @param handle
///
auto PlatformDCompositionDeviceWin::CreateSharedResourceFromHandle(HANDLE handle) -> Microsoft::WRL::ComPtr<ID3D11Resource>
{
    if (_d3d11Device)
    {
        if (auto const device = _d3d11Device->GetDevice())
        {
            Microsoft::WRL::ComPtr<ID3D11Resource> sharedResource;
            auto const hr = device->OpenSharedResource1(handle, IID_PPV_ARGS(&sharedResource));
            if (SUCCEEDED(hr))
            {
                return sharedResource;
            }
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
auto PlatformDCompositionDeviceWin::CreateDCompositionDevice(Microsoft::WRL::ComPtr<ID3D11Device> const& d3d11Device) -> Microsoft::WRL::ComPtr<IDCompositionDesktopDevice>
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
/// @brief Create presentation factory.
///
auto PlatformDCompositionDeviceWin::CreatePresentationFactory(Microsoft::WRL::ComPtr<ID3D11Device> const& d3d11Device) -> Microsoft::WRL::ComPtr<IPresentationFactory>
{
    Microsoft::WRL::ComPtr<IPresentationFactory> presentationFactory;
    auto const hr = ::CreatePresentationFactory(d3d11Device.Get(), IID_PPV_ARGS(&presentationFactory));
    if (FAILED(hr) || !presentationFactory || !presentationFactory->IsPresentationSupported())
    {
        return {};
    }
    return presentationFactory;
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

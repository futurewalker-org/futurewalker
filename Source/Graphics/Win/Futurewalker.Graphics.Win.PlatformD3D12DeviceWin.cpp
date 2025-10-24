// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.Win.PlatformD3D12DeviceWin.hpp"
#include "Futurewalker.Graphics.Win.PlatformGraphicsDeviceObjectWin.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Core.BuildConfig.hpp"

#include <dxgi1_6.h>

namespace FW_DETAIL_NS
{
///
/// @brief Constructor.
///
/// @param preference Preference of device creation.
///
PlatformD3D12DeviceWin::PlatformD3D12DeviceWin(PlatformD3D12DevicePreference const preference)
  : _preference {preference}
{
    _adapter = ChooseAdapter(preference);
    if (!_adapter)
    {
        throw Exception(ErrorCode::Failure, "PlatformD3D12DeviceWin: Failed to find adapter");
    }

    _device = CreateDevice(_adapter);
    if (!_device)
    {
        throw Exception(ErrorCode::Failure, "PlatformD3D12DeviceWin: Failed to create device");
    }
}

///
/// @brief Get ID3D12Device instance.
///
auto PlatformD3D12DeviceWin::GetDevice() -> Microsoft::WRL::ComPtr<ID3D12Device>
{
    return _device;
}

///
/// @brief Get IDXGIAdapter1 instance.
///
auto PlatformD3D12DeviceWin::GetAdapter() -> Microsoft::WRL::ComPtr<IDXGIAdapter1>
{
    return _adapter;
}

///
/// @brief NotifyHandle device lost event.
///
auto PlatformD3D12DeviceWin::NotifyDeviceLost() -> void
{
    HandleDeviceLost();
}

///
/// @brief
///
/// @param deviceObject
///
auto PlatformD3D12DeviceWin::AddDeviceObject(Shared<PlatformGraphicsDeviceObjectWin> const& deviceObject) -> void
{
    _deviceObjects.push_back(deviceObject);
}

///
/// @brief Choose preferred adapter to be used.
///
/// @param preference Preference of device to use
///
auto PlatformD3D12DeviceWin::ChooseAdapter(PlatformD3D12DevicePreference const preference) -> Microsoft::WRL::ComPtr<IDXGIAdapter1>
{
    auto dxgiFactoryFlags = UINT(0);
    if constexpr (BuildConfig::IsDebug())
    {
        auto debug = Microsoft::WRL::ComPtr<ID3D12Debug>();
        auto const hr = ::D3D12GetDebugInterface(IID_PPV_ARGS(debug.GetAddressOf()));
        if (SUCCEEDED(hr) && debug)
        {
            debug->EnableDebugLayer();
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
            FW_DEBUG_LOG_INFO("PlatformD3D12DeviceWin: Debug layer enabled.");
        }
        else
        {
            FW_DEBUG_LOG_WARNING("PlatformD3D12DeviceWin: Debug layer unavailable.");
        }
    }

    auto dxgiFactory = Microsoft::WRL::ComPtr<IDXGIFactory2>();
    auto hr = ::CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(dxgiFactory.GetAddressOf()));
    if (FAILED(hr))
    {
        FW_DEBUG_LOG_ERROR("PlatformD3D12DeviceWin: CreateDXGIFactory2 failed.");
        return {};
    }

    auto dxgiFactory6 = Microsoft::WRL::ComPtr<IDXGIFactory6>();
    hr = dxgiFactory->QueryInterface(IID_PPV_ARGS(dxgiFactory6.GetAddressOf()));
    if (SUCCEEDED(hr) && dxgiFactory6)
    {
        auto gpuPreference = DXGI_GPU_PREFERENCE_UNSPECIFIED;
        if (preference == PlatformD3D12DevicePreference::HighPerformance)
        {
            gpuPreference = DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE;
        }
        else if (preference == PlatformD3D12DevicePreference::MinimumPower)
        {
            gpuPreference = DXGI_GPU_PREFERENCE_MINIMUM_POWER;
        }

        auto adapter = Microsoft::WRL::ComPtr<IDXGIAdapter1>();
        auto adapterIndex = UINT(0);
        while (true)
        {
            hr = dxgiFactory6->EnumAdapterByGpuPreference(adapterIndex, gpuPreference, IID_PPV_ARGS(adapter.ReleaseAndGetAddressOf()));
            if (SUCCEEDED(hr) && adapter)
            {
                hr = ::D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr);
                if (SUCCEEDED(hr))
                {
                    return adapter;
                }
            }
            else
            {
                break;
            }
            ++adapterIndex;
        }
    }
    else
    {
        FW_DEBUG_LOG_WARNING("PlatformD3D12DeviceWin: CreateDXGIFactory6 unavailable. GPU preference will be ignored.");

        auto adapter = Microsoft::WRL::ComPtr<IDXGIAdapter1>();
        auto adapterIndex = UINT(0);
        while (true)
        {
            hr = dxgiFactory->EnumAdapters1(adapterIndex, adapter.ReleaseAndGetAddressOf());
            if (SUCCEEDED(hr) && adapter)
            {
                hr = ::D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr);
                if (SUCCEEDED(hr))
                {
                    return adapter;
                }
            }
            else
            {
                break;
            }
            ++adapterIndex;
        }
    }
    return {};
}

///
/// @brief Create device.
///
auto PlatformD3D12DeviceWin::CreateDevice(Microsoft::WRL::ComPtr<IDXGIAdapter1> const& adapter) -> Microsoft::WRL::ComPtr<ID3D12Device>
{
    if (adapter)
    {
        Microsoft::WRL::ComPtr<ID3D12Device> device;
        auto const hr = ::D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(device.GetAddressOf()));
        if (SUCCEEDED(hr) && device)
        {
            return device;
        }
    }
    return {};
}

///
/// @brief 
///
auto PlatformD3D12DeviceWin::ClearResources() -> void
{
    _device.Reset();
    _adapter.Reset();
}

///
/// @brief
///
auto PlatformD3D12DeviceWin::BuildResources() -> void
{
    _adapter = ChooseAdapter(_preference);
    _device = CreateDevice(_adapter);
}

///
/// @brief Handle device lost event.
///
auto PlatformD3D12DeviceWin::HandleDeviceLost() -> void
{
    ClearResources();
    BuildResources();

    for (auto& weakDeviceObject : _deviceObjects)
    {
        if (auto deviceObject = weakDeviceObject.Lock())
        {
            deviceObject->HandleDeviceLost();
        }
    }
}

///
/// @brief
///
auto Locator::Resolver<PlatformD3D12DeviceWin>::Resolve(PlatformD3D12DevicePreference const preference) -> Shared<PlatformD3D12DeviceWin>
{
    return Shared<PlatformD3D12DeviceWin>::Make(preference);
}
}

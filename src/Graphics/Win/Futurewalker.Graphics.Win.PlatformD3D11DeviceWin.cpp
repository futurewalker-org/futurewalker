// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.Win.PlatformD3D11DeviceWin.hpp"
#include "Futurewalker.Graphics.Win.PlatformGraphicsDeviceObjectWin.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Core.Exception.hpp"
#include "Futurewalker.Core.BuildConfig.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Constructor.
///
PlatformD3D11DeviceWin::PlatformD3D11DeviceWin()
{
    _device = CreateDevice();
}

///
/// @brief Get ID3D11Device instance.
///
auto PlatformD3D11DeviceWin::GetDevice() -> Microsoft::WRL::ComPtr<ID3D11Device1>
{
    return _device;
}

///
/// @brief
///
auto PlatformD3D11DeviceWin::NotifyDeviceLost() -> void
{
    HandleDeviceLost();
}

///
/// @brief 
///
/// @param deviceObject 
///
auto PlatformD3D11DeviceWin::AddDeviceObject(Shared<PlatformGraphicsDeviceObjectWin> const& deviceObject) -> void
{
    _deviceObjects.push_back(deviceObject);
}

///
/// @brief Create device.
///
auto PlatformD3D11DeviceWin::CreateDevice() -> Microsoft::WRL::ComPtr<ID3D11Device1>
{
    if (auto device = CreateDeviceForType(D3D_DRIVER_TYPE_HARDWARE))
    {
        return device;
    }

    if (auto device = CreateDeviceForType(D3D_DRIVER_TYPE_WARP))
    {
        FW_DEBUG_LOG_WARNING("PlatformD3D11DeviceWin: Hardware device unavailable. Selecting WARP device");
        return device;
    }

    FW_DEBUG_LOG_ERROR("PlatformD3D11DeviceWin: Device unavailable");

    throw Exception(ErrorCode::Failure, "Failed to create D3D11 Device");
}

///
/// @brief Create device for driver type.
///
/// @param type Driver type
///
auto PlatformD3D11DeviceWin::CreateDeviceForType(D3D_DRIVER_TYPE const type) -> Microsoft::WRL::ComPtr<ID3D11Device1>
{
    auto flags = UINT(D3D11_CREATE_DEVICE_BGRA_SUPPORT);
    if constexpr (BuildConfig::IsDebug())
    {
        flags |= D3D11_CREATE_DEVICE_DEBUG;
    }

    flags |=  D3D11_CREATE_DEVICE_SINGLETHREADED;
    flags |=  D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS;

    auto constexpr featureLevels = std::array {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
      D3D_FEATURE_LEVEL_9_3,
      D3D_FEATURE_LEVEL_9_2,
      D3D_FEATURE_LEVEL_9_1,
    };

    auto device = Microsoft::WRL::ComPtr<ID3D11Device>();
    auto hr = ::D3D11CreateDevice(nullptr, type, NULL, flags, featureLevels.data(), static_cast<UINT>(featureLevels.size()), D3D11_SDK_VERSION, device.GetAddressOf(), nullptr, nullptr);
    if (SUCCEEDED(hr) && device)
    {
        auto device1 = Microsoft::WRL::ComPtr<ID3D11Device1>();
        hr = device.As(&device1);
        if (SUCCEEDED(hr) && device1)
        {
            return device1;
        }
    }
    return nullptr;
}

///
/// @brief
///
auto PlatformD3D11DeviceWin::ClearResources() -> void
{
    _device.Reset();
}

///
/// @brief
///
auto PlatformD3D11DeviceWin::BuildResources() -> void
{
    _device = CreateDevice();
}

///
/// @brief
///
auto PlatformD3D11DeviceWin::HandleDeviceLost() -> void
{
    ClearResources();
    BuildResources();

    for (auto const& weakDeviceObject : _deviceObjects)
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
auto Locator::Resolver<PlatformD3D11DeviceWin>::Resolve() -> Shared<PlatformD3D11DeviceWin>
{
    return Shared<PlatformD3D11DeviceWin>::Make();
}
}

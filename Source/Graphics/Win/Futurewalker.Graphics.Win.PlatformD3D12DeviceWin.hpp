// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Graphics.Win.PlatformD3D12DeviceWinType.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.BuildConfig.hpp"

#include <wrl/client.h>
#include <d3d12.h>
#include <dxgi.h>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Direct3D 12 device.
///
class PlatformD3D12DeviceWin : NonCopyable
{
public:
    PlatformD3D12DeviceWin(PlatformD3D12DevicePreference const preference);

    auto GetDevice() -> Microsoft::WRL::ComPtr<ID3D12Device>;
    auto GetAdapter() -> Microsoft::WRL::ComPtr<IDXGIAdapter1>;

    auto NotifyDeviceLost() -> void;

private:
    auto ChooseAdapter(PlatformD3D12DevicePreference const preference) -> Microsoft::WRL::ComPtr<IDXGIAdapter1>;
    auto CreateDevice(Microsoft::WRL::ComPtr<IDXGIAdapter1> const& adapter) -> Microsoft::WRL::ComPtr<ID3D12Device>;

    auto HandleDeviceLost() -> void;

private:
    PlatformD3D12DevicePreference _preference;
    Microsoft::WRL::ComPtr<IDXGIAdapter1> _adapter;
    Microsoft::WRL::ComPtr<ID3D12Device> _device;
};

template <>
struct Locator::Resolver<PlatformD3D12DeviceWin>
{
    using Interface = PlatformD3D12DeviceWin;
    static auto Resolve(PlatformD3D12DevicePreference const preference = PlatformD3D12DevicePreference::Unspecified) -> Shared<PlatformD3D12DeviceWin>;
};
}
}

// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.Win.PlatformD3D11DeviceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformGraphicsDeviceObjectWinType.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.BuildConfig.hpp"

#include <wrl/client.h>
#include <d3d11_1.h>

#include <array>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Direct3D 11 device.
///
class PlatformD3D11DeviceWin : public NonCopyable
{
public:
    PlatformD3D11DeviceWin();

    auto GetDevice() -> Microsoft::WRL::ComPtr<ID3D11Device1>;

    auto NotifyDeviceLost() -> void;
    auto AddDeviceObject(Shared<PlatformGraphicsDeviceObjectWin> const& deviceObject) -> void;

private:
    auto CreateDevice() -> Microsoft::WRL::ComPtr<ID3D11Device1>;
    auto CreateDeviceForType(D3D_DRIVER_TYPE const type) -> Microsoft::WRL::ComPtr<ID3D11Device1>;
    auto ClearResources() -> void;
    auto BuildResources() -> void;
    auto HandleDeviceLost() -> void;

private:
    Microsoft::WRL::ComPtr<ID3D11Device1> _device;
    std::vector<Weak<PlatformGraphicsDeviceObjectWin>> _deviceObjects;
};

template <>
struct Locator::Resolver<PlatformD3D11DeviceWin>
{
    using Interface = PlatformD3D11DeviceWin;
    static auto Resolve() -> Shared<PlatformD3D11DeviceWin>;
};
}
}


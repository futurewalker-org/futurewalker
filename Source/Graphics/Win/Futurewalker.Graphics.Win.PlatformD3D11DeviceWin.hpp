// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Graphics.Win.PlatformD3D11DeviceWinType.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.BuildConfig.hpp"

#include <wrl/client.h>
#include <d3d11.h>

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

    auto GetDevice() -> Microsoft::WRL::ComPtr<ID3D11Device>;

private:
    auto CreateDevice() -> Microsoft::WRL::ComPtr<ID3D11Device>;
    auto CreateDeviceForType(D3D_DRIVER_TYPE const type) -> Microsoft::WRL::ComPtr<ID3D11Device>;

private:
    Microsoft::WRL::ComPtr<ID3D11Device> _device;
};

template <>
struct Locator::Resolver<PlatformD3D11DeviceWin>
{
    using Interface = PlatformD3D11DeviceWin;
    static auto Resolve() -> Shared<PlatformD3D11DeviceWin>;
};
}
}


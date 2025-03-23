// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.Win.PlatformDCompositionDeviceWinType.hpp"

#include "Futurewalker.Graphics.Win.PlatformD3D11DeviceWinType.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"

#include <wrl/client.h>
#include <d3d11.h>
#include <dcomp.h>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Direct composition device.
///
class PlatformDCompositionDeviceWin : public NonCopyable
{
public:
    PlatformDCompositionDeviceWin(Shared<PlatformD3D11DeviceWin> d3d11Device);

    auto Commit() -> void;
    auto CreateVisual() -> Microsoft::WRL::ComPtr<IDCompositionVisual3>;
    auto CreateTargetForHwnd(HWND hwnd) -> Microsoft::WRL::ComPtr<IDCompositionTarget>;
    auto CreateRectangleClip() -> Microsoft::WRL::ComPtr<IDCompositionRectangleClip>;

    auto GetDevice() -> Microsoft::WRL::ComPtr<IDCompositionDesktopDevice>;

private:
    auto CreateDCompositionDevice(Microsoft::WRL::ComPtr<ID3D11Device> d3d11Device) -> Microsoft::WRL::ComPtr<IDCompositionDesktopDevice>;

private:
    Shared<PlatformD3D11DeviceWin> _d3d11Device;
    Microsoft::WRL::ComPtr<IDCompositionDesktopDevice> _dcompDevice;
};

///
/// @brief
///
template <>
struct Locator::Resolver<PlatformDCompositionDeviceWin>
{
    using Interface = PlatformDCompositionDeviceWin;
    static auto Resolve() -> Shared<PlatformDCompositionDeviceWin>;
};
}
}

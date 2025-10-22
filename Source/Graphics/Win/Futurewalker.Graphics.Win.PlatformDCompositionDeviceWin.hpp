// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWinType.hpp"

#include "Futurewalker.Graphics.Win.PlatformD3D11DeviceWinType.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"

#include <wrl/client.h>
#include <wrl/wrappers/corewrappers.h>
#include <d3d11_3.h>
#include <d3d12.h>
#include <dcomp.h>
#include <presentation.h>

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

    auto IsPresentationSupported() const -> Bool;

    auto CreateVisual() -> Microsoft::WRL::ComPtr<IDCompositionVisual3>;
    auto CreateTargetForHwnd(HWND hwnd) -> Microsoft::WRL::ComPtr<IDCompositionTarget>;
    auto CreateSurfaceFromHandle(HANDLE handle) -> Microsoft::WRL::ComPtr<IUnknown>;
    auto CreateRectangleClip() -> Microsoft::WRL::ComPtr<IDCompositionRectangleClip>;
    auto CreatePresentationManager() -> Microsoft::WRL::ComPtr<IPresentationManager>;
    auto CreateSurfaceHandle() -> Microsoft::WRL::Wrappers::HandleT<Microsoft::WRL::Wrappers::HandleTraits::HANDLENullTraits>;
    auto CreateSharedResourceFromHandle(HANDLE handle) -> Microsoft::WRL::ComPtr<ID3D11Resource>;

    auto GetDevice() -> Microsoft::WRL::ComPtr<IDCompositionDesktopDevice>;

private:
    auto CreateDCompositionDevice(Microsoft::WRL::ComPtr<ID3D11Device> const& d3d11Device) -> Microsoft::WRL::ComPtr<IDCompositionDesktopDevice>;
    auto CreatePresentationFactory(Microsoft::WRL::ComPtr<ID3D11Device> const& d3d11Device) -> Microsoft::WRL::ComPtr<IPresentationFactory>;

private:
    Shared<PlatformD3D11DeviceWin> _d3d11Device;
    Microsoft::WRL::ComPtr<IDCompositionDesktopDevice> _dcompDevice;
    Microsoft::WRL::ComPtr<IPresentationFactory> _presentationFactory;
};

template <>
struct Locator::Resolver<PlatformDCompositionDeviceWin>
{
    using Interface = PlatformDCompositionDeviceWin;
    static auto Resolve() -> Shared<PlatformDCompositionDeviceWin>;
};
}
}

// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWinType.hpp"
#include "Futurewalker.Graphics.Win.PlatformGraphicsDeviceObjectWin.hpp"

#include "Futurewalker.Graphics.Win.PlatformD3D11DeviceWinType.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.PassKey.hpp"

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
class PlatformDCompositionDeviceWin : public PlatformGraphicsDeviceObjectWin
{
public:
    static auto Make(Shared<PlatformD3D11DeviceWin> const& d3d11Device) -> Shared<PlatformDCompositionDeviceWin>;

    PlatformDCompositionDeviceWin(PassKey<PlatformDCompositionDeviceWin> ,Shared<PlatformD3D11DeviceWin> d3d11Device);

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

    auto NotifyDeviceLost() -> void override;
    auto HandleDeviceLost() -> void override;
    auto AddDeviceObject(Shared<PlatformGraphicsDeviceObjectWin> const& deviceObject) -> void;

private:
    auto GetSelf() -> Shared<PlatformDCompositionDeviceWin>;
    auto CreateDCompositionDevice(Microsoft::WRL::ComPtr<ID3D11Device> const& d3d11Device) -> Microsoft::WRL::ComPtr<IDCompositionDesktopDevice>;
    auto CreatePresentationFactory(Microsoft::WRL::ComPtr<ID3D11Device> const& d3d11Device) -> Microsoft::WRL::ComPtr<IPresentationFactory>;
    auto ClearResources() -> void;
    auto BuildResources() -> void;

private:
    Weak<PlatformDCompositionDeviceWin> _self;
    Shared<PlatformD3D11DeviceWin> _d3d11Device;
    Microsoft::WRL::ComPtr<IDCompositionDesktopDevice> _dcompDevice;
    Microsoft::WRL::ComPtr<IPresentationFactory> _presentationFactory;
    std::vector<Weak<PlatformGraphicsDeviceObjectWin>> _deviceObjects;
};

template <>
struct Locator::Resolver<PlatformDCompositionDeviceWin>
{
    using Interface = PlatformDCompositionDeviceWin;
    static auto Resolve() -> Shared<PlatformDCompositionDeviceWin>;
};
}
}

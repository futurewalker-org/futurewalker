// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformDrawableViewLayerContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformDrawableViewLayerWin.hpp"

#include "Futurewalker.Graphics.Win.PlatformD3D11DeviceWin.hpp"
#include "Futurewalker.Graphics.Win.PlatformD3D12DeviceWin.hpp"
#include "Futurewalker.Graphics.Win.PlatformGraphicsDeviceWin.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Core.StringFunction.hpp"
#include "Futurewalker.Core.Win.PlatformStringFunctionWin.hpp"

#include <include/gpu/ganesh/d3d/GrD3DBackendContext.h>

namespace FW_DETAIL_NS
{
namespace
{
auto constexpr WindowClassName = L"58b733a3-cd9b-457d-835d-e555efd12dea";
auto constexpr WindowName = L"";

auto RegisterWindowClass(LPCWSTR className, WNDPROC windowProc, HINSTANCE instance) -> ATOM
{
    const auto wndClass = WNDCLASSEXW {
      .cbSize = sizeof(WNDCLASSEX),
      .style = CS_HREDRAW | CS_VREDRAW,
      .lpfnWndProc = windowProc,
      .cbClsExtra = 0,
      .cbWndExtra = 0,
      .hInstance = instance,
      .hIcon = ::LoadIconW(NULL, IDI_APPLICATION),
      .hCursor = ::LoadCursorW(NULL, IDC_ARROW),
      .hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH), //::GetSysColorBrush(COLOR_WINDOW),
      .lpszMenuName = NULL,
      .lpszClassName = className,
      .hIconSm = NULL,
    };

    if (const auto atom = ::RegisterClassExW(&wndClass))
    {
        return atom;
    }
    FW_DEBUG_LOG_ERROR("Failed to register window class");
    throw Exception(ErrorCode::Failure, "Failed to register window class");
}

auto UnregisterWindowClass(ATOM windowClass, HINSTANCE instance) -> void
{
    if (!::UnregisterClassW(MAKEINTATOM(windowClass), instance))
    {
        FW_DEBUG_LOG_ERROR("Failed to unregister window class");
        FW_DEBUG_ASSERT(false);
    }
}
}

///
/// @brief
///
/// @param instanceHandle
/// @param dcompDevice
/// @param d3d12Device
///
/// @return
///
auto PlatformDrawableViewLayerContextWin::Make(Shared<PlatformInstanceHandleWin> instanceHandle, Shared<PlatformDCompositionDeviceWin> dcompDevice, Shared<PlatformD3D12DeviceWin> d3d12Device)
  -> Shared<PlatformDrawableViewLayerContextWin>
{
    auto key = PassKey<PlatformDrawableViewLayerContextWin>();
    auto context = Shared<PlatformDrawableViewLayerContextWin>::Make(key, instanceHandle, dcompDevice, d3d12Device);
    context->_self = context;
    return context;
}

///
/// @brief
///
/// @param instanceHandle
/// @param dcompDevice
/// @param d3d12Device
///
PlatformDrawableViewLayerContextWin::PlatformDrawableViewLayerContextWin(
  PassKey<PlatformDrawableViewLayerContextWin>,
  Shared<PlatformInstanceHandleWin> instanceHandle,
  Shared<PlatformDCompositionDeviceWin> dcompDevice,
  Shared<PlatformD3D12DeviceWin> d3d12Device)
  : _instanceHandle {instanceHandle}
  , _dcompDevice {dcompDevice}
  , _d3d12Device {d3d12Device}
  , _windowClassAtom {NULL}
{
    const auto instance = instanceHandle->GetInstanceHandle();
    const auto windowClassName = std::wstring(WindowClassName);
    _windowClassAtom = RegisterWindowClass(windowClassName.c_str(), &PlatformDrawableViewLayerContextWin::WindowProcedure, instance);
    _graphicsDevice = Graphics::PlatformGraphicsDeviceWin::MakeForD3D12(_d3d12Device);
}

///
/// @brief
///
PlatformDrawableViewLayerContextWin::~PlatformDrawableViewLayerContextWin()
{
    const auto instance = _instanceHandle->GetInstanceHandle();
    UnregisterWindowClass(_windowClassAtom, instance);
}

///
/// @brief
///
auto PlatformDrawableViewLayerContextWin::MakeDrawableLayer() -> Shared<PlatformViewLayer>
{
    return PlatformDrawableViewLayerWin::Make(GetSelf(), _dcompDevice);
}

///
/// @brief 
///
auto PlatformDrawableViewLayerContextWin::MakeSwapChainSurface(IntPx const width, IntPx const height) -> Shared<Graphics::PlatformSwapChainSurfaceWin>
{
    if (_graphicsDevice)
    {
        return _graphicsDevice->MakeSwapChainSurface(width, height);
    }
    return {};
}

///
/// @brief Make child window which hosts PlatformDrawableViewLayerWin.
///
/// @param parent Parent window handle.
///
/// @return New child window handle.
///
auto PlatformDrawableViewLayerContextWin::MakeChildWindowHandle(HWND parent) -> HWND
{
    const auto style = DWORD(WS_CHILD | WS_VISIBLE);
    const auto exStyle = DWORD(WS_EX_LAYERED | WS_EX_NOREDIRECTIONBITMAP);
    const auto instanceHandle = _instanceHandle->GetInstanceHandle();
    return ::CreateWindowExW(exStyle, MAKEINTATOM(_windowClassAtom), WindowName, style, 0, 0, 1, 1, parent, NULL, instanceHandle, nullptr);
}

///
/// @brief
///
/// @param hwnd
///
auto PlatformDrawableViewLayerContextWin::DestroyChildWindowHandle(HWND hwnd) -> void
{
    ::DestroyWindow(hwnd);
}

///
/// @brief
///
auto PlatformDrawableViewLayerContextWin::GetSelf() -> Shared<PlatformDrawableViewLayerContextWin>
{
    return _self.Lock();
}

///
/// @brief
///
/// @param hWnd
/// @param msg
/// @param wParam
/// @param lParam
///
auto CALLBACK PlatformDrawableViewLayerContextWin::WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief
///
auto Locator::Resolver<PlatformDrawableViewLayerContextWin>::Resolve() -> Shared<PlatformDrawableViewLayerContextWin>
{
    const auto instanceHandle = Locator::Resolve<PlatformInstanceHandleWin>();
    const auto dcompDevice = Locator::Resolve<PlatformDCompositionDeviceWin>();
    const auto d3d12Device = Locator::Resolve<PlatformD3D12DeviceWin>();
    return PlatformDrawableViewLayerContextWin::Make(instanceHandle, dcompDevice, d3d12Device);
}
}

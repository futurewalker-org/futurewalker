// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformScreenWin.hpp"
#include "Futurewalker.Application.PlatformScreen.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.Win.PlatformStringFunctionWin.hpp"

#include <shellscalingapi.h>

namespace FW_DETAIL_NS
{
namespace
{
auto GetMonitorDisplayScale(MONITORINFOEXW const& info) -> DisplayScale
{
    // GetDpiForMonitor() does not support per-monitor DPI atm.
    // Use EnumDisplaySettingsW() instead.
    auto devMode = DEVMODEW {};
    if (::EnumDisplaySettingsW(info.szDevice, ENUM_CURRENT_SETTINGS, &devMode))
    {
        return static_cast<float64_t>(devMode.dmPelsWidth) / (info.rcMonitor.right - info.rcMonitor.left);
    }
    return 1.0;
}

auto GetMonitorDisplayName(MONITORINFOEXW const& info) -> String
{
    return PlatformStringFunctionWin::WideToUtf8(info.szDevice);
}
}

///
/// @brief 
///
/// @param monitor 
///
PlatformScreenWin::PlatformScreenWin(HMONITOR monitor)
  : _monitor {monitor}
{
}

///
/// @brief
///
auto PlatformScreenWin::GetInfo() const -> Optional<ScreenInfo>
{
    return _info;
}

///
/// @brief
///
auto PlatformScreenWin::Refresh() -> void
{
    Invalidate();

    if (_monitor)
    {
        auto info = MONITORINFOEXW {MONITORINFO {.cbSize = sizeof(MONITORINFOEXW)}};
        if (::GetMonitorInfoW(_monitor, &info))
        {
            auto const frameRect = info.rcMonitor;
            auto const spFrameRect = Rect<Sp>(frameRect.left, frameRect.top, frameRect.right, frameRect.bottom);

            auto const workRect = info.rcWork;
            auto const spWorkRect = Rect<Sp>(0, 0, workRect.right - workRect.left, workRect.bottom - workRect.top);

            auto const displayScale = GetMonitorDisplayScale(info);
            auto const backingScale = BackingScale(1.0);

            auto const dpFrameRect = UnitFunction::ConvertSpToDp(spFrameRect, displayScale);
            auto const dpWorkRect = UnitFunction::ConvertSpToDp(spWorkRect, displayScale);

            auto const displayName = GetMonitorDisplayName(info);

            _info = ScreenInfo {
              .frameRect = dpFrameRect,
              .workAreaRect = dpWorkRect,
              .safeAreaRect = dpWorkRect,
              .displayScale = displayScale,
              .backingScale = backingScale,
              .displayName = displayName,
            };
        }
    }
}

///
/// @brief
///
auto PlatformScreenWin::Invalidate() -> void
{
    _info = {};
}

///
/// @brief
///
auto PlatformScreenWin::GetNativeHandle() const -> HMONITOR
{
    return _monitor;
}
}

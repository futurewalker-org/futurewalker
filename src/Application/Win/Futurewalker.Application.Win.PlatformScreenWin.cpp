// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformScreenWin.hpp"
#include "Futurewalker.Application.PlatformScreen.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Core.Win.PlatformStringFunctionWin.hpp"

#include <shellscalingapi.h>

namespace FW_DETAIL_NS
{
namespace
{
auto GetMonitorDisplayScale(HMONITOR const monitor) -> DisplayScale
{
    auto xDpi = UINT(USER_DEFAULT_SCREEN_DPI);
    auto yDpi = UINT(USER_DEFAULT_SCREEN_DPI);
    auto const hr = ::GetDpiForMonitor(monitor, MDT_EFFECTIVE_DPI, &xDpi, &yDpi);
    if (SUCCEEDED(hr))
    {
        FW_DEBUG_ASSERT(xDpi == yDpi);
        return DisplayScale(static_cast<float64_t>(yDpi) / USER_DEFAULT_SCREEN_DPI);
    }
    FW_DEBUG_ASSERT(false);
    return 1.0;
}

auto GetMonitorDisplayName(MONITORINFOEXW const& info) -> String
{
    return PlatformStringFunctionWin::WideToUtf8(info.szDevice);
}

auto RectToVpRect(RECT const& rect) -> Rect<Vp>
{
    return Rect<Vp>(rect.left, rect.top, rect.right, rect.bottom);
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
            auto const bounds = RectToVpRect(info.rcMonitor);
            auto const workArea = RectToVpRect(info.rcWork);
            auto const safeArea = RectToVpRect(info.rcWork);

            auto const displayScale = GetMonitorDisplayScale(_monitor);
            auto const backingScale = BackingScale(1.0);

            auto const displayName = GetMonitorDisplayName(info);

            _info = ScreenInfo {
                .bounds = bounds,
                .workArea = workArea,
                .safeArea = safeArea,
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

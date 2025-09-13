// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformScreenContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformWindowWin.hpp"
#include "Futurewalker.Application.Win.PlatformScreenWin.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
namespace
{
struct MonitorEnumProcData
{
    PlatformScreenArray& screens;
    PlatformScreenArray unchanged;
    PlatformScreenArray connected;
};

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC /*hdc*/, LPRECT /*lpRect*/, LPARAM dwData)
{
    try
    {
        auto const data = reinterpret_cast<MonitorEnumProcData*>(dwData);

        auto& screens = data->screens;

        auto const it = std::find_if(screens.begin(), screens.end(), [&](auto&& p) { return p.template As<PlatformScreenWin>()->GetNativeHandle() == hMonitor; });

        if (it != screens.end())
        {
            data->unchanged.push_back(std::move(*it));
            screens.erase(it);
        }
        else
        {
            data->connected.push_back(Shared<PlatformScreenWin>::Make(hMonitor));
        }
    }
    catch (...)
    {
        FW_DEBUG_ASSERT(false);
    }
    return TRUE;
}

struct ScreenRefreshInfo
{
    PlatformScreenArray disconnected;
    PlatformScreenArray unchanged;
    PlatformScreenArray connected;
};

auto GetRefreshInfo(PlatformScreenArray screens) -> ScreenRefreshInfo
{
    auto data = MonitorEnumProcData {
        .screens = screens,
        .unchanged = {},
        .connected = {},
    };

    if (::EnumDisplayMonitors(nullptr, nullptr, static_cast<MONITORENUMPROC>(MonitorEnumProc), reinterpret_cast<LPARAM>(&data)))
    {
        return {
            .disconnected = data.screens,
            .unchanged = data.unchanged,
            .connected = data.connected,
        };
    }
    return {};
}

auto GetPrimaryMonitor() -> HMONITOR
{
    const POINT p = {0, 0};
    return MonitorFromPoint(p, MONITOR_DEFAULTTOPRIMARY);
}
}

///
/// @brief Get list of connected screens.
///
auto PlatformScreenContextWin::GetScreens() -> PlatformScreenArray
{
    return _screens;
}

///
/// @brief Get primary screen.
///
auto PlatformScreenContextWin::GetPrimaryScreen() -> Shared<PlatformScreen>
{
    if (!_screens.empty())
    {
        return _screens[0];
    }
    return {};
}

///
/// @brief Get screen from window.
///
auto PlatformScreenContextWin::GetScreenFromWindow(Shared<PlatformWindow> window) -> Shared<PlatformScreen>
{
    if (auto const win = window.Maybe<PlatformWindowWin>())
    {
        auto const hwnd = win->GetNativeHandle();
        if (::IsWindow(hwnd))
        {
            if (auto const monitor = ::MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST))
            {
                for (auto const& screen : _screens)
                {
                    if (screen.As<PlatformScreenWin>()->GetNativeHandle() == monitor)
                    {
                        return screen;
                    }
                }
            }
        }
    }
    return {};
}

///
/// @brief Get screen from rect.
///
auto PlatformScreenContextWin::GetScreenFromRect(Rect<Vp> const& rect) -> Shared<PlatformScreen>
{
    auto const l = static_cast<LONG>(Vp::Floor(rect.GetLeft()));
    auto const t = static_cast<LONG>(Vp::Floor(rect.GetTop()));
    auto const r = static_cast<LONG>(Vp::Floor(rect.GetRight()));
    auto const b = static_cast<LONG>(Vp::Floor(rect.GetBottom()));
    auto const targetRect = RECT {.left = l, .top = t, .right = r, .bottom = b};
    auto const monitor = ::MonitorFromRect(&targetRect, MONITOR_DEFAULTTONEAREST);
    for (auto const& screen : _screens)
    {
        if (screen.As<PlatformScreenWin>()->GetNativeHandle() == monitor)
        {
            return screen;
        }
    }
    return {};
}

///
/// @brief Refresh screen info.
///
auto PlatformScreenContextWin::Refresh() -> void
{
    auto const refreshInfo = GetRefreshInfo(_screens);
    Add(refreshInfo.connected);
    Update(refreshInfo.unchanged);
    Remove(refreshInfo.disconnected);
}

///
/// @brief Add screens to list.
///
auto PlatformScreenContextWin::Add(const PlatformScreenArray& screens) -> void
{
    _screens.insert(_screens.end(), screens.begin(), screens.end());

    for (auto const& screen : _screens)
    {
        screen.As<PlatformScreenWin>()->Refresh();
    }

    // Move primary screen to front.
    const auto primary = GetPrimaryMonitor();
    std::stable_partition(_screens.begin(), _screens.end(), [&](auto&& s) { return s.template As<PlatformScreenWin>()->GetNativeHandle() == primary; });
}

///
/// @brief Remove screens from list.
///
auto PlatformScreenContextWin::Remove(const PlatformScreenArray& screens) -> void
{
    for (auto const& screen : screens)
    {
        std::erase(_screens, screen);
        screen.As<PlatformScreenWin>()->Invalidate();
    }
}

///
/// @brief Update screens.
///
auto PlatformScreenContextWin::Update(const PlatformScreenArray& screens) -> void
{
    for (auto const& screen : screens)
    {
        screen.As<PlatformScreenWin>()->Refresh();
    }
}

auto Locator::Resolver<PlatformScreenContextWin>::Resolve() -> Shared<PlatformScreenContextWin>
{
    return Shared<PlatformScreenContextWin>::Make();
}
}
}

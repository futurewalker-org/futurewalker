// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformWindowWin.hpp"
#include "Futurewalker.Application.Win.PlatformWindowContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformRootViewLayerWin.hpp"
#include "Futurewalker.Application.Win.PlatformKeyboardLayoutWin.hpp"
#include "Futurewalker.Application.Win.PlatformApplicationContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformApplicationWin.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodTextStoreWin.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodWin.hpp"
#include "Futurewalker.Application.Win.PlatformPointerEventFunctionWin.hpp"
#include "Futurewalker.Application.PlatformWindowEvent.hpp"
#include "Futurewalker.Application.PlatformFrameEvent.hpp"
#include "Futurewalker.Application.PlatformPointerEvent.hpp"
#include "Futurewalker.Application.PlatformKeyEvent.hpp"
#include "Futurewalker.Application.PlatformInputEvent.hpp"
#include "Futurewalker.Application.PlatformHitTestEvent.hpp"
#include "Futurewalker.Application.MainThread.hpp"
#include "Futurewalker.Application.Key.hpp"

#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Async.AsyncFunction.hpp"

#include "Futurewalker.Core.MonotonicClock.hpp"
#include "Futurewalker.Core.StringFunction.hpp"
#include "Futurewalker.Core.ScopedResetter.hpp"
#include "Futurewalker.Core.Win.PlatformStringFunctionWin.hpp"

#include <dwmapi.h>

#include <utility>
#include <numbers>

namespace FW_DETAIL_NS
{
///
/// @brief
///
/// @param context
/// @param options
/// @param delegate
///
/// @return
///
auto PlatformWindowWin::Make(
  Shared<PlatformWindowContextWin> context,
  Shared < PlatformApplicationThemeContext> themeContext,
  Shared<PlatformDCompositionDeviceWin> compositionDevice,
  PlatformWindowOptions const& options,
  Delegate const& delegate) -> Shared<PlatformWindowWin>
{
    auto window = PlatformWindow::MakeDerived<PlatformWindowWin>(context, themeContext, compositionDevice, options, delegate);
    window->SetSelf(window);
    context->InitializeWindow(window, options);
    return window;
}

///
/// @brief
///
/// @param context
/// @param parameter
/// @param delegate
///
PlatformWindowWin::PlatformWindowWin(
  PassKey<PlatformWindow> key,
  Shared<PlatformWindowContextWin> context,
  Shared<PlatformApplicationThemeContext> themeContext,
  Shared<PlatformDCompositionDeviceWin> compositionDevice,
  PlatformWindowOptions const& options,
  Delegate const& delegate)
  : PlatformWindow(key, delegate)
  , _context {context}
  , _compositionDevice {compositionDevice}
  , _options {options}
{
    _theme = themeContext->MakeApplicationTheme({.sendThemeEvent = [&](Event<>& event) -> Async<Bool> { co_return co_await ReceiveThemeEvent(event); }});
}

///
/// @brief
///
PlatformWindowWin::~PlatformWindowWin()
{
    _destructing = true;

    if (!IsClosed())
    {
        Destroy();
    }
}

///
/// @brief
///
auto PlatformWindowWin::GetContext() -> Shared<PlatformWindowContextWin>
{
    return _context;
}

///
/// @brief
///
auto PlatformWindowWin::GetNativeHandle() const -> HWND
{
    return _hwnd;
}

///
/// @brief
///
auto PlatformWindowWin::GetOptions() const -> PlatformWindowOptions
{
    return _options;
}

///
/// @brief
///
auto PlatformWindowWin::AddOwnedWindow(Shared<PlatformWindowWin> const& window) -> void
{
    for (const auto& win : _ownedWindows)
    {
        if (win.Lock() == window)
        {
            return;
        }
    }
    _ownedWindows.push_back(window);
}

///
/// @brief
///
auto PlatformWindowWin::RemoveOwnedWindow(Shared<PlatformWindowWin> const& window) -> void
{
    const auto it = std::remove_if(_ownedWindows.begin(), _ownedWindows.end(), [window](const auto& win) { return win.Lock() == window; });
    _ownedWindows.erase(it, _ownedWindows.end());
}

///
/// @brief
///
auto PlatformWindowWin::GetOwnedWindows() -> const std::vector<Weak<PlatformWindowWin>>&
{
    return _ownedWindows;
}

///
/// @brief
///
auto PlatformWindowWin::IsVisible() -> Bool
{
    if (!IsClosed())
    {
        return static_cast<bool>(::IsWindowVisible(_hwnd));
    }
    return false;
}

///
/// @brief
///
/// @param visible
///
auto PlatformWindowWin::SetVisible(Bool visible) -> void
{
    if (!IsClosed())
    {
        if (IsVisible() != visible)
        {
            if (visible)
            {
                ::ShowWindow(_hwnd, _restoreOnShow ? SW_SHOWNORMAL : SW_SHOW);
                _restoreOnShow = false;
            }
            else
            {
                ::ShowWindow(_hwnd, SW_HIDE);
            }
        }
    }
}

///
/// @brief
///
auto PlatformWindowWin::IsActive() -> Bool
{
    if (!IsClosed())
    {
        return ::GetActiveWindow() == _hwnd;
    }
    return false;
}

///
/// @brief
///
auto PlatformWindowWin::SetActive() -> void
{
    if (!IsClosed())
    {
        ::SetActiveWindow(_hwnd);
    }
}

///
/// @brief
///
auto PlatformWindowWin::IsFocused() -> Bool
{
    if (!IsClosed())
    {
        return ::GetFocus() == _hwnd;
    }
    return false;
}

///
/// @brief
///
auto PlatformWindowWin::SetFocus() -> void
{
    if (!IsClosed())
    {
        ::SetFocus(_hwnd);
    }
}

///
/// @brief
///
auto PlatformWindowWin::GetFrameRect() -> Rect<Vp>
{
    if (!IsClosed())
    {
        auto placement = WINDOWPLACEMENT();
        placement.length = sizeof(WINDOWPLACEMENT);
        if (!::GetWindowPlacement(_hwnd, &placement))
        {
            FW_DEBUG_ASSERT(false);
            return {};
        }

        if (placement.showCmd == SW_SHOWMINIMIZED)
        {
            auto rect = Rect<Vp>(placement.rcNormalPosition.left, placement.rcNormalPosition.top, placement.rcNormalPosition.right, placement.rcNormalPosition.bottom);
            if (!HasWindowStyle(WS_EX_TOOLWINDOW))
            {
                auto monitor = ::MonitorFromWindow(_hwnd, MONITOR_DEFAULTTOPRIMARY);
                auto monitorInfo = MONITORINFO();
                monitorInfo.cbSize = sizeof(MONITORINFO);
                if (::GetMonitorInfoW(monitor, &monitorInfo))
                {
                    rect.x0 += monitorInfo.rcWork.left;
                    rect.y0 += monitorInfo.rcWork.top;
                }
            }
            return rect;
        }
        else
        {
            auto rect = RECT();
            if (::GetWindowRect(_hwnd, &rect))
            {
                return Rect<Vp>(rect.left, rect.top, rect.right, rect.bottom);
            }
        }
        FW_DEBUG_ASSERT(false);
    }
    return {};
}

///
/// @brief
///
/// @param rect
///
auto PlatformWindowWin::SetFrameRect(Rect<Vp> const& rect) -> void
{
    if (!IsClosed())
    {
        auto const w = static_cast<int>(Vp::Round(rect.GetWidth()));
        auto const h = static_cast<int>(Vp::Round(rect.GetHeight()));
        auto x = static_cast<int>(Vp::Round(rect.x0));
        auto y = static_cast<int>(Vp::Round(rect.y0));

        auto placement = WINDOWPLACEMENT();
        placement.length = sizeof(WINDOWPLACEMENT);
        if (!::GetWindowPlacement(_hwnd, &placement))
        {
            FW_DEBUG_ASSERT(false);
            return;
        }

        if (placement.showCmd == SW_SHOWMINIMIZED || placement.showCmd == SW_SHOWMAXIMIZED)
        {
            if (!HasWindowStyle(WS_EX_TOOLWINDOW))
            {
                auto monitor = ::MonitorFromWindow(_hwnd, MONITOR_DEFAULTTOPRIMARY);
                auto monitorInfo = MONITORINFO();
                monitorInfo.cbSize = sizeof(MONITORINFO);
                if (::GetMonitorInfoW(monitor, &monitorInfo))
                {
                    x -= monitorInfo.rcWork.left;
                    y -= monitorInfo.rcWork.top;
                }
            }

            placement.rcNormalPosition.left = x;
            placement.rcNormalPosition.top = y;
            placement.rcNormalPosition.right = placement.rcNormalPosition.left + w;
            placement.rcNormalPosition.bottom = placement.rcNormalPosition.top + h;

            if (::IsWindowVisible(_hwnd))
            {
                placement.showCmd = placement.showCmd == SW_SHOWMINIMIZED ? SW_SHOWMINNOACTIVE : SW_SHOWNOACTIVATE;
            }
            else
            {
                if (placement.showCmd == SW_SHOWMAXIMIZED)
                {
                    _restoreOnShow = true;
                }
                placement.showCmd = SW_HIDE;
            }

            if (::SetWindowPlacement(_hwnd, &placement))
            {
                return;
            }
        }
        else
        {
            if (::SetWindowPos(_hwnd, NULL, x, y, w, h, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER))
            {
                return;
            }
        }
        FW_DEBUG_ASSERT(false);
    }
}

///
/// @brief 
///
auto PlatformWindowWin::GetRestoredFrameRect() -> Rect<Vp>
{
    if (!IsClosed())
    {
        auto placement = WINDOWPLACEMENT();
        placement.length = sizeof(WINDOWPLACEMENT);
        if (::GetWindowPlacement(_hwnd, &placement))
        {
            auto rect = Rect<Vp>(placement.rcNormalPosition.left, placement.rcNormalPosition.top, placement.rcNormalPosition.right, placement.rcNormalPosition.bottom);
            if (!HasWindowStyle(WS_EX_TOOLWINDOW))
            {
                auto monitor = ::MonitorFromWindow(_hwnd, MONITOR_DEFAULTTOPRIMARY);
                auto monitorInfo = MONITORINFO();
                monitorInfo.cbSize = sizeof(MONITORINFO);
                if (::GetMonitorInfoW(monitor, &monitorInfo))
                {
                    rect.x0 += monitorInfo.rcWork.left;
                    rect.y0 += monitorInfo.rcWork.top;
                }
            }
            return rect;
        }
    }
    return {};
}

///
/// @brief
///
/// @param rect
///
auto PlatformWindowWin::SetRestoredFrameRect(Rect<Vp> const& rect) -> void
{
    if (!IsClosed())
    {
        auto placement = WINDOWPLACEMENT();
        placement.length = sizeof(WINDOWPLACEMENT);
        if (::GetWindowPlacement(_hwnd, &placement))
        {
            auto const w = static_cast<int>(Vp::Round(rect.GetWidth()));
            auto const h = static_cast<int>(Vp::Round(rect.GetHeight()));
            auto x = static_cast<int>(Vp::Round(rect.x0));
            auto y = static_cast<int>(Vp::Round(rect.y0));

            if (!HasWindowStyle(WS_EX_TOOLWINDOW))
            {
                auto monitor = ::MonitorFromWindow(_hwnd, MONITOR_DEFAULTTOPRIMARY);
                auto monitorInfo = MONITORINFO();
                monitorInfo.cbSize = sizeof(MONITORINFO);
                if (::GetMonitorInfoW(monitor, &monitorInfo))
                {
                    x -= monitorInfo.rcWork.left;
                    y -= monitorInfo.rcWork.top;
                }
            }

            placement.rcNormalPosition.left = x;
            placement.rcNormalPosition.top = y;
            placement.rcNormalPosition.right = placement.rcNormalPosition.left + w;
            placement.rcNormalPosition.bottom = placement.rcNormalPosition.top + h;

            if (::IsWindowVisible(_hwnd))
            {
                if (placement.showCmd == SW_SHOWMINIMIZED)
                {
                    placement.showCmd = SW_SHOWMINNOACTIVE;
                }
                else
                {
                    placement.showCmd = SW_SHOWNA;
                }
            }
            else
            {
                placement.showCmd = SW_HIDE;
            }

            if (::SetWindowPlacement(_hwnd, &placement))
            {
                return;
            }
        }
        FW_DEBUG_ASSERT(false);
    }
}

///
/// @brief Get bounding rectangles which are obscured by system UI elements within the specified area.
///
/// @param[in] area Area to get bounds.
///
/// @note This does not include the bounds of the area itself. Use GetAreaInsets to calculate overall geometry of the area.
///
auto PlatformWindowWin::GetAreaBounds(WindowArea const area) -> std::vector<Rect<Dp>>
{
    if (!IsClosed())
    {
        if (area == WindowArea::TitleBar)
        {
            if (_options.hasTitleBar)
            {
                const auto controlRect = GetSystemControlRect();
                if (!controlRect.IsEmpty())
                {
                    return {UnitFunction::ConvertVpToDp(controlRect, GetDisplayScale())};
                }
            }
        }
    }
    return {};
}

///
/// @brief Get insets of the specified area.
///
/// @param[in] area Area to get insets.
///
auto PlatformWindowWin::GetAreaInsets(WindowArea const area) -> EdgeInsets
{
    if (!IsClosed())
    {
        if (area == WindowArea::Frame)
        {
            // This should match WM_NCCALCSIZE handling.
            if (_options.hasFrame)
            {
                if (_options.hasTitleBar || !HasWindowStyle(WS_POPUP))
                {
                    const auto marginX = UnitFunction::ConvertVpToDp(GetSystemFrameThicknessX(), GetDisplayScale());
                    const auto marginTop = UnitFunction::ConvertVpToDp(0, GetDisplayScale());
                    const auto marginBottom = UnitFunction::ConvertVpToDp(GetSystemFrameThicknessY(), GetDisplayScale());
                    return EdgeInsets(marginX, marginTop, marginX, marginBottom);
                }
                else
                {
                    return EdgeInsets::MakeUniform(1);
                }
            }
        }
        else if (area == WindowArea::TitleBar)
        {
            auto const titleBarHeight = _options.hasTitleBar ? UnitFunction::ConvertVpToDp(GetSystemTitleBarHeight(), GetDisplayScale()) : Dp(0);
            auto clientInsets = GetAreaInsets(WindowArea::Frame);
            clientInsets.SetTop(clientInsets.GetTop() + titleBarHeight);
            return clientInsets;
        }
    }
    return {};
}

///
/// @brief
///
/// @return
///
auto PlatformWindowWin::GetDisplayScale() -> DisplayScale
{
    return DisplayScale(_dpi) / USER_DEFAULT_SCREEN_DPI;
}

///
/// @brief
///
/// @return
///
auto PlatformWindowWin::GetBackingScale() -> BackingScale
{
    return 1.0;
}

///
/// @brief
///
auto PlatformWindowWin::GetSizeConstraints() -> BoxConstraints
{
    return _sizeConstraints;
}

///
/// @brief
///
/// @return
///
auto PlatformWindowWin::SetSizeConstraints(BoxConstraints const& constraints) -> void
{
    if (!IsClosed())
    {
        if (_sizeConstraints != constraints)
        {
            _sizeConstraints = constraints;
        }
    }
}

///
/// @brief
///
auto PlatformWindowWin::GetTitle() -> String
{
    return _title;
}

///
/// @brief
///
/// @param title
///
auto PlatformWindowWin::SetTitle(String const& title) -> void
{
    if (!IsClosed())
    {
        if (_title != title)
        {
            const auto wideTitle = PlatformStringFunctionWin::Utf8ToWide(title);
            ::SetWindowTextW(_hwnd, wideTitle.c_str());
        }
    }
}

///
/// @brief
///
auto PlatformWindowWin::Minimize() -> void
{
    if (!IsClosed())
    {
        ::ShowWindow(_hwnd, SW_MINIMIZE);
        _restoreOnShow = false;
    }
}

///
/// @brief
///
auto PlatformWindowWin::Maximize() -> void
{
    if (!IsClosed())
    {
        ::ShowWindow(_hwnd, SW_MAXIMIZE);
        _restoreOnShow = false;
    }
}

///
/// @brief
///
auto PlatformWindowWin::Restore() -> void
{
    if (!IsClosed())
    {
        if (::IsWindowVisible(_hwnd))
        {
            ::ShowWindow(_hwnd, SW_RESTORE);
        }
        else
        {
            _restoreOnShow = true;
        }
    }
}

///
/// @brief
///
auto PlatformWindowWin::IsClosed() -> Bool
{
    if (_hwnd)
    {
        return _closed;
    }
    return true;
}

///
/// @brief
///
auto PlatformWindowWin::Close() -> Async<Bool>
{
    if (!IsClosed())
    {
        auto event = Event<>(Event<PlatformWindowEvent::CloseRequested>());
        co_await SendWindowEvent(event);

        auto close = True;
        if (event.Is<PlatformWindowEvent::CloseRequested>())
        {
            if (event.As<PlatformWindowEvent::CloseRequested>()->IsCancelled())
            {
                close = false;
            }
        }

        if (close)
        {
            ::DestroyWindow(_hwnd);
        }
        co_return close;
    }
    co_return true;
}

///
/// @brief
///
auto PlatformWindowWin::Destroy() -> void
{
    if (_hwnd)
    {
        if (const auto owner = _options.owner.TryAs<PlatformWindowWin>())
        {
            if (_options.behavior == WindowBehavior::Dialog)
            {
                owner->EnableWindow(GetSelf());
            }
        }

        if (!::DestroyWindow(_hwnd))
        {
            FW_DEBUG_LOG_ERROR("PlatformWindowWin: DestroyWindow failed");
        }
    }
}

///
/// @brief
///
auto PlatformWindowWin::Render() -> void
{
    if (_rootViewLayer)
    {
        _rootViewLayer->Render();
    }

    if (_compositionDevice)
    {
        _compositionDevice->Commit();
    }
}

///
/// @brief
///
auto PlatformWindowWin::RequestFrame() -> void
{
    if (!IsClosed())
    {
        if (const auto context = GetContext())
        {
            context->RequestFrame(GetSelf(), [](Shared<void> data, PlatformVsyncFrameInfo const& frameInfo) {
                if (const auto window = data.UnsafeAs<PlatformWindowWin>())
                {
                    window->HandleFrameSwap(frameInfo.targetTimestamp);
                }
            });
        }
    }
}

///
/// @brief
///
auto PlatformWindowWin::GetFrameTime() -> MonotonicTime
{
    if (_context)
    {
        return _context->GetFrameTime(*this);
    }
    FW_DEBUG_ASSERT(false);
    return {};
}

///
/// @brief
///
auto PlatformWindowWin::CapturePointer(PointerId const id) -> void
{
    if (!IsClosed())
    {
        if (id == 1U)
        {
            ::SetCapture(_hwnd);
        }
    }
}

///
/// @brief
///
auto PlatformWindowWin::ReleasePointer(PointerId const id) -> void
{
    if (!IsClosed())
    {
        if (id == 1U)
        {
            ::ReleaseCapture();
        }
    }
}

///
/// @brief
///
auto PlatformWindowWin::GetViewLayer() -> Shared<PlatformViewLayer>
{
    return _rootViewLayer;
}

///
/// @brief
///
auto PlatformWindowWin::GetInputMethod() -> Shared<PlatformInputMethod>
{
    return _inputMethod;
}

///
/// @brief
///
auto PlatformWindowWin::SetBackgroundColor(RGBColor const& backgroundColor) -> void
{
    if (_backgroundColor != backgroundColor)
    {
        _backgroundColor = backgroundColor;

        if (_options.backgroundStyle == WindowBackgroundStyle::Solid)
        {
            const auto r = static_cast<BYTE>(_backgroundColor.GetRedU8());
            const auto g = static_cast<BYTE>(_backgroundColor.GetGreenU8());
            const auto b = static_cast<BYTE>(_backgroundColor.GetBlueU8());
            const auto color = RGB(r, g, b);
            if (FAILED(::DwmSetWindowAttribute(_hwnd, DWMWA_CAPTION_COLOR, &color, sizeof(color))))
            {
                FW_DEBUG_LOG_ERROR("PlatformWindowWin: Failed to set DWMWA_CAPTION_COLOR");
            }
            // Trigger WM_ERASEBKGND.
            ::InvalidateRect(_hwnd, nullptr, TRUE);
        }
    }
}

///
/// @brief Set native window handle.
///
/// @param hwnd A window handle.
///
auto PlatformWindowWin::SetNativeHandle(PassKey<PlatformWindowContextWin>, HWND hwnd) -> void
{
    if (::IsWindow(hwnd))
    {
        _hwnd = hwnd;
        _dpi = ::GetDpiForWindow(_hwnd);
        _rootViewLayer = PlatformRootViewLayerWin::Make(_hwnd, _compositionDevice);
    }
}

//!
//! @brief Initialize.
//!
auto PlatformWindowWin::Initialize() -> void
{
}

//!
//! @brief
//!
auto PlatformWindowWin::ReceiveThemeEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<PlatformApplicationThemeEvent::SystemBrightnessChanged>())
    {
        UpdateThemeColor();
    }
    co_return false;
}

///
/// @brief
///
/// @param hWnd
/// @param msg
/// @param wParam
/// @param lParam
///
auto PlatformWindowWin::WindowProcedure(PassKey<PlatformWindowContextWin>, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    auto self = GetSelf();

    if (msg == WM_CREATE)
    {
        return HandleCreate(hWnd, msg, wParam, lParam);
    }
    else if (msg == WM_NCCALCSIZE)
    {
        return HandleNcCalcSize(hWnd, msg, wParam, lParam);
    }
    else if (msg == WM_NCACTIVATE)
    {
        return HandleNcActivate(hWnd, msg, wParam, lParam);
    }
    else if (msg == WM_SHOWWINDOW)
    {
        return HandleShow(hWnd, msg, wParam, lParam);
    }
    else if (msg == WM_GETMINMAXINFO)
    {
        return HandleGetMinMaxInfo(hWnd, msg, wParam, lParam);
    }
    else if (msg == WM_ACTIVATE)
    {
        return HandleActivate(hWnd, msg, wParam, lParam);
    }
    else if (msg == WM_ACTIVATEAPP)
    {
        return HandleActivateApp(hWnd, msg, wParam, lParam);
    }
    else if (msg == WM_GETICON)
    {
        // TODO: handle icon change.
    }
    else if (msg == WM_GETOBJECT)
    {
        // TODO: handle accessibility.
    }
    else if (msg == WM_SETFOCUS)
    {
        return HandleSetFocus(hWnd, msg, wParam, lParam);
    }
    else if (msg == WM_KILLFOCUS)
    {
        return HandleKillFocus(hWnd, msg, wParam, lParam);
    }
    else if (msg == WM_CHAR || msg == WM_DEADCHAR || msg == WM_KEYDOWN || msg == WM_KEYUP || msg == WM_HOTKEY || msg == WM_SYSKEYDOWN || msg == WM_SYSKEYUP || msg == WM_SYSDEADCHAR)
    {
        return HandleKey(hWnd, msg, wParam, lParam);
    }
    else if (msg == WM_ERASEBKGND)
    {
        return HandleEraseBackground(hWnd, msg, wParam, lParam);
    }
    else if (msg == WM_WINDOWPOSCHANGING)
    {
        return HandlePosChanging(hWnd, msg, wParam, lParam);
    }
    else if (msg == WM_WINDOWPOSCHANGED)
    {
        return HandlePosChanged(hWnd, msg, wParam, lParam);
    }
    else if (msg == WM_DPICHANGED)
    {
        return HandleDpiChanged(hWnd, msg, wParam, lParam);
    }
    else if (msg == WM_CLOSE)
    {
        return HandleClose(hWnd, msg, wParam, lParam);
    }
    else if (msg == WM_DESTROY || msg == WM_NCDESTROY)
    {
        return HandleDestroy(hWnd, msg, wParam, lParam);
    }
    else if (msg == WM_NCPOINTERDOWN || msg == WM_NCPOINTERUP || msg == WM_NCPOINTERUPDATE)
    {
        return HandleNcPointer(hWnd, msg, wParam, lParam);
    }
    else if (
      msg == WM_POINTERDOWN || msg == WM_POINTERUP || msg == WM_POINTERUPDATE || //
      msg == WM_POINTERENTER || msg == WM_POINTERLEAVE ||                        //
      msg == WM_POINTERWHEEL || msg == WM_POINTERHWHEEL ||                       //
      msg == WM_POINTERCAPTURECHANGED ||                                         //
      msg == WM_POINTERDEVICECHANGE || msg == WM_POINTERDEVICEINRANGE)
    {
        return HandlePointer(hWnd, msg, wParam, lParam);
    }
    else if (msg == WM_NCHITTEST)
    {
        return HandleNcHitTest(hWnd, msg, wParam, lParam);
    }
    else if (msg == WM_NCMOUSEHOVER || msg == WM_NCMOUSEHOVER || msg == WM_NCMOUSELEAVE || msg == WM_NCLBUTTONDOWN || msg == WM_NCLBUTTONDBLCLK || msg == WM_NCLBUTTONUP)
    {
        return HandleNcMouse(hWnd, msg, wParam, lParam);
    }
    else if ((WM_MOUSEFIRST <= msg && msg <= WM_MOUSELAST) || msg == WM_MOUSEHOVER || msg == WM_MOUSELEAVE)
    {
        return HandleMouse(hWnd, msg, wParam, lParam);
    }
    else if (msg == WM_CAPTURECHANGED)
    {
        return HandleCaptureChanged(hWnd, msg, wParam, lParam);
    }
    else if (msg == WM_INPUTLANGCHANGE)
    {
        // TODO: Use ITfLanguageProfileNotifySink instead.
        return HandleInputLangChange(hWnd, msg, wParam, lParam);
    }
    else if (
      msg == WM_IME_STARTCOMPOSITION || //
      msg == WM_IME_ENDCOMPOSITION ||   //
      msg == WM_IME_COMPOSITION ||      //
      msg == WM_IME_SETCONTEXT ||       //
      msg == WM_IME_NOTIFY ||           //
      msg == WM_IME_CONTROL ||          //
      msg == WM_IME_COMPOSITIONFULL ||  //
      msg == WM_IME_SELECT ||           //
      msg == WM_IME_CHAR ||             //
      msg == WM_IME_REQUEST ||          //
      msg == WM_IME_KEYDOWN ||          //
      msg == WM_IME_KEYUP)
    {
        return HandleInputMethod(hWnd, msg, wParam, lParam);
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief Handle WM_CREATE
///
auto PlatformWindowWin::HandleCreate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    if (msg == WM_CREATE)
    {
        if (_options.backgroundStyle == WindowBackgroundStyle::System)
        {
            // Enable Win11 Mica effect.
            // WS_EX_NOREDIRECTIONBITMAP is required to remove standard background.
            const auto systemBackdrop = DWMSBT_MAINWINDOW;
            if (FAILED(::DwmSetWindowAttribute(hWnd, DWMWA_SYSTEMBACKDROP_TYPE, &systemBackdrop, sizeof(systemBackdrop))))
            {
                FW_DEBUG_LOG_ERROR("PlatformWindowWin: Failed to set DWMWA_SYSTEMBACKDROP_TYPE");
            }
        }
        else if (_options.backgroundStyle == WindowBackgroundStyle::Solid)
        {
            // Make color of title bar match background.
            const auto r = static_cast<BYTE>(_backgroundColor.GetRedU8());
            const auto g = static_cast<BYTE>(_backgroundColor.GetGreenU8());
            const auto b = static_cast<BYTE>(_backgroundColor.GetBlueU8());
            const auto color = RGB(r, g, b);
            if (FAILED(::DwmSetWindowAttribute(hWnd, DWMWA_CAPTION_COLOR, &color, sizeof(color))))
            {
                FW_DEBUG_LOG_ERROR("PlatformWindowWin: Failed to set DWMWA_CAPTION_COLOR");
            }
        }

        if (_options.hasFrame)
        {
            // Popups are not eligible to automatic rounding when not resizable.
            if (HasWindowStyle(WS_POPUP))
            {
                const auto cornerRadius = DWMWCP_ROUNDSMALL;
                if (FAILED(::DwmSetWindowAttribute(hWnd, DWMWA_WINDOW_CORNER_PREFERENCE, &cornerRadius, sizeof(cornerRadius))))
                {
                    FW_DEBUG_LOG_ERROR("PlatformWindowWin: Failed to set DWMWA_WINDOW_CORNER_PREFERENCE");
                }
            }
        }
        else
        {
            const auto borderColor = DWMWA_COLOR_NONE;
            if (FAILED(::DwmSetWindowAttribute(hWnd, DWMWA_BORDER_COLOR, &borderColor, sizeof(borderColor))))
            {
                FW_DEBUG_LOG_ERROR("PlatformWindowWin: Failed to set DWMWA_BORDER_COLOR");
            }
            const auto cornerRadius = DWMWCP_DONOTROUND;
            if (FAILED(::DwmSetWindowAttribute(hWnd, DWMWA_WINDOW_CORNER_PREFERENCE, &cornerRadius, sizeof(cornerRadius))))
            {
                FW_DEBUG_LOG_ERROR("PlatformWindowWin: Failed to set DWMWA_WINDOW_CORNER_PREFERENCE");
            }
        }

        // Set initial theme color.
        UpdateThemeColor();

        // Force WM_NCCALCSIZE to be sent right away.
        // Sending WM_ACTIVATE here will cause WM_SETFOCUS before first WM_SHOWWINDOW, which breaks TSF based IME handling.
        ::SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOMOVE | SWP_NOSIZE | SWP_NOSENDCHANGING);

        // Initialize IME state.
        _textStore = _context->GetInputMethodContext().MakeTextStore(
          {
              .sendKeyEventDetached = [&](Event<>& event) { return SendKeyEventDetached(event); },
          },
          hWnd);
        _inputMethod = Shared<PlatformInputMethodWin>::Make();
        _inputMethod->SetTextStore(_textStore);

        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief Handle WM_NCCALCSIZE
///
auto PlatformWindowWin::HandleNcCalcSize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    if (msg == WM_NCCALCSIZE)
    {
        if (wParam == TRUE)
        {
            if (_options.hasFrame)
            {
                if (auto ncSizeParams = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam))
                {
                    auto& rect = ncSizeParams->rgrc[0];

                    // Set minimum amount of insets to make window border visible.
                    if (_options.hasTitleBar || !HasWindowStyle(WS_POPUP))
                    {
                        // Adding top inset breaks system controls in title bar.
                        const auto x = GetSystemFrameThicknessX();
                        const auto y = GetSystemFrameThicknessY();
                        rect.left += x;
                        rect.right -= x;
                        rect.bottom -= y;
                    }
                    else
                    {
                        auto const length = static_cast<int>(Vp::Round(UnitFunction::ConvertDpToVp(1, GetDisplayScale())));
                        ::InflateRect(&rect, -length, -length);
                    }
                }
                return WVR_REDRAW;
            }
            return 0;
        }
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief Handle WM_NCACTIVATE
///
auto PlatformWindowWin::HandleNcActivate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    FW_DEBUG_LOG_INFO("PlatformWindowWin::HandleNcActivate(): {}", wParam);
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief Handle WM_NCHITTEST
///
auto PlatformWindowWin::HandleNcHitTest(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    if (msg == WM_NCHITTEST)
    {
        // Let DWM handle system controls.
        auto dwmResult = LRESULT();
        if (::DwmDefWindowProc(hWnd, msg, wParam, lParam, &dwmResult))
        {
            return dwmResult;
        }

        const auto frameX = GetSystemFrameThicknessX() * 2;
        const auto frameY = GetSystemFrameThicknessY() * 2;

        const auto x = LOWORD(lParam);
        const auto y = HIWORD(lParam);

        auto frameRect = RECT();
        if (::GetWindowRect(hWnd, &frameRect))
        {
            auto yPosition = 0;
            if ((y - frameRect.top) <= frameY)
            {
                yPosition = -1;
            }
            else if ((frameRect.bottom - y) <= frameY)
            {
                yPosition = 1;
            }

            auto xPosition = 0;
            if ((x - frameRect.left) <= frameX)
            {
                xPosition = -1;
            }
            else if ((frameRect.right - x) <= frameX)
            {
                xPosition = 1;
            }

            if (yPosition == -1)
            {
                if (xPosition == -1)
                {
                    return HTTOPLEFT;
                }
                else if (xPosition == 0)
                {
                    return HTTOP;
                }
                else if (xPosition == 1)
                {
                    return HTTOPRIGHT;
                }
            }
            else if (yPosition == 0)
            {
                if (xPosition == -1)
                {
                    return HTLEFT;
                }
                else if (xPosition == 1)
                {
                    return HTRIGHT;
                }
            }
            else if (yPosition == 1)
            {
                if (xPosition == -1)
                {
                    return HTBOTTOMLEFT;
                }
                else if (xPosition == 0)
                {
                    return HTBOTTOM;
                }
                else if (xPosition == 1)
                {
                    return HTBOTTOMRIGHT;
                }
            }
        }

        if (_options.hasTitleBar)
        {
            if ((y - frameRect.top) <= GetSystemTitleBarHeight())
            {
                auto const dpX = UnitFunction::ConvertVpToDp(x - frameRect.left, GetDisplayScale());
                auto const dpY = UnitFunction::ConvertVpToDp(y - frameRect.top, GetDisplayScale());
                auto hitTestEvent = Event<PlatformHitTestEvent>();
                hitTestEvent->SetPosition({dpX, dpY});
                auto event = Event<>(hitTestEvent);
                if (SendHitTestEventDetached(event))
                {
                    if (event.Is<PlatformHitTestEvent>())
                    {
                        if (event.As<PlatformHitTestEvent>()->GetHit())
                        {
                            return HTCLIENT;
                        }
                    }
                }
                return HTCAPTION;
            }
        }
        return HTCLIENT;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief Handle WM_NCMOUSE*
///
auto PlatformWindowWin::HandleNcMouse(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    auto dwmResult = LRESULT();
    if (::DwmDefWindowProc(hWnd, msg, wParam, lParam, &dwmResult))
    {
        return dwmResult;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief Handle WM_NCPOINTER*
///
auto PlatformWindowWin::HandleNcPointer(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    auto dwmResult = LRESULT();
    if (::DwmDefWindowProc(hWnd, msg, wParam, lParam, &dwmResult))
    {
        return dwmResult;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief Handle WM_GETMINMAXINFO.
///
auto PlatformWindowWin::HandleGetMinMaxInfo(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    if (msg == WM_GETMINMAXINFO)
    {
        if (const auto info = reinterpret_cast<MINMAXINFO*>(lParam))
        {
            const auto displayScale = GetDisplayScale();
            const auto minSize = Size<Vp>::Max(UnitFunction::ConvertDpToVp(_sizeConstraints.GetMinSize(), displayScale), GetSystemMinWindowSize());
            const auto maxSize = Size<Vp>::Min(UnitFunction::ConvertDpToVp(_sizeConstraints.GetMaxSize(), displayScale), GetSystemMaxWindowSize());

            info->ptMinTrackSize.x = static_cast<LONG>(Vp::Ceil(minSize.width));
            info->ptMinTrackSize.y = static_cast<LONG>(Vp::Ceil(minSize.height));

            info->ptMaxTrackSize.x = static_cast<LONG>(Vp::Floor(maxSize.width));
            info->ptMaxTrackSize.y = static_cast<LONG>(Vp::Floor(maxSize.height));
        }
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

static auto IsOwnerOf(HWND wnd, HWND target) -> Bool
{
    HWND parent = target;
    while (parent)
    {
        if (parent == wnd)
        {
            return true;
        }
        parent = ::GetWindow(parent, GW_OWNER);
    }
    return false;
}

static auto GetWindowPointer(HWND wnd) -> Pointer<PlatformWindowWin>
{
    const auto data = ::GetWindowLongPtrW(wnd, GWLP_USERDATA);
    return reinterpret_cast<PlatformWindowWin*>(data);
}

///
/// @brief Handle WM_ACTIVATE.
///
auto PlatformWindowWin::HandleActivate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    if (msg == WM_ACTIVATE)
    {
        try
        {
            FW_DEBUG_LOG_TRACE("PlatformWindowWin::HandleActivate(): {} {:x} <-> {:x}", LOWORD(wParam), (LPARAM)hWnd, lParam);
            FW_DEBUG_LOG_TRACE("PlatformWindowWin::HandleActivate(): active={:x} ", (LPARAM)::GetActiveWindow());

            if (!_destructing)
            {
                auto windowEvent = Event<PlatformWindowEvent::ActiveChanged>();
                windowEvent->SetActive(LOWORD(wParam) != WA_INACTIVE);
                auto event = Event<>(std::move(windowEvent));
                SendWindowEventDetached(event);
            }

            // Destroy popup chain when unrelated window is activated.
            // TODO: Consider moving this logic to Window so that we can share it with other platforms.
            if (LOWORD(wParam) == WA_INACTIVE)
            {
                if (_options.behavior == WindowBehavior::Popup)
                {
                    // If owner window is activated, destroying popup will break activation flow and WM_ACTIVATE will not be sent to the owner.
                    // So we only destroy popup chain when unrelated window is activated here.
                    auto const activated = reinterpret_cast<HWND>(lParam);
                    if (!IsOwnerOf(hWnd, activated) && !IsOwnerOf(activated, hWnd))
                    {
                        FW_DEBUG_LOG_TRACE("PlatformWindowWin::HandleActivate(): Unrelated window activated, destroying popup chain.");
                        auto parent = hWnd;
                        while (parent)
                        {
                            auto const window = GetWindowPointer(parent);
                            if (!window || window->_options.behavior != WindowBehavior::Popup)
                            {
                                break;
                            }
                            FW_DEBUG_LOG_TRACE("PlatformWindowWin::HandleActivate(): DestroyWindow({:x})", (uintptr_t)parent);
                            ::DestroyWindow(std::exchange(parent, ::GetWindow(parent, GW_OWNER)));
                        }
                        FW_DEBUG_LOG_TRACE("PlatformWindowWin::HandleActivate(): Destroyed popup chain.");
                    }
                }
            }
            else
            {
                // Destroy owned popup chain after owner window is activated.
                auto const deactivated = reinterpret_cast<HWND>(lParam);
                if (IsOwnerOf(hWnd, deactivated))
                {
                    FW_DEBUG_LOG_TRACE("PlatformWindowWin::HandleActivate(): Owner window activated, destroying popup chain.");
                    HWND parent = deactivated;
                    while (parent)
                    {
                        auto const window = GetWindowPointer(parent);
                        if (!window || window->_options.behavior != WindowBehavior::Popup)
                        {
                            break;
                        }

                        if (parent == hWnd)
                        {
                            break;
                        }
                        auto newParent = ::GetWindow(parent, GW_OWNER);
                        auto oldParent = std::exchange(parent, newParent);
                        if (newParent && oldParent)
                        {
                            if (auto newParentWindow = GetWindowPointer(newParent))
                            {
                                if (newParentWindow->_options.behavior == WindowBehavior::Popup)
                                {
                                    if (window->_options.allowActiveOwnerPopup)
                                    {
                                        continue;
                                    }
                                }
                            }
                        }
                        FW_DEBUG_LOG_TRACE("PlatformWindowWin::HandleActivate(): DestroyWindow({:x})", (uintptr_t)oldParent);
                        ::DestroyWindow(oldParent);
                    }
                    FW_DEBUG_LOG_TRACE("PlatformWindowWin::HandleActivate(): Destroyed popup chain.");
                }
            }
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
    }
    // Sets keyboard focus to the window.
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief Handle WM_ACTIVATE.
///
auto PlatformWindowWin::HandleActivateApp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    if (msg == WM_ACTIVATEAPP)
    {
        if (auto context = Locator::GetInstance<PlatformApplicationContext>().As<PlatformApplicationContextWin>())
        {
            if (auto application = context->GetCurrentApplication())
            {
                const auto active = static_cast<bool>(wParam);
                application->SetActive(active);
            }
        }
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief Handle WM_SHOWWINDOW.
///
auto PlatformWindowWin::HandleShow(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    if (msg == WM_SHOWWINDOW)
    {
        if (wParam)
        {
            if (_options.behavior == WindowBehavior::Dialog)
            {
                if (const auto owner = _options.owner.TryAs<PlatformWindowWin>())
                {
                    owner->DisableWindow(GetSelf());
                }
            }
        }

        try
        {
            auto windowEvent = Event<PlatformWindowEvent::VisibleChanged>();
            windowEvent->SetVisible(wParam != FALSE);
            auto event = Event<>(std::move(windowEvent));
            SendWindowEventDetached(event);
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief Handle WM_DPICHANGED.
///
auto PlatformWindowWin::HandleDpiChanged(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    if (msg == WM_DPICHANGED)
    {
        auto const yDpi = HIWORD(wParam);
        if (_dpi != yDpi)
        {
            _dpi = yDpi;

            try
            {
                if (_rootViewLayer)
                {
                    _rootViewLayer->NotifyRootChanged();
                }
                auto event = Event<>(Event<PlatformWindowEvent::DisplayScaleChanged>());
                SendWindowEventDetached(event);
            }
            catch (...)
            {
                FW_DEBUG_ASSERT(false);
            }
        }

        if (const auto rect = reinterpret_cast<RECT const*>(lParam))
        {
            auto const x = rect->left;
            auto const y = rect->top;
            auto const w = rect->right - rect->left;
            auto const h = rect->bottom - rect->top;
            ::SetWindowPos(hWnd, NULL, x, y, w, h, SWP_NOOWNERZORDER | SWP_NOACTIVATE);
        }
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief Handle WM_POSCHANGING.
///
auto PlatformWindowWin::HandlePosChanging(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief Handle WM_WINDOWPOSCHANGED
///
auto PlatformWindowWin::HandlePosChanged(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    if (msg == WM_WINDOWPOSCHANGED)
    {
        const auto windowPos = reinterpret_cast<WINDOWPOS const*>(lParam);

        if (_options.hasTitleBar)
        {
            // Extend title bar into client area. This enables us to draw over title bar.
            // Several observations:
            // * Setting margin other than MARGINS::cyTopHeight creates redirection bitmap which cannot be removed by WM_NOREDIRECTIONBITMAP.
            //   * cyTopHeight can be added without breaking WM_NOREDIRECTIONBITMAP.
            const auto margins = MARGINS {0, 0, GetSystemTitleBarHeight(), 0};
            if (FAILED(::DwmExtendFrameIntoClientArea(hWnd, &margins)))
            {
                FW_DEBUG_LOG_ERROR("PlatformWindowWin: DwmExtendFrameIntoClientArea failed");
            }
        }

        if ((windowPos->flags & SWP_NOMOVE) == 0)
        {
            try
            {
                auto event = Event<>(Event<PlatformWindowEvent::PositionChanged>());
                SendWindowEventDetached(event);
            }
            catch (...)
            {
                FW_DEBUG_ASSERT(false);
            }
        }

        if ((windowPos->flags & SWP_NOSIZE) == 0)
        {
            try
            {
                // Update geometry of system controls in title bar.
                // NOTE: DWM may return stalte value on DWMWA_CAPTION_BUTTON_BOUNDS immediately after resizing.
                RefreshSystemControlRect();
                {
                    auto event = Event<>(Event<PlatformWindowEvent::SizeChanged>());
                    SendWindowEventDetached(event);
                }
                // Force a new frame so that we can catch up with the new state of DWM in next layout pass.
                RequestFrame();
            }
            catch (...)
            {
                FW_DEBUG_ASSERT(false);
            }
        }
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief Handle WM_CLOSE.
///
auto PlatformWindowWin::HandleClose(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    if (msg == WM_CLOSE)
    {
        try
        {
            auto event = Event<>(Event<PlatformWindowEvent::CloseRequested>());
            auto result = Shared<Optional<Bool>>::Make();
            AsyncFunction::SpawnFn([&event, result, self = GetSelf()]() mutable -> Task<void> {
                try
                {
                    *result = co_await self->SendWindowEvent(event);
                }
                catch (...)
                {
                    FW_DEBUG_ASSERT(false);
                }
            }).Detach();

            auto quit = Optional<WPARAM>();
            while (true)
            {
                if (result)
                {
                    break;
                }

                auto loopMsg = MSG();
                if (::GetMessageW(&loopMsg, NULL, 0, 0) == -1)
                {
                    FW_DEBUG_ASSERT(false);
                    continue;
                }

                if (loopMsg.message == WM_QUIT)
                {
                    quit = loopMsg.wParam;
                    continue;
                }
                ::TranslateMessage(&loopMsg);
                ::DispatchMessageW(&loopMsg);
            }

            if (quit)
            {
                ::PostQuitMessage(static_cast<int>(*quit));
            }

            auto close = True;
            if (event.Is<PlatformWindowEvent::CloseRequested>())
            {
                if (event.As<PlatformWindowEvent::CloseRequested>()->IsCancelled())
                {
                    close = false;
                }
            }

            if (close)
            {
                ::DestroyWindow(hWnd);
            }
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief
///
auto PlatformWindowWin::HandleDestroy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    if (msg == WM_DESTROY)
    {
        // WM_DESTROY is sent before the child windows are destroyed.
        return 0;
    }
    else if (msg == WM_NCDESTROY)
    {
        _closed = true;

        try
        {
            if (const auto owner = _options.owner.TryAs<PlatformWindowWin>())
            {
                owner->RemoveOwnedWindow(GetSelf());
            }

            // Avoid sending event from destructor.
            if (!_destructing)
            {
                auto event = Event<>(Event<PlatformWindowEvent::Closed>());

                if (_closing)
                {
                    PostWindowEvent(event);
                }
                else
                {
                    SendWindowEventDetached(event);
                }
            }
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief Handle WM_POINTER*
///
auto PlatformWindowWin::HandlePointer(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    (void)lParam;
    (void)hWnd;

    try
    {
        if (msg == WM_POINTERDOWN)
        {
            PointerEnter(true, wParam);
            PointerDown(true, wParam);
        }
        else if (msg == WM_POINTERUP)
        {
            PointerDown(false, wParam);
        }
        else if (msg == WM_POINTERUPDATE)
        {
            PointerUpdate(wParam);
        }
        else if (msg == WM_POINTERENTER)
        {
            PointerUpdate(wParam);
        }
        else if (msg == WM_POINTERLEAVE)
        {
            PointerEnter(false, wParam);
        }
        else if (msg == WM_POINTERWHEEL)
        {
            PointerWheel(true, wParam);
        }
        else if (msg == WM_POINTERHWHEEL)
        {
            PointerWheel(false, wParam);
        }
        else if (msg == WM_POINTERCAPTURECHANGED)
        {
            PointerCancel(wParam);
        }
        else if (msg == WM_POINTERDEVICECHANGE)
        {
            // TODO.
        }
        else if (msg == WM_POINTERDEVICEINRANGE)
        {
            // TODO.
        }
        return 0;
    }
    catch (...)
    {
        FW_DEBUG_ASSERT(false);
        return 0;
    }
}

///
/// @brief Handle WM_NCMOUSE*
///
auto PlatformWindowWin::HandleMouse(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    if (msg == WM_MOUSEMOVE)
    {
        if (!_mouseEnter)
        {
            FW_DEBUG_LOG_INFO("PlatformWindowWin::HandleMouse(): Motion::Enter");
            _mouseEnter = true;
            auto parameter = Event<PlatformPointerEvent::Motion::Enter>();
            PlatformPointerEventFunctionWin::SetPointerEventParamsForMouse(*parameter, hWnd, msg, wParam, lParam);
            auto event = Event<>(parameter);
            SendPointerEventDetached(event);
        }
        {
            FW_DEBUG_LOG_INFO("PlatformWindowWin::HandleMouse(): Motion::Move");
            auto parameter = Event<PlatformPointerEvent::Motion::Move>();
            PlatformPointerEventFunctionWin::SetPointerEventParamsForMouse(*parameter, hWnd, msg, wParam, lParam);
            PlatformPointerEventFunctionWin::SetPointerMotionEventParamsForMouse(*parameter, msg, wParam, lParam);
            auto event = Event<>(parameter);
            SendPointerEventDetached(event);
        }

        auto tme = TRACKMOUSEEVENT {
            .cbSize = sizeof(TRACKMOUSEEVENT),
            .dwFlags = TME_LEAVE,
            .hwndTrack = hWnd,
            .dwHoverTime = 0,
        };
        ::TrackMouseEvent(&tme);
    }
    else if (msg == WM_MOUSELEAVE)
    {
        if (_mouseEnter)
        {
            FW_DEBUG_LOG_INFO("PlatformWindowWin::HandleMouse(): Motion::Leave");
            _mouseEnter = false;
            auto parameter = Event<PlatformPointerEvent::Motion::Leave>();
            PlatformPointerEventFunctionWin::SetPointerEventParamsForMouse(*parameter, hWnd, msg, wParam, lParam);
            auto event = Event<>(parameter);
            SendPointerEventDetached(event);
        }
    }
    else if (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_XBUTTONDOWN)
    {
        FW_DEBUG_LOG_INFO("PlatformWindowWin::HandleMouse(): Motion::Down");
        auto parameter = Event<PlatformPointerEvent::Motion::Down>();
        PlatformPointerEventFunctionWin::SetPointerEventParamsForMouse(*parameter, hWnd, msg, wParam, lParam);
        PlatformPointerEventFunctionWin::SetPointerMotionEventParamsForMouse(*parameter, msg, wParam, lParam);
        auto event = Event<>(parameter);
        SendPointerEventDetached(event);
    }
    else if (msg == WM_LBUTTONUP || msg == WM_RBUTTONUP || msg == WM_MBUTTONUP || msg == WM_XBUTTONUP)
    {
        FW_DEBUG_LOG_INFO("PlatformWindowWin::HandleMouse(): Motion::Up");
        auto parameter = Event<PlatformPointerEvent::Motion::Up>();
        PlatformPointerEventFunctionWin::SetPointerEventParamsForMouse(*parameter, hWnd, msg, wParam, lParam);
        PlatformPointerEventFunctionWin::SetPointerMotionEventParamsForMouse(*parameter, msg, wParam, lParam);
        auto event = Event<>(parameter);
        SendPointerEventDetached(event);
    }
    else if (msg == WM_MOUSEWHEEL || msg == WM_MOUSEHWHEEL)
    {
        FW_DEBUG_LOG_INFO("PlatformWindowWin::HandleMouse(): Motion::Wheel");
        auto parameter = Event<PlatformPointerEvent::Action::Scroll>();
        PlatformPointerEventFunctionWin::SetPointerEventParamsForMouse(*parameter, hWnd, msg, wParam, lParam);
        PlatformPointerEventFunctionWin::SetPointerScrollEventParamsForMouse(*parameter, msg, wParam, lParam);
        auto event = Event<>(parameter);
        SendPointerEventDetached(event);
    }
    return 0;
}

auto PlatformWindowWin::HandleCaptureChanged(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    if (msg == WM_CAPTURECHANGED)
    {
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

auto PlatformWindowWin::HandleKey(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    // https://learn.microsoft.com/en-us/windows/win32/inputdev/about-keyboard-input

    if (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN)
    {
        // We want to generate Unicode representation of pressed key here, but WM_KEYDOWN/WM_CHAR model doesn't allow us to do this directly.
        // Currently we uses two methods to implement this:
        // 1. Use PeekMessage() to retrieve WM_*CHAR message posted by TranslateMessage().
        // 2. Build lookup table by using ToUnicodeEx() and map virtual key code into printable characters.

        auto const virtualKeyCode = static_cast<DWORD>(wParam);
        auto const keyFlags = HIWORD(lParam);
        auto const isRepeat = (keyFlags & KF_REPEAT) != 0;
        auto const eventCount = LOWORD(lParam);

        auto& keyboardLayout = _context->GetKeyboardLayout();

        auto const modifiers = keyboardLayout.GetModifierState();
        auto const [key, text] = keyboardLayout.GetKeyAndText(hWnd, virtualKeyCode, keyboardLayout.GetPlatformModifierState());
        auto const unmodifiedKey = keyboardLayout.GetUnmodifiedKey(virtualKeyCode);
        auto const timestamp = MonotonicClock::GetNow();

        if (_textStore)
        {
            _textStore->CancelProcessKeyDown();
        }

        for (auto i = 0; i < eventCount; ++i)
        {
            try
            {
                auto parameter = Event<PlatformKeyEvent::Down>();
                parameter->SetKey(key);
                parameter->SetUnmodifiedKey(unmodifiedKey);
                parameter->SetText(text);
                parameter->SetModifiers(modifiers);
                parameter->SetRepeat(isRepeat);
                parameter->SetTimestamp(timestamp);

                auto event = Event<>(std::move(parameter));
                SendKeyEventDetached(event);
            }
            catch (...)
            {
                FW_DEBUG_ASSERT(false);
            }

            try
            {
                if (text.IsEmpty())
                {
                    if (_textStore)
                    {
                        _textStore->InputKeyFromKeyEvent(key);
                    }
                }
                else if (key != Key::Dead)
                {
                    if (_textStore)
                    {
                        _textStore->InsertTextFromKeyEvent(text);
                    }
                }
            }
            catch (...)
            {
                FW_DEBUG_ASSERT(false);
            }
        }
        return 0;
    }
    else if (msg == WM_KEYUP || msg == WM_SYSKEYUP)
    {
        try
        {
            auto const virtualKeyCode = static_cast<DWORD>(wParam);

            auto& keyboardLayout = _context->GetKeyboardLayout();

            auto const key = keyboardLayout.GetKey(virtualKeyCode, keyboardLayout.GetPlatformModifierState());
            auto const unmodifiedKey = keyboardLayout.GetUnmodifiedKey(virtualKeyCode);

            auto parameter = Event<PlatformKeyEvent::Up>();
            parameter->SetKey(key);
            parameter->SetUnmodifiedKey(unmodifiedKey);
            parameter->SetTimestamp(MonotonicClock::GetNow());
            auto event = Event<>(std::move(parameter));
            SendKeyEventDetached(event);
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
        return 0;
    }
    else if (msg == WM_CHAR || msg == WM_SYSCHAR || msg == WM_DEADCHAR || msg == WM_SYSDEADCHAR)
    {
        FW_DEBUG_LOG_WARNING("PlatformWindowWin: Unhandled WM_(SYS)CHAR/WM_(SYS)DEADCHAR");
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief Handle WM_IME*
///
auto PlatformWindowWin::HandleInputMethod(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    // TODO: Should we handle this for TSF-unaware IMEs?
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief Handle WM_ERASEBKGND
///
auto PlatformWindowWin::HandleEraseBackground(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    if (msg == WM_ERASEBKGND)
    {
        if (_options.backgroundStyle == WindowBackgroundStyle::Solid)
        {
            const auto dc = ::GetDC(hWnd);

            auto clientRect = RECT();
            ::GetClientRect(hWnd, &clientRect);

            const auto titleBarHeight = _options.hasTitleBar ? GetSystemTitleBarHeight() : 0;

            if (titleBarHeight > 0)
            {
                const auto rect = RECT {
                    .left = clientRect.left,
                    .top = clientRect.top,
                    .right = clientRect.right,
                    .bottom = clientRect.top + titleBarHeight,
                };
                const auto brush = static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH));
                ::FillRect(dc, &rect, brush);
            }
            {
                const auto rect = RECT {
                    .left = clientRect.left,
                    .top = clientRect.top + titleBarHeight,
                    .right = clientRect.right,
                    .bottom = clientRect.bottom,
                };
                const auto r = static_cast<BYTE>(_backgroundColor.GetRedU8());
                const auto g = static_cast<BYTE>(_backgroundColor.GetGreenU8());
                const auto b = static_cast<BYTE>(_backgroundColor.GetBlueU8());
                const auto color = RGB(r, g, b);
                const auto brush = ::CreateSolidBrush(color);
                ::FillRect(dc, &rect, brush);
                ::DeleteObject(brush);
            }
            return 1;
        }
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief Handle WM_SETFOCUS
///
auto PlatformWindowWin::HandleSetFocus(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    if (msg == WM_SETFOCUS)
    {
        try
        {
            auto windowEvent = Event<PlatformWindowEvent::FocusedChanged>();
            windowEvent->SetFocused(true);
            auto event = Event<>(std::move(windowEvent));
            SendWindowEventDetached(event);
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief Handle WM_KILLFOCUS
///
auto PlatformWindowWin::HandleKillFocus(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    if (msg == WM_KILLFOCUS)
    {
        try
        {
            auto windowEvent = Event<PlatformWindowEvent::FocusedChanged>();
            windowEvent->SetFocused(false);
            auto event = Event<>(std::move(windowEvent));
            SendWindowEventDetached(event);
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief Handle WM_INPUTLANGCHANGE
///
auto PlatformWindowWin::HandleInputLangChange(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    if (msg == WM_INPUTLANGCHANGE)
    {
        FW_DEBUG_LOG_INFO("PlatformWindowWin::HandleInputLangChange()");
        if (_context)
        {
            _context->GetKeyboardLayout().UpdateLayout();
        }
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

///
/// @brief Handle frame swap event.
///
/// @param targetTimestamp
///
auto PlatformWindowWin::HandleFrameSwap(MonotonicTime const& targetTimestamp) -> void
{
    try
    {
        // Refresh metrics which might have changed since last frame.
        RefreshSystemControlRect();

        auto parameter = Event<PlatformFrameEvent::Tick>();
        parameter->SetTargetTimestamp(targetTimestamp);
        auto event = Event<>(parameter);
        SendFrameEventDetached(event);
    }
    catch (...)
    {
        FW_DEBUG_ASSERT(false);
    }
}

///
/// @brief
///
auto PlatformWindowWin::GetSystemMinWindowSize() const -> Size<Vp>
{
    const auto w = ::GetSystemMetricsForDpi(SM_CXMINTRACK, _dpi);
    const auto h = ::GetSystemMetricsForDpi(SM_CYMINTRACK, _dpi);
    return Size<Vp>(w, h);
}

///
/// @brief
///
auto PlatformWindowWin::GetSystemMaxWindowSize() const -> Size<Vp>
{
    const auto w = ::GetSystemMetricsForDpi(SM_CXMAXTRACK, _dpi);
    const auto h = ::GetSystemMetricsForDpi(SM_CYMAXTRACK, _dpi);
    return Size<Vp>(w, h);
}

///
/// @brief Get system frame thickness.
///
/// @note Does not include padding thickness (SM_CXPADDEDBORDER).
///
auto PlatformWindowWin::GetSystemFrameThicknessX() const -> int
{
    return ::GetSystemMetricsForDpi(SM_CXFRAME, _dpi);
}

///
/// @brief Get system frame thickness.
///
/// @note Does not include padding thickness (SM_CYPADDEDBORDER).
///
auto PlatformWindowWin::GetSystemFrameThicknessY() const -> int
{
    return ::GetSystemMetricsForDpi(SM_CYFRAME, _dpi);
}

///
/// @brief Get rectangle of system control buttons.
///
auto PlatformWindowWin::GetSystemControlRect() const -> Rect<Vp>
{
    return _systemControlRect;
}

///
/// @brief
///
auto PlatformWindowWin::RefreshSystemControlRect() -> void
{
    if (::IsWindowVisible(_hwnd) && !::IsIconic(_hwnd))
    {
        auto rect = RECT();
        if (SUCCEEDED(::DwmGetWindowAttribute(_hwnd, DWMWA_CAPTION_BUTTON_BOUNDS, &rect, sizeof(rect))))
        {
            auto const systemControlRect = Rect<Vp>(rect.left, rect.top, rect.right, rect.bottom);
            if (_systemControlRect != systemControlRect)
            {
                _systemControlRect = systemControlRect;

                try
                {
                    auto event = Event<>(Event<PlatformWindowEvent::AreaChanged>());
                    SendWindowEventDetached(event);
                }
                catch (...)
                {
                    FW_DEBUG_ASSERT(false);
                }
            }
        }
    }
}

///
/// @brief
///
/// @return
///
auto PlatformWindowWin::GetSystemTitleBarHeight() const -> int
{
    return ::GetSystemMetricsForDpi(SM_CYFRAME, _dpi) + ::GetSystemMetricsForDpi(SM_CYCAPTION, _dpi) + ::GetSystemMetricsForDpi(SM_CXPADDEDBORDER, _dpi);
}

///
/// @brief
///
auto PlatformWindowWin::HasWindowStyle(DWORD style) const -> Bool
{
    auto const styles = static_cast<DWORD>(::GetWindowLongW(_hwnd, GWL_STYLE));
    return (style & styles) == style;
}

///
/// @brief
///
/// @return
///
auto PlatformWindowWin::GetSelf() -> Shared<PlatformWindowWin>
{
    return _self.Lock();
}

///
/// @brief
///
auto PlatformWindowWin::SetSelf(Shared<PlatformWindowWin> const& self) -> void
{
    _self = self;
}

///
/// @brief
///
/// @param down
/// @param wParam
///
void PlatformWindowWin::PointerDown(Bool const down, WPARAM const wParam)
{
    auto const pointerId = GET_POINTERID_WPARAM(wParam);

    auto it = std::find_if(_pointerStates.begin(), _pointerStates.end(), [&](const auto& state) { return state.id == pointerId; });
    if (it == _pointerStates.end())
    {
        it = _pointerStates.insert(
          _pointerStates.end(),
          PointerState {
              .id = pointerId,
          });
    }

    auto& state = *it;

    if (state.down != down)
    {
        state.down = down;

        if (down)
        {
            auto parameter = Event<PlatformPointerEvent::Motion::Down>();
            PlatformPointerEventFunctionWin::SetPointerEventParamsForPointer(*parameter, pointerId, _hwnd);
            PlatformPointerEventFunctionWin::SetPointerMotionEventParamsForPointer(*parameter, pointerId);
            auto event = Event<>(parameter);
            SendPointerEventDetached(event);
        }
        else
        {
            auto parameter = Event<PlatformPointerEvent::Motion::Up>();
            PlatformPointerEventFunctionWin::SetPointerEventParamsForPointer(*parameter, pointerId, _hwnd);
            PlatformPointerEventFunctionWin::SetPointerMotionEventParamsForPointer(*parameter, pointerId);
            auto event = Event<>(parameter);
            SendPointerEventDetached(event);
        }
    }
}

///
/// @brief
///
/// @param enter
/// @param wParam
///
void PlatformWindowWin::PointerEnter(Bool const enter, WPARAM const wParam)
{
    const auto pointerId = GET_POINTERID_WPARAM(wParam);

    auto it = std::find_if(_pointerStates.begin(), _pointerStates.end(), [&](const auto& state) { return state.id == pointerId; });
    if (it == _pointerStates.end())
    {
        it = _pointerStates.insert(
          _pointerStates.end(),
          PointerState {
              .id = pointerId,
          });
    }

    auto& state = *it;

    if (state.enter != enter)
    {
        state.enter = enter;

        if (enter)
        {
            auto parameter = Event<PlatformPointerEvent::Motion::Enter>();
            PlatformPointerEventFunctionWin::SetPointerEventParamsForPointer(*parameter, pointerId, _hwnd);
            auto event = Event<>(parameter);
            SendPointerEventDetached(event);
        }
        else
        {
            auto parameter = Event<PlatformPointerEvent::Motion::Leave>();
            PlatformPointerEventFunctionWin::SetPointerEventParamsForPointer(*parameter, pointerId, _hwnd);
            auto event = Event<>(parameter);
            SendPointerEventDetached(event);
        }
    }
}

///
/// @brief
///
/// @param wParam
///
void PlatformWindowWin::PointerUpdate(WPARAM const wParam)
{
    const auto pointerId = GET_POINTERID_WPARAM(wParam);

    if (IS_POINTER_CANCELED_WPARAM(wParam))
    {
        const auto it = std::find_if(_pointerStates.begin(), _pointerStates.end(), [&](const auto& state) { return state.id == pointerId; });
        if (it != _pointerStates.end())
        {
            _pointerStates.erase(it);
        }

        auto parameter = Event<PlatformPointerEvent::Motion::Cancel>();
        PlatformPointerEventFunctionWin::SetPointerEventParamsForPointer(*parameter, pointerId, _hwnd);
        auto event = Event<>(parameter);
        SendPointerEventDetached(event);
        return;
    }

    PointerEnter(true, wParam);

    auto parameter = Event<PlatformPointerEvent::Motion::Move>();
    PlatformPointerEventFunctionWin::SetPointerEventParamsForPointer(*parameter, pointerId, _hwnd);
    PlatformPointerEventFunctionWin::SetPointerMotionEventParamsForPointer(*parameter, pointerId);
    auto event = Event<>(parameter);
    SendPointerEventDetached(event);
}

///
/// @brief
///
auto PlatformWindowWin::PointerCancel(WPARAM const wParam) -> void
{
    (void)wParam;
    auto event = Event<>(Event<PlatformPointerEvent::Motion::Cancel>());
    SendPointerEventDetached(event);
}

///
/// @brief
///
/// @param vertical
/// @param wParam
///
auto PlatformWindowWin::PointerWheel(Bool const vertical, WPARAM const wParam) -> void
{
    auto const delta = GET_WHEEL_DELTA_WPARAM(wParam);
    auto const pointerId = GET_POINTERID_WPARAM(wParam);

    auto pointerInfo = POINTER_INFO();
    if (!::GetPointerInfo(pointerId, &pointerInfo))
    {
        FW_DEBUG_LOG_ERROR("PlatformWindowWin: GetPointerInfo failed");
        return;
    }

    auto const precision = pointerInfo.pointerType == PT_TOUCHPAD ? PointerScrollPrecision::Precise : PointerScrollPrecision::Coarse;
    auto const deltaX = vertical ? 0 : delta / WHEEL_DELTA;
    auto const deltaY = vertical ? delta / WHEEL_DELTA : 0;

    auto parameter = Event<PlatformPointerEvent::Action::Scroll>();
    PlatformPointerEventFunctionWin::SetPointerEventParamsForPointer(*parameter, pointerId, _hwnd);
    parameter->SetPrecision(precision);
    parameter->SetDeltaY(deltaY);
    parameter->SetDeltaX(deltaX);
    auto event = Event<>(parameter);
    SendPointerEventDetached(event);
}

///
/// @brief
///
auto PlatformWindowWin::PostWindowEvent(Event<> const& event) -> void
{
    auto weakSelf = Weak(GetSelf());
    AsyncFunction::SpawnFn([=]() -> Task<void> {
        try
        {
            co_await MainThread::Schedule();

            if (const auto self = weakSelf.Lock())
            {
                auto e = event;
                co_await self->SendWindowEvent(e);
            }
        }
        catch (...)
        {
            FW_DEBUG_ASSERT(false);
        }
    }).Detach();
}

///
/// @brief
///
auto PlatformWindowWin::DisableWindow(Shared<PlatformWindowWin> const& source) -> void
{
    const auto self = GetSelf();

    for (const auto& owned : _ownedWindows)
    {
        if (const auto ownedWindow = owned.Lock())
        {
            if (ownedWindow != source)
            {
                ownedWindow->DisableWindow(self);
            }
        }
    }

    FW_DEBUG_LOG_INFO("PlatformWindowWin::DisableWindow()");
    ::EnableWindow(_hwnd, FALSE);

    if (_options.behavior != WindowBehavior::Dialog)
    {
        const auto owner = _options.owner.TryAs<PlatformWindowWin>();
        if (owner && owner != source)
        {
            owner->DisableWindow(self);
        }
    }
}

///
/// @brief
///
auto PlatformWindowWin::EnableWindow(Shared<PlatformWindowWin> const& source) -> void
{
    const auto self = GetSelf();

    if (_options.behavior != WindowBehavior::Dialog)
    {
        const auto owner = _options.owner.TryAs<PlatformWindowWin>();
        if (owner && owner != source)
        {
            owner->EnableWindow(self);
        }
    }

    FW_DEBUG_LOG_INFO("PlatformWindowWin::EnableWindow()");
    ::EnableWindow(_hwnd, TRUE);

    for (const auto& owned : _ownedWindows)
    {
        if (const auto ownedWindow = owned.Lock())
        {
            if (ownedWindow != source)
            {
                ownedWindow->EnableWindow(self);
            }
        }
    }
}

///
/// @brief 
///
auto PlatformWindowWin::UpdateThemeColor() -> void
{
    if (_theme)
    {
        auto const isDarkMode = _theme->GetSystemBrightness() == ThemeBrightness::Dark;
        const auto useImmersiveDarkMode = BOOL(isDarkMode);
        if (FAILED(::DwmSetWindowAttribute(_hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useImmersiveDarkMode, sizeof(useImmersiveDarkMode))))
        {
            FW_DEBUG_LOG_ERROR("PlatformWindowWin: Failed to enable DWMA_USE_IMMERSIVE_DARK_MODE");
        }
    }
}
}

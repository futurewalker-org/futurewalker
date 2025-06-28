// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.Win.PlatformWindowWin.hpp"
#include "Futurewalker.Application.Win.PlatformWindowContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformRootViewLayerWin.hpp"
#include "Futurewalker.Application.Win.PlatformKeyboardLayoutWin.hpp"
#include "Futurewalker.Application.PlatformWindowEvent.hpp"
#include "Futurewalker.Application.PlatformFrameEvent.hpp"
#include "Futurewalker.Application.PlatformPointerEvent.hpp"
#include "Futurewalker.Application.PlatformKeyEvent.hpp"
#include "Futurewalker.Application.PlatformInputEvent.hpp"
#include "Futurewalker.Application.Win.PlatformApplicationContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformApplicationWin.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodTextStoreWin.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodWin.hpp"
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
auto PlatformWindowWin::Make(Shared<PlatformWindowContextWin> context, Shared<PlatformDCompositionDeviceWin> compositionDevice, PlatformWindowOptions const& options, Delegate const& delegate)
  -> Shared<PlatformWindowWin>
{
    auto window = PlatformWindow::MakeDerived<PlatformWindowWin>(context, compositionDevice, options, delegate);
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
  Shared<PlatformDCompositionDeviceWin> compositionDevice,
  PlatformWindowOptions const& options,
  Delegate const& delegate)
  : PlatformWindow(key, delegate)
  , _context {context}
  , _compositionDevice {compositionDevice}
  , _options {options}
{
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
        ::ShowWindow(_hwnd, visible ? SW_SHOW : SW_HIDE);
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
auto PlatformWindowWin::GetSize() -> Size<Dp>
{
    if (!IsClosed())
    {
        auto rect = RECT();
        if (::GetWindowRect(_hwnd, &rect))
        {
            const auto size = Size<Vp>(rect.right - rect.left, rect.bottom - rect.top);
            return UnitFunction::ConvertVpToDp(size, GetDisplayScale());
        }
    }
    return {};
}

///
/// @brief 
///
/// @param frameRect 
///
auto PlatformWindowWin::SetSize(Size<Dp> const& size) -> void
{
    if (!IsClosed())
    {
        const auto spSize = UnitFunction::ConvertDpToVp(size, GetDisplayScale());
        const auto w = static_cast<int>(Vp::Round(spSize.GetWidth()));
        const auto h = static_cast<int>(Vp::Round(spSize.GetHeight()));
        ::SetWindowPos(_hwnd, NULL, 0, 0, w, h, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE);
    }
}

///
/// @brief 
///
auto PlatformWindowWin::GetPosition() -> Point<Dp>
{
    if (!IsClosed())
    {
        auto rect = RECT();
        if (::GetWindowRect(_hwnd, &rect))
        {
            const auto position = Point<Vp>(rect.left, rect.right);
            return UnitFunction::ConvertVpToDp(position, GetDisplayScale());
        }
    }
    return {};
}

///
/// @brief 
///
/// @param contentRect 
///
auto PlatformWindowWin::SetPosition(Point<Dp> const& position) -> void
{
    if (!IsClosed())
    {
        const auto spPosition = UnitFunction::ConvertDpToVp(position, GetDisplayScale());
        const auto x = static_cast<int>(Vp::Round(spPosition.GetX()));
        const auto y = static_cast<int>(Vp::Round(spPosition.GetY()));
        ::SetWindowPos(_hwnd, NULL, x, y, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOSIZE);
    }
}

///
/// @brief
///
auto PlatformWindowWin::GetFrameRect()  -> Rect<Dp>
{
    if (!IsClosed())
    {
        auto rect = RECT();
        if (::GetWindowRect(_hwnd, &rect))
        {
            const auto frameRect = Rect<Vp>(rect.left, rect.top, rect.right, rect.bottom);
            return UnitFunction::ConvertVpToDp(frameRect, GetDisplayScale());
        }
    }
    return {};
}

///
/// @brief
///
auto PlatformWindowWin::GetContentRect() -> Rect<Dp>
{
    if (!IsClosed())
    {
        auto windowRect = RECT();
        if (::GetWindowRect(_hwnd, &windowRect))
        {
            auto origin = POINT();
            if (::ClientToScreen(_hwnd, &origin))
            {
                auto rect = RECT();
                if (::GetClientRect(_hwnd, &rect))
                {
                    const auto x = origin.x - windowRect.left;
                    const auto y = origin.y - windowRect.top;
                    const auto contentRect = Rect<Vp>(x, y, x + rect.right - rect.left, y + rect.bottom - rect.top);
                    return UnitFunction::ConvertVpToDp(contentRect, GetDisplayScale());
                }
            }
        }
    }
    return {};
}

///
/// @brief
///
auto PlatformWindowWin::GetAreaRect(WindowArea const area) -> Rect<Dp>
{
    if (!IsClosed())
    {
        auto rect = RECT();
        if (!::GetClientRect(_hwnd, &rect))
        {
            return {};
        }

        const auto width = rect.right - rect.left;
        const auto height = rect.bottom - rect.top;

        if (area == WindowArea::TitleBar)
        {
            if (_options.hasTitleBar)
            {
                const auto titleBarHeight = GetSystemTitleBarHeight();
                const auto titleBarRect = Rect<Vp>(0, 0, width, titleBarHeight);
                return UnitFunction::ConvertVpToDp(titleBarRect, GetDisplayScale());
            }
        }
        else if (area == WindowArea::TitleBarContent)
        {
            if (_options.hasTitleBar)
            {
                const auto titleBarHeight = GetSystemTitleBarHeight();
                const auto controlRect = GetSystemControlRect();
                const auto availableRect = Rect<Vp>(0, 0, controlRect.left, titleBarHeight);
                return UnitFunction::ConvertVpToDp(availableRect, GetDisplayScale());
            }
        }
        else if (area == WindowArea::Content)
        {
            const auto titleBarHeight = _options.hasTitleBar ? GetSystemTitleBarHeight() : 0;
            const auto contentRect = Rect<Vp>(0, titleBarHeight, width, height);
            return UnitFunction::ConvertVpToDp(contentRect, GetDisplayScale());
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
auto PlatformWindowWin::GetTitle() const -> String
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
auto PlatformWindowWin::Restore() -> void
{
    if (!IsClosed())
    {
        ::ShowWindow(_hwnd, SW_NORMAL);
    }
}

///
/// @brief
///
auto PlatformWindowWin::Close() -> void
{
    if (!IsClosed())
    {
        auto resetter = ScopedResetter(_closing = true);
        Destroy();
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
                if (const auto window = data.Assume<PlatformWindowWin>())
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
auto PlatformWindowWin::GetFrameTime() const -> MonotonicTime
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
    else if (
      msg == WM_NCPOINTERDOWN || msg == WM_NCPOINTERUP || msg == WM_NCPOINTERUPDATE || //
      msg == WM_POINTERDOWN || msg == WM_POINTERUP || msg == WM_POINTERUPDATE ||       //
      msg == WM_POINTERENTER || msg == WM_POINTERLEAVE ||                              //
      msg == WM_POINTERWHEEL || msg == WM_POINTERHWHEEL ||                             //
      msg == WM_POINTERCAPTURECHANGED ||                                               //
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
        return HandleMouse(hWnd, msg, wParam, lParam);
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
        // Change color of titlebar based on user settings.
        const auto useImmersiveDarkMode = TRUE;
        if (FAILED(::DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useImmersiveDarkMode, sizeof(useImmersiveDarkMode))))
        {
            FW_DEBUG_LOG_ERROR("PlatformWindowWin: Failed to enable DWMA_USE_IMMERSIVE_DARK_MODE");
        }

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
            const auto r = static_cast<BYTE>(_options.backgroundColor.GetRedU8());
            const auto g = static_cast<BYTE>(_options.backgroundColor.GetGreenU8());
            const auto b = static_cast<BYTE>(_options.backgroundColor.GetBlueU8());
            const auto color = RGB(r, g, b);
            if (FAILED(::DwmSetWindowAttribute(hWnd, DWMWA_CAPTION_COLOR, &color, sizeof(color))))
            {
                FW_DEBUG_LOG_ERROR("PlatformWindowWin: Failed to set DWMWA_CAPTION_COLOR");
            }
        }

        if (!_options.hasFrame)
        {
            const auto borderColor = DWMWA_COLOR_NONE;
            if (FAILED(::DwmSetWindowAttribute(hWnd, DWMWA_BORDER_COLOR, &borderColor, sizeof(borderColor))))
            {
                FW_DEBUG_LOG_ERROR("PlatformWindowWin: Failed to set DWMWA_BORDER_COLOR");
            }
            const auto cornerRadius = DWMWCP_DONOTROUND;
            if (FAILED(::DwmSetWindowAttribute(hWnd, DWMWA_WINDOW_CORNER_PREFERENCE, &cornerRadius, sizeof(cornerRadius))))
            {
                FW_DEBUG_LOG_ERROR("PlatformWindowWin: Failed to set DWMWA_WINDOW_CORNER_REFERENCE");
            }
        }

        // Force WM_NCCALCSIZE to be sent right away.
        // Sending WM_ACTIVATE here will cause WM_SETFOCUS before first WM_SHOWWINDOW, which breaks TSF based IME handling.
        ::SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOMOVE | SWP_NOSIZE | SWP_NOSENDCHANGING);

        // Initialize IME state.
        _textStore = _context->GetInputMethodContext().MakeTextStore(hWnd);
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
        FW_DEBUG_LOG_INFO("PlatformWindowWin::HandleNcCalcSize()");

        if (wParam == TRUE)
        {
            const auto x = GetSystemFrameThicknessX();
            const auto y = GetSystemFrameThicknessY();
            if (auto ncSizeParams = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam))
            {
                // Set minimum amount of insets to make window border visible.
                auto& rect = ncSizeParams->rgrc[0];
                rect.left += x;
                rect.right -= x;
                rect.bottom -= y;
            }
            return WVR_REDRAW;
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
                return HTCAPTION;
            }
        }
        return HTCLIENT;
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

            info->ptMinTrackSize.x = static_cast<LONG>(Vp::Ceil(minSize.GetWidth()));
            info->ptMinTrackSize.y = static_cast<LONG>(Vp::Ceil(minSize.GetHeight()));

            info->ptMaxTrackSize.x = static_cast<LONG>(Vp::Floor(maxSize.GetWidth()));
            info->ptMaxTrackSize.y = static_cast<LONG>(Vp::Floor(maxSize.GetHeight()));
        }
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
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
            auto event = Event<>(Event<PlatformWindowEvent::ActiveChanged>());
            SendWindowEventDetached(event);
        }
        catch(...)
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
                if (const auto owner = _options.owner.Maybe<PlatformWindowWin>())
                {
                    owner->DisableWindow(GetSelf());
                }
            }
        }

        try
        {
            auto event = Event<>(Event<PlatformWindowEvent::VisibleChanged>());
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
        if (_dpi != HIWORD(wParam))
        {
            _dpi = HIWORD(wParam);

            try
            {
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
            ::SetWindowPos(hWnd, NULL, rect->left, rect->top, rect->right, rect->bottom, SWP_NOOWNERZORDER | SWP_NOACTIVATE);
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
                {
                    auto event = Event<>(Event<PlatformWindowEvent::AreaChanged>());
                    SendWindowEventDetached(event);
                }
                {
                    auto event = Event<>(Event<PlatformWindowEvent::SizeChanged>());
                    SendWindowEventDetached(event);
                }

                if (_compositionDevice)
                {
                    _compositionDevice->Commit();
                }
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
            if (const auto owner = _options.owner.Maybe<PlatformWindowWin>())
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
/// @brief
///
static void SetPointerInfoParams(PlatformPointerEvent& parameter, POINTER_INFO const& info, HWND const hwnd)
{
    parameter.SetPointerId(PointerId(info.pointerId));
    parameter.SetPrimaryPointer((info.pointerFlags & POINTER_FLAG_PRIMARY) != POINTER_FLAG_NONE);

    if (info.pointerType == PT_MOUSE)
    {
        parameter.SetPointerType(PointerType::Mouse);
    }
    else if (info.pointerType == PT_PEN)
    {
        parameter.SetPointerType(PointerType::Pen);
    }
    else if (info.pointerType == PT_TOUCH)
    {
        parameter.SetPointerType(PointerType::Touch);
    }
    else if (info.pointerType == PT_TOUCHPAD)
    {
        parameter.SetPointerType(PointerType::TouchPad);
    }

    {
        auto frequency = LARGE_INTEGER();
        ::QueryPerformanceFrequency(&frequency);

        auto timestamp = info.PerformanceCount;
        if (timestamp == 0)
        {
            auto tickCount = LARGE_INTEGER();
            ::QueryPerformanceCounter(&tickCount);
            timestamp = tickCount.QuadPart;
        }
        parameter.SetTimestamp(static_cast<float64_t>(timestamp) / frequency.QuadPart);
    }
    {
        auto rect = RECT();
        if (::GetWindowRect(hwnd, &rect))
        {
            const auto displayScale = ::GetDpiForWindow(hwnd) / DisplayScale(USER_DEFAULT_SCREEN_DPI);
            const auto pos = info.ptPixelLocation;
            parameter.SetPosition(UnitFunction::ConvertVpToDp(Point<Vp>(pos.x - rect.left, pos.y - rect.top), displayScale));
        }
    }
}

///
/// @brief
///
static void SetPointerEventParams(PlatformPointerEvent::Motion& parameter, UINT32 const pointerId, HWND const hwnd)
{
    POINTER_INPUT_TYPE inputType = PT_POINTER;
    if (!::GetPointerType(pointerId, &inputType))
    {
        FW_DEBUG_LOG_ERROR("PlatformWindowWin: GetPointerType failed");
        return;
    }

    if (inputType == PT_PEN)
    {
        POINTER_PEN_INFO info {};
        if (!::GetPointerPenInfo(pointerId, &info))
        {
            FW_DEBUG_LOG_ERROR("PlatformWindowWin: GetPointerPenInfo failed");
            return;
        }
        SetPointerInfoParams(parameter, info.pointerInfo, hwnd);
        const auto [azimuth, altitude] = PlatformPointerEvent::ConvertTiltToSpherical(info.tiltX, info.tiltY);
        parameter.SetTwist(info.rotation);
        parameter.SetTiltX(info.tiltX);
        parameter.SetTiltY(info.tiltY);
        parameter.SetAzimuth(azimuth);
        parameter.SetAltitude(altitude);
    }
    else if (inputType == PT_TOUCH)
    {
        POINTER_TOUCH_INFO info {};
        if (!::GetPointerTouchInfo(pointerId, &info))
        {
            FW_DEBUG_LOG_ERROR("PlatformWindowWin: GetPointerTouchInfo failed");
            return;
        }
        SetPointerInfoParams(parameter, info.pointerInfo, hwnd);
    }
    else
    {
        POINTER_INFO info {};
        if (!::GetPointerInfo(pointerId, &info))
        {
            FW_DEBUG_LOG_ERROR("PlatformWindowWin: GetPointerInfo failed");
            return;
        }
        SetPointerInfoParams(parameter, info, hwnd);
    }
}

///
/// @brief Handle WM_POINTER*
///
auto PlatformWindowWin::HandlePointer(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    try
    {
        if (msg == WM_NCPOINTERDOWN)
        {
            PointerEnter(true, wParam);
            PointerDown(true, wParam);
        }
        else if (msg == WM_NCPOINTERUP)
        {
            PointerDown(false, wParam);
        }
        else if (msg == WM_NCPOINTERUPDATE)
        {
            PointerUpdate(wParam);
        }
        else if (msg == WM_POINTERDOWN)
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
            // TODO.
        }
        else if (msg == WM_POINTERHWHEEL)
        {
            // TODO.
        }
        else if (msg == WM_POINTERCAPTURECHANGED)
        {
            // TODO.
        }
        else if (msg == WM_POINTERDEVICECHANGE)
        {
            // TODO.
        }
        else if (msg == WM_POINTERDEVICEINRANGE)
        {
            // TODO.
        }

        auto dwmResult = LRESULT();
        if (::DwmDefWindowProc(hWnd, msg, wParam, lParam, &dwmResult))
        {
            return dwmResult;
        }
        return ::DefWindowProcW(hWnd, msg, wParam, lParam);
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
    auto dwmResult = LRESULT();
    if (::DwmDefWindowProc(hWnd, msg, wParam, lParam, &dwmResult))
    {
        return dwmResult;
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

                auto event = Event(parameter);
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
            auto parameter = Event<PlatformKeyEvent::Up>();
            parameter->SetTimestamp(MonotonicClock::GetNow());
            auto event = Event(parameter);
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
/// @brief Handle WM_DISPLAYCHANGE
///
auto PlatformWindowWin::HandleDisplayChange(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    if (msg == WM_DISPLAYCHANGE)
    {
        if (_context)
        {
            _context->RefreshScreens();
        }
    }
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
                const auto r = static_cast<BYTE>(_options.backgroundColor.GetRedU8());
                const auto g = static_cast<BYTE>(_options.backgroundColor.GetGreenU8());
                const auto b = static_cast<BYTE>(_options.backgroundColor.GetBlueU8());
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
            auto event = Event<>(Event<PlatformWindowEvent::FocusedChanged>());
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
            auto event = Event<>(Event<PlatformWindowEvent::FocusedChanged>());
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
        auto parameter = Event<PlatformFrameEvent::Tick>();
        parameter->SetTargetTimestamp(targetTimestamp);
        auto event = Event(parameter);
        SendFrameEventDetached(event);

        if (_compositionDevice)
        {
            _compositionDevice->Commit();
        }
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
auto PlatformWindowWin::GetSystemFrameThicknessX() -> int
{
    return ::GetSystemMetricsForDpi(SM_CXFRAME, _dpi);
}

///
/// @brief Get system frame thickness.
///
/// @note Does not include padding thickness (SM_CYPADDEDBORDER).
///
auto PlatformWindowWin::GetSystemFrameThicknessY() -> int
{
    return ::GetSystemMetricsForDpi(SM_CYFRAME, _dpi);
}

///
/// @brief Get rectangle of system control buttons.
///
auto PlatformWindowWin::GetSystemControlRect() -> RECT
{
    auto systemControlRect = RECT();
    if (SUCCEEDED(::DwmGetWindowAttribute(_hwnd, DWMWA_CAPTION_BUTTON_BOUNDS, &systemControlRect, sizeof(systemControlRect))))
    {
        return systemControlRect;
    }
    return {};
}

///
/// @brief
///
/// @return
///
auto PlatformWindowWin::GetSystemTitleBarHeight() -> int
{
    return ::GetSystemMetricsForDpi(SM_CYFRAME, _dpi) + ::GetSystemMetricsForDpi(SM_CYCAPTION, _dpi) + ::GetSystemMetricsForDpi(SM_CXPADDEDBORDER, _dpi);
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
            SetPointerEventParams(*parameter, pointerId, _hwnd);
            auto event = Event(parameter);
            SendPointerEventDetached(event);
        }
        else
        {
            auto parameter = Event<PlatformPointerEvent::Motion::Up>();
            SetPointerEventParams(*parameter, pointerId, _hwnd);
            auto event = Event(parameter);
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
            SetPointerEventParams(*parameter, pointerId, _hwnd);
            auto event = Event(parameter);
            SendPointerEventDetached(event);
        }
        else
        {
            auto parameter = Event<PlatformPointerEvent::Motion::Leave>();
            SetPointerEventParams(*parameter, pointerId, _hwnd);
            auto event = Event(parameter);
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
        SetPointerEventParams(*parameter, pointerId, _hwnd);
        auto event = Event(parameter);
        SendPointerEventDetached(event);
        return;
    }

    PointerEnter(true, wParam);

    auto parameter = Event<PlatformPointerEvent::Motion::Move>();
    SetPointerEventParams(*parameter, pointerId, _hwnd);
    auto event = Event(parameter);
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
    });
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

    ::EnableWindow(_hwnd, FALSE);

    if (_options.behavior != WindowBehavior::Dialog)
    {
        const auto owner = _options.owner.Maybe<PlatformWindowWin>();
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
        const auto owner = _options.owner.Maybe<PlatformWindowWin>();
        if (owner && owner != source)
        {
            owner->EnableWindow(self);
        }
    }

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
auto PlatformWindowWin::Destroy() -> void
{
    if (_hwnd)
    {
        if (const auto owner = _options.owner.Maybe<PlatformWindowWin>())
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
}

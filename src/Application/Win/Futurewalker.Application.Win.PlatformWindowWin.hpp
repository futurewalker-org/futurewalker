// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformWindowWinType.hpp" 
#include "Futurewalker.Application.Win.PlatformWindowContextWinType.hpp"
#include "Futurewalker.Application.Win.PlatformRootViewLayerWinType.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodTextStoreWinType.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodWinType.hpp"
#include "Futurewalker.Application.PlatformWindow.hpp"
#include "Futurewalker.Application.PlatformApplicationThemeContextType.hpp"
#include "Futurewalker.Application.PlatformApplicationThemeType.hpp"

#include "Futurewalker.Graphics.Win.PlatformDCompositionDeviceWinType.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
class PlatformWindowWin : public PlatformWindow
{
public:
    static auto Make(
      Shared<PlatformWindowContextWin> context,
      Shared<PlatformApplicationThemeContext> themeContext,
      Shared<PlatformDCompositionDeviceWin> compositionDevice,
      PlatformWindowOptions const& options,
      Delegate const& delegate) -> Shared<PlatformWindowWin>;

    PlatformWindowWin(
      PassKey<PlatformWindow> key,
      Shared<PlatformWindowContextWin> context,
      Shared<PlatformApplicationThemeContext> themeContext,
      Shared<PlatformDCompositionDeviceWin> compositionDevice,
      PlatformWindowOptions const& options,
      Delegate const& delegate);

    ~PlatformWindowWin();

    auto GetContext() -> Shared<PlatformWindowContextWin>;
    auto GetNativeHandle() const -> HWND;
    auto GetOptions() const -> PlatformWindowOptions;

    auto AddOwnedWindow(Shared<PlatformWindowWin> const& window) -> void;
    auto RemoveOwnedWindow(Shared<PlatformWindowWin> const& window) -> void;
    auto GetOwnedWindows() -> const std::vector<Weak<PlatformWindowWin>>&;

public:
    auto IsVisible() -> Bool override;
    auto SetVisible(Bool visible) -> void override;
    auto IsActive() -> Bool override;
    auto SetActive() -> void override;
    auto IsFocused() -> Bool override;
    auto SetFocus() -> void override;
    auto GetFrameRect() -> Rect<Vp> override;
    auto SetFrameRect(Rect<Vp> const& rect) -> void override;
    auto GetRestoredFrameRect() -> Rect<Vp> override;
    auto SetRestoredFrameRect(Rect<Vp> const& rect) -> void override;
    auto GetAreaBounds(WindowArea const area) -> std::vector<Rect<Dp>> override;
    auto GetAreaInsets(WindowArea const area) -> EdgeInsets override;
    auto GetDisplayScale() -> DisplayScale override;
    auto GetBackingScale() -> BackingScale override;
    auto GetSizeConstraints() -> BoxConstraints override;
    auto SetSizeConstraints(BoxConstraints const& constraints) -> void override;
    auto GetTitle() -> String override;
    auto SetTitle(String const& title) -> void override;
    auto Minimize() -> void override;
    auto Maximize() -> void override;
    auto Restore() -> void override;
    auto IsClosed() -> Bool override;
    auto RequestClose() -> Async<Bool> override;
    auto Destroy() -> void override;
    auto Render() -> void override;
    auto RequestFrame() -> void override;
    auto GetFrameTime() -> MonotonicTime override;
    auto CapturePointer(PointerId const id) -> void override;
    auto ReleasePointer(PointerId const id) -> void override;
    auto GetViewLayer() -> Shared<PlatformViewLayer> override;
    auto GetInputMethod() -> Shared<PlatformInputMethod> override;
    auto SetBackgroundColor(RGBColor const& backgroundColor) -> void override;

public:
    auto SetNativeHandle(PassKey<PlatformWindowContextWin>, HWND hwnd) -> void;
    auto WindowProcedure(PassKey<PlatformWindowContextWin>, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT;

protected:
    auto Initialize() -> void override;
    auto ReceiveThemeEvent(Event<>& event) -> Async<Bool>;

private:
    auto HandleCreate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleNcCalcSize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleNcActivate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleNcHitTest(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleNcMouse(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleNcPointer(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleGetMinMaxInfo(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleActivate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleActivateApp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleShow(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleDpiChanged(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandlePosChanging(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandlePosChanged(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleClose(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleDestroy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandlePointer(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleMouse(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleCaptureChanged(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleKey(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleKeyTraceDown(WPARAM wParam, LPARAM lParam) -> void;
    auto HandleInputMethod(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleEraseBackground(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleSetFocus(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleKillFocus(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleInputLangChange(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleFrameSwap(MonotonicTime const& targetTimestamp) -> void;

    auto GetSystemMinWindowSize() const -> Size<Vp>;
    auto GetSystemMaxWindowSize() const -> Size<Vp>;
    
    auto GetSystemFrameThicknessX() const -> int;
    auto GetSystemFrameThicknessY() const -> int;
    auto GetSystemTitleBarHeight() const -> int;
    auto GetSystemControlRect() const -> Rect<Vp>;
    auto RefreshSystemControlRect() -> void;

    auto HasWindowStyle(DWORD style) const -> Bool;

    auto GetSelf() -> Shared<PlatformWindowWin>;
    auto SetSelf(Shared<PlatformWindowWin> const& self) -> void;

    auto PointerDown(Bool const down, WPARAM const wParam) -> void;
    auto PointerEnter(Bool const enter, WPARAM const wParam) -> void;
    auto PointerUpdate(WPARAM const wParam) -> void;
    auto PointerCancel(WPARAM const wParam) -> void;
    auto PointerWheel(Bool const vertical, WPARAM const wParam) -> void;

    auto PostWindowEvent(Event<> const& event) -> void;

    auto DisableWindow(Shared<PlatformWindowWin> const& source) -> void;
    auto EnableWindow(Shared<PlatformWindowWin> const& source) -> void;

    auto UpdateThemeColor() -> void;

private:
    Weak<PlatformWindowWin> _self;
    Shared<PlatformWindowContextWin> _context;
    Shared<PlatformApplicationTheme> _theme;
    Shared<PlatformDCompositionDeviceWin> _compositionDevice;
    Shared<PlatformRootViewLayerWin> _rootViewLayer;
    Shared<PlatformInputMethodTextStoreWin> _textStore;
    Shared<PlatformInputMethodWin> _inputMethod;
    PlatformWindowOptions _options;
    std::vector<Weak<PlatformWindowWin>> _ownedWindows;
    HWND _hwnd = NULL;
    UINT _dpi = USER_DEFAULT_SCREEN_DPI;
    String _title;
    RGBColor _backgroundColor;
    BoxConstraints _sizeConstraints = BoxConstraints::MakeUnconstrained();
    Bool _destructing = false;
    Bool _closed = false;
    Rect<Vp> _systemControlRect;
    Bool _restoreOnShow = false;
    Bool _mouseEnter = false;

private:
    struct PointerState
    {
        UINT32 id = 0;
        Bool down = false;
        Bool enter = false;
    };
    std::vector<PointerState> _pointerStates;
};
}
}

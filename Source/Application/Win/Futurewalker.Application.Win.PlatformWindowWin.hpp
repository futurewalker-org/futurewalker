// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.Win.PlatformWindowWinType.hpp" 
#include "Futurewalker.Application.Win.PlatformWindowContextWinType.hpp"
#include "Futurewalker.Application.Win.PlatformRootViewLayerWinType.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodTextStoreWinType.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodWinType.hpp"
#include "Futurewalker.Application.Win.PlatformDCompositionDeviceWinType.hpp"

#include "Futurewalker.Application.PlatformWindow.hpp"

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
    static auto Make(Shared<PlatformWindowContextWin> context, Shared<PlatformDCompositionDeviceWin> compositionDevice, PlatformWindowOptions const& options, Delegate const& delegate)
      -> Shared<PlatformWindowWin>;

    PlatformWindowWin(
      PassKey<PlatformWindowWin>,
      Shared<PlatformWindowContextWin> context,
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
    auto GetSize() -> Size<Dp> override;
    auto SetSize(Size<Dp> const& size) -> void override;
    auto GetPosition() -> Point<Dp> override;
    auto SetPosition(Point<Dp> const& position) -> void override;
    auto GetFrameRect() -> Rect<Dp> override;
    auto GetContentRect() -> Rect<Dp> override;
    auto GetAreaRect(WindowArea const area) -> Rect<Dp> override;
    auto GetDisplayScale() -> DisplayScale override;
    auto GetBackingScale() -> BackingScale override;
    auto GetSizeConstraints() -> BoxConstraints override;
    auto SetSizeConstraints(BoxConstraints const& constraints) -> void override;
    auto GetTitle() const -> String override;
    auto SetTitle(String const& title) -> void override;
    auto Minimize() -> void override;
    auto Maximize() -> void override;
    auto Restore() -> void override;
    auto IsClosed() -> Bool override;
    auto Close() -> void override;
    auto RequestFrame() -> void override;
    auto GetFrameTime() const -> MonotonicTime override;
    auto GetViewLayer() -> Shared<PlatformViewLayer> override;
    auto GetInputMethod() -> Shared<PlatformInputMethod> override;

public:
    auto SetNativeHandle(PassKey<PlatformWindowContextWin>, HWND hwnd) -> void;
    auto WindowProcedure(PassKey<PlatformWindowContextWin>, HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT;

private:
    auto HandleCreate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleNcCalcSize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleNcActivate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleNcHitTest(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
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
    auto HandleKey(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleInputMethod(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleDisplayChange(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleEraseBackground(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleSetFocus(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleKillFocus(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleInputLangChange(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto HandleFrameSwap(MonotonicTime const& targetTimestamp) -> void;

    auto GetSystemMinWindowSize() const -> Size<Sp>;
    auto GetSystemMaxWindowSize() const -> Size<Sp>;
    
    auto GetSystemFrameThicknessX() -> int;
    auto GetSystemFrameThicknessY() -> int;
    auto GetSystemTitleBarHeight() -> int;
    auto GetSystemControlRect() -> RECT;

    auto GetSelf() -> Shared<PlatformWindowWin>;
    auto SetSelf(Shared<PlatformWindowWin> const& self) -> void;

    auto PointerDown(Bool const down, WPARAM const wParam) -> void;
    auto PointerEnter(Bool const enter, WPARAM const wParam) -> void;
    auto PointerUpdate(WPARAM const wParam) -> void;

    auto PostWindowEvent(Event const& event) -> void;

    auto DisableWindow(Shared<PlatformWindowWin> const& source) -> void;
    auto EnableWindow(Shared<PlatformWindowWin> const& source) -> void;

    auto Destroy() -> void;

private:
    Weak<PlatformWindowWin> _self;
    Shared<PlatformWindowContextWin> _context;
    Shared<PlatformDCompositionDeviceWin> _compositionDevice;
    Shared<PlatformRootViewLayerWin> _rootViewLayer;
    Shared<PlatformInputMethodTextStoreWin> _textStore;
    Shared<PlatformInputMethodWin> _inputMethod;
    PlatformWindowOptions _options;
    std::vector<Weak<PlatformWindowWin>> _ownedWindows;
    HWND _hwnd = NULL;
    UINT _dpi = USER_DEFAULT_SCREEN_DPI;
    String _title;
    BoxConstraints _sizeConstraints = BoxConstraints::MakeUnconstrained();
    Bool _destructing = false;
    Bool _closing = false;
    Bool _closed = false;

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

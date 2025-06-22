// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformWindowContextWin.hpp"
#include "Futurewalker.Application.PlatformWindowContext.hpp"
#include "Futurewalker.Application.PlatformScreenContext.hpp"
#include "Futurewalker.Application.Win.PlatformWindowWinType.hpp"
#include "Futurewalker.Application.Win.PlatformDCompositionDeviceWin.hpp"
#include "Futurewalker.Application.Win.PlatformVsyncProviderWinType.hpp"
#include "Futurewalker.Application.Win.PlatformKeyboardLayoutWinType.hpp"
#include "Futurewalker.Application.Win.PlatformInputMethodContextWinType.hpp"

#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Win.PlatformInstanceHandleWin.hpp"

#include "Futurewalker.Core.PassKey.hpp"

#include <windows.h>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
class PlatformWindowContextWin : public PlatformWindowContext
{
public:
    static auto Make(
      Shared<PlatformInstanceHandleWin> instanceHandle,
      Shared<PlatformInputMethodContextWin> inputMethodContext,
      Shared<PlatformDCompositionDeviceWin> compositionDevice,
      Shared<PlatformVsyncProviderWin> vsyncProvider,
      Shared<PlatformScreenContext> screenContext) -> Shared<PlatformWindowContextWin>;

    PlatformWindowContextWin(
      PassKey<PlatformWindowContextWin>,
      Shared<PlatformInstanceHandleWin> instanceHandle,
      Shared<PlatformInputMethodContextWin> inputMethodContext,
      Shared<PlatformDCompositionDeviceWin> compositionDevice,
      Shared<PlatformVsyncProviderWin> vsyncProvider,
      Shared<PlatformScreenContext> screenContext);

    ~PlatformWindowContextWin();

    auto MakePlatformWindow(PlatformWindowOptions const& options, PlatformWindow::Delegate const& delegate) -> Shared<PlatformWindow> override;

    auto InitializeWindow(Shared<PlatformWindowWin> const& window, PlatformWindowOptions const& options) -> void;

    auto RequestFrame(Weak<PlatformWindowWin> window, PlatformVsyncCallbackFunction callback) -> void;
    auto GetFrameTime(PlatformWindowWin const& window) const -> MonotonicTime;

    auto RefreshScreens() -> void;

    auto GetKeyboardLayout() -> PlatformKeyboardLayoutWin&;
    auto GetInputMethodContext() -> PlatformInputMethodContextWin&;

private:
    static auto CALLBACK ToplevelWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;
    static auto CALLBACK LevelWindowProcedure(HWND hWnd, UINT msg, WPARAM wPAram, LPARAM lParam) -> LRESULT;

private:
    Weak<PlatformWindowContextWin> _self;
    Shared<PlatformInstanceHandleWin> _instanceHandle;
    Shared<PlatformInputMethodContextWin> _inputMethodContext;
    Shared<PlatformDCompositionDeviceWin> _compositionDevice;
    Shared<PlatformVsyncProviderWin> _vsyncProvider;
    Shared<PlatformScreenContext> _screenContext;
    Shared<PlatformKeyboardLayoutWin> _keyboardLayout;
    ATOM _toplevelWindowClass = 0;
    ATOM _levelWindowClass = 0;
    std::vector<HWND> _levelWindows;
};

///
/// @brief
///
template <>
struct Locator::Resolver<PlatformWindowContextWin>
{
    using Interface = PlatformWindowContext;
    static auto Resolve() -> Shared<PlatformWindowContextWin>;
};
}
}

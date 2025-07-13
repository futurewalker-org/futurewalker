// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.Prelude.hpp"
#include "Futurewalker.Application.PlatformApplicationContext.hpp"
#include "Futurewalker.Application.PlatformApplicationThemeContextType.hpp"
#include "Futurewalker.Application.PlatformScreenContext.hpp"
#include "Futurewalker.Application.PlatformWindowContext.hpp"
#include "Futurewalker.Application.PlatformMainThread.hpp"
#include "Futurewalker.Application.Win.PlatformApplicationWinType.hpp"

#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Win.PlatformThreadRuntimeContextWin.hpp"
#include "Futurewalker.Base.Win.PlatformInstanceHandleWin.hpp" 

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.PassKey.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
class PlatformApplicationContextWin : public PlatformApplicationContext
{
public:
    static auto Make(
      Shared<PlatformInstanceHandleWin> instanceHandle,
      Shared<PlatformThreadRuntimeContextWin> mainThreadRuntimeContext,
      Shared<PlatformMainThread> mainThread,
      Shared<PlatformScreenContext> screenContext,
      Shared<PlatformWindowContext> windowContext,
      Shared<PlatformApplicationThemeContext> themeContext) -> Shared<PlatformApplicationContextWin>;

    PlatformApplicationContextWin(
      PassKey<PlatformApplicationContextWin>,
      Shared<PlatformInstanceHandleWin> instanceHandle,
      Shared<PlatformThreadRuntimeContextWin> mainThreadRuntimeContext,
      Shared<PlatformMainThread> mainThread,
      Shared<PlatformScreenContext> screenContext,
      Shared<PlatformWindowContext> windowContext,
      Shared<PlatformApplicationThemeContext> themeContext);

    ~PlatformApplicationContextWin() override;

    auto MakePlatformApplication(PlatformApplication::Delegate delegate) -> Shared<PlatformApplication> override;

    auto GetCurrentApplication() -> Shared<PlatformApplicationWin>;

    auto CreateMessageWindow(PassKey<PlatformApplicationWin>, PlatformApplicationWin& application) -> HWND;
    auto DestroyMessageWindow(PassKey<PlatformApplicationWin>, HWND hwnd) -> void;

private:
    static auto CALLBACK MessageWindowProcedure(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT;

private:
    Weak<PlatformApplicationContextWin> _self;
    Shared<PlatformInstanceHandleWin> _instanceHandle;
    Shared<PlatformThreadRuntimeContextWin> _mainThreadRuntimeContext;
    Shared<PlatformMainThread> _mainThread;
    Shared<PlatformScreenContext> _screenContext;
    Shared<PlatformWindowContext> _windowContext;
    Shared<PlatformApplicationThemeContext> _themeContext;
    Weak<PlatformApplicationWin> _currentApplication;
    ATOM _classAtom = 0;
};

///
/// @brief
///
template <>
struct Locator::Resolver<PlatformApplicationContextWin>
{
    using Interface = PlatformApplicationContext;
    static auto Resolve() -> Shared<PlatformApplicationContextWin>;
};
}
}

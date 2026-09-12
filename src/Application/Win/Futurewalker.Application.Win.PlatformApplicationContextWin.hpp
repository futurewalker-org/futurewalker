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

#include "Futurewalker.Async.ThreadPool.hpp"
#include "Futurewalker.Async.ThisThread.hpp"

#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.PassKey.hpp"

#include <deque>
#include <thread>

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

    auto MakeApplication(PlatformApplication::Delegate delegate) -> Shared<PlatformApplication> override;
    auto GetCurrentApplication() -> Shared<PlatformApplication> override;
    auto RunApplication(Shared<PlatformApplication> app, Function<void()> cleanup) -> Async<void> override;

    auto PostQuitMessage() -> void;

    auto Schedule() -> AsyncTask<void>;
    auto ScheduleAfter(const std::chrono::nanoseconds& delay) -> AsyncTask<void>;

    auto IsMainThread() const -> Bool;
    auto IsRunning() const -> Bool;

private:
    auto Initialize() -> void;
    auto GetSelf() -> Shared<PlatformApplicationContextWin>;
    auto GetSelf() const -> Shared<PlatformApplicationContextWin const>;
    auto CreateMessageWindow() -> Bool;
    auto DestroyMessageWindow() -> void;
    auto TranslateAndDispatchMessage(Shared<PlatformApplicationWin> const& app, MSG const& msg, Function<void()> const& cleanup) -> Bool;
    auto HandlePostedEvent(Bool& callDefaultProcedure, WPARAM wParam, LPARAM lParam) -> LRESULT;

    auto BeginRunning(Shared<PlatformApplicationWin> const& app) -> void;
    auto EndRunning(Shared<PlatformApplicationWin> const& app) -> void;

    auto Post(LazyTask<void> task) -> void;
    auto PopTask() -> Optional<LazyTask<void>>;
    auto HasTask() -> Bool;

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
    Shared<ThreadPool> _threadPool;
    std::thread::id _mainThreadId;
    Shared<ThisThread::Scheduler> _mainThreadScheduler;
    HWND _messageWindow = nullptr;
    Bool _running = false;
    Bool _quitting = false;
    std::deque<LazyTask<void>> _tasks;
    std::mutex _mutex;
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

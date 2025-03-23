// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.Win.PlatformEventLoopWinType.hpp"
#include "Futurewalker.Application.Win.PlatformEventLoopContextWinType.hpp"
#include "Futurewalker.Application.PlatformEventLoop.hpp"

#include "Futurewalker.Async.Lazy.hpp"
#include "Futurewalker.Async.LazyTask.hpp"
#include "Futurewalker.Async.ThreadPoolType.hpp"

#include "Futurewalker.Core.Optional.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.Memory.hpp"

#include <windows.h>
#include <winuser.h>

#include <atomic>
#include <exception>
#include <deque>
#include <mutex>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Platform event loop implementation. 
///
class PlatformEventLoopWin : public PlatformEventLoop
{
public:
    struct ThreadInfo
    {
        std::thread::id const threadId;
    };
    static auto Make(Shared<PlatformEventLoopContextWin> context, ThreadInfo threadInfo, Shared<ThreadPool> threadPool) -> Shared<PlatformEventLoopWin>;

    PlatformEventLoopWin(PassKey<PlatformEventLoopWin>, Shared<PlatformEventLoopContextWin> context, ThreadInfo threadInfo, Shared<ThreadPool> threadPool);

    auto Spawn(Lazy<void> task, Bool const wait) -> void;
    auto Run(RunDelegate const& delegate) -> Async<void> override;
    auto Exit() -> void override;
    auto Schedule() -> AsyncTask<void> override;
    auto ScheduleAfter(const std::chrono::nanoseconds& delay) -> AsyncTask<void> override;

    auto SetNativeHandle(PassKey<PlatformEventLoopContextWin>, HWND hwnd) -> void;
    auto MessageWindowProcedure(PassKey<PlatformEventLoopContextWin>, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept -> LRESULT;

private:
    auto TranslateAndDispatchMessage(MSG const& msg) -> Bool;
    auto HandlePostedEvent(bool& callDefaultProcedure, WPARAM wParam, LPARAM lParam) -> LRESULT;

private:
    auto Post(LazyTask<void> task) -> void;

private:
    auto PopTask() -> Optional<LazyTask<void>>;
    auto HasTask() -> Bool;

private:
    ThreadInfo _threadInfo;
    Weak<PlatformEventLoopWin> _self;
    Shared<PlatformEventLoopContextWin> _context;
    Shared<ThreadPool> _threadPool;
    HWND _hwnd = NULL;
    std::exception_ptr _exception = nullptr;
    std::deque<LazyTask<void>> _tasks;
    std::atomic<bool> _running;
    std::mutex _mutex;
};
}
}

// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.PlatformApplicationWinType.hpp"
#include "Futurewalker.Application.PlatformApplication.hpp"
#include "Futurewalker.Application.Win.PlatformApplicationContextWin.hpp"

#include "Futurewalker.Async.ThreadPool.hpp"
#include "Futurewalker.Async.ThisThread.hpp"
#include "Futurewalker.Async.AsyncTask.hpp"
#include "Futurewalker.Async.LazyTask.hpp"

#include <deque>
#include <thread>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
class PlatformApplicationWin : public PlatformApplication
{
public:
    static auto Make(Delegate delegate, Shared<PlatformApplicationContextWin> context, Shared<ThreadPool> threadPool) -> Shared<PlatformApplicationWin>;

    PlatformApplicationWin(PassKey<PlatformApplication>, Delegate delegate, Shared<PlatformApplicationContextWin> context, Shared<ThreadPool> threadPool);
    ~PlatformApplicationWin();

    auto Run() -> Async<void> override;
    auto Exit() -> void override;
    auto IsRunning() -> Bool override;
    auto IsActive() -> Bool override;
    auto IsForeground() -> Bool override;
    auto SetMainMenu(Menu const& menu) -> void override;

    auto IsMainThread() const -> Bool;
    auto Schedule() -> AsyncTask<void>;
    auto ScheduleAfter(const std::chrono::nanoseconds& delay) -> AsyncTask<void>;

    auto SetActive(Bool const active) -> void;
    auto SetNativeHandle(PassKey<PlatformApplicationContextWin>, HWND hwnd) -> void;
    auto MessageWindowProcedure(PassKey<PlatformApplicationContextWin>, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept -> LRESULT;

protected:
    auto Initialize() -> void override;

private:
    auto TranslateAndDispatchMessage(MSG const& msg) -> Bool;
    auto HandlePostedEvent(bool& callDefaultProcedure, WPARAM wParam, LPARAM lParam) -> LRESULT;
    auto Post(LazyTask<void> task) -> void;
    auto PopTask() -> Optional<LazyTask<void>>;
    auto HasTask() -> Bool;

private:
    Shared<PlatformApplicationContextWin> _context;
    Shared<ThreadPool> _threadPool;
    Shared<ThisThread::Scheduler> _thisThreadScheduler;
    HWND _hwnd = NULL;
    Bool _running = false;
    Bool _active = false;
    std::deque<LazyTask<void>> _tasks;
    std::mutex _mutex;
    std::thread::id _threadId;
};
}
}

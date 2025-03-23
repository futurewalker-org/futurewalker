// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.Win.PlatformEventLoopWin.hpp"
#include "Futurewalker.Application.Win.PlatformEventLoopContextWin.hpp"

#include "Futurewalker.Async.AsyncFunction.hpp"
#include "Futurewalker.Async.ThreadPool.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Core.Win.PlatformWindowMessageWin.hpp"

#include <thread>
#include <chrono>

namespace FW_DETAIL_NS
{
///
/// @brief Create PlatformEventLoopWin object.
///
/// @param context Event loop context
/// @param hwnd Message window handle
///
auto PlatformEventLoopWin::Make(Shared<PlatformEventLoopContextWin> context, ThreadInfo threadInfo, Shared<ThreadPool> threadPool) -> Shared<PlatformEventLoopWin>
{
    auto eventLoop = Shared<PlatformEventLoopWin>::Make(PassKey<PlatformEventLoopWin>(), context, threadInfo, threadPool);
    eventLoop->_self = eventLoop;
    return eventLoop;
}

///
/// @brief Constructor.
///
PlatformEventLoopWin::PlatformEventLoopWin(PassKey<PlatformEventLoopWin>, Shared<PlatformEventLoopContextWin> context, ThreadInfo threadInfo, Shared<ThreadPool> threadPool)
  : PlatformEventLoop()
  , _threadInfo {threadInfo}
  , _context {context}
  , _threadPool {threadPool}
{
}

///
/// @brief Spawn a task in the event loop.
///
/// @param task The task to spawn.
/// @param wait Wait for the task to complete with event loop.
///
auto PlatformEventLoopWin::Spawn(Lazy<void> task, Bool const wait) -> void
{
    if (_threadInfo.threadId != std::this_thread::get_id())
    {
        throw Exception(ErrorCode::InvalidOperation, "Event loop must run on the thread it belongs to");
    }

    if (!_running)
    {
        throw Exception(ErrorCode::InvalidOperation);
    }

    const auto handle = AsyncFunction::SpawnFn([&]() -> Lazy<void> { co_await std::move(task); });

    while (wait && !handle.WaitFor(std::chrono::milliseconds(0)))
    {
        auto msg = MSG();
        const auto result = ::GetMessageW(&msg, NULL, 0, 0);

        if (result == -1)
        {
            break;
        }

        if (!TranslateAndDispatchMessage(msg))
        {
            break;
        }
    }
}

///
/// @brief Run event loop.
///
/// @param delegate Delegate
///
auto PlatformEventLoopWin::Run(RunDelegate const& delegate) -> Async<void>
{
    if (_threadInfo.threadId != std::this_thread::get_id())
    {
        FW_DEBUG_LOG_ERROR("PlatformEventLoopWin::Run: Event loop must run on the thread it belongs to");
        FW_DEBUG_ASSERT(false);
        co_return;
    }

    if (_running.exchange(true))
    {
        FW_DEBUG_LOG_ERROR("PlatformEventLoopWin::Run: Event loop must run on the thread it belongs to");
        FW_DEBUG_ASSERT(false);
        co_return;
    }

    auto const hwnd = _context->CreateMessageWindow({}, *this);
    if (!hwnd)
    {
        FW_DEBUG_LOG_ERROR("PlatformEventLoopWin::Run: Failed to create message window");
        FW_DEBUG_ASSERT(false);
        co_return;
    }

    try
    {
        FW_DEBUG_LOG_INFO("Starting event loop on thread {}", std::this_thread::get_id());

        AsyncFunction::SpawnFn(delegate.startTask);

        while (true)
        {
            auto msg = MSG();
            const auto result = ::GetMessageW(&msg, NULL, 0, 0);

            if (result == -1)
            {
                break;
            }

            // WM_QUIT or other messages.
            if (!TranslateAndDispatchMessage(msg))
            {
                break;
            }
        }

        AsyncFunction::SpawnFn(delegate.exitTask);

        FW_DEBUG_LOG_INFO("Exiting event loop on thread {}", std::this_thread::get_id());
    }
    catch (...)
    {
        FW_DEBUG_LOG_ERROR("PlatformEventLoopWin::Run: Unhandled exception in event loop");
        FW_DEBUG_ASSERT(false);
    }

    _context->DestroyMessageWindow({}, hwnd);

    _running.store(false);
}

///
/// @brief Exit event loop.
///
auto PlatformEventLoopWin::Exit() -> void
{
    if (_threadInfo.threadId == std::this_thread::get_id())
    {
        FW_DEBUG_LOG_INFO("PlatformEventLoopWin::Exit: Called on thread {}", std::this_thread::get_id());
        ::PostQuitMessage(0);
        return;
    }
    FW_DEBUG_LOG_ERROR("PlatformEventLoopWin::Exit: Event loop must run on the thread it belongs to");
    FW_DEBUG_ASSERT(false);
}

///
/// @brief Schedule task to event loop.
///
auto PlatformEventLoopWin::Schedule() -> AsyncTask<void>
{
    struct Awaitable
    {
        Weak<PlatformEventLoopWin> eventLoop;

        bool await_ready()
        {
            return false;
        }

        void await_suspend(std::coroutine_handle<> c)
        {
            if (auto loop = eventLoop.Lock())
            {
                return loop->Post([](std::coroutine_handle<> c) -> LazyTask<void> { co_return c.resume(); }(c));
            }
            throw Exception(ErrorCode::Failure);
        }

        void await_resume()
        {
        }
    };
    co_await Awaitable {_self};
}

///
/// @brief Schedule task to event loop after delay.
///
/// @param delay Delay
///
auto PlatformEventLoopWin::ScheduleAfter(const std::chrono::nanoseconds& delay) -> AsyncTask<void>
{
    auto self = _self;

    co_await _threadPool->ScheduleAfter(delay);

    if (auto eventLoop = self.Lock())
    {
        co_return co_await eventLoop->Schedule();
    }
    throw Exception(ErrorCode::Failure);
}

///
/// @brief Set native handle.
///
/// @param hwnd Message window handle
///
auto PlatformEventLoopWin::SetNativeHandle(PassKey<PlatformEventLoopContextWin>, HWND hwnd) -> void
{
    auto lock = std::unique_lock(_mutex);
    _hwnd = hwnd;
}

///
/// @brief Window procedure of message window.
///
auto PlatformEventLoopWin::MessageWindowProcedure(PassKey<PlatformEventLoopContextWin>, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept -> LRESULT
{
    auto callDefaultProcedure = true;
    auto result = LRESULT();
    try
    {
        if (msg == WM_FW_EVENT_QUEUE_POST)
        {
            result = HandlePostedEvent(callDefaultProcedure, wParam, lParam);
        }
    }
    catch (...)
    {
        FW_DEBUG_LOG_ERROR("Unhandled exception in window procedure");
        FW_DEBUG_ASSERT(false);
    }

    if (callDefaultProcedure)
    {
        return ::DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    return result;
}

///
/// @brief Translate and dispatch window message.
///
Bool PlatformEventLoopWin::TranslateAndDispatchMessage(MSG const& msg)
{
    if (msg.message == WM_QUIT)
    {
        ::PostQuitMessage(static_cast<int>(msg.wParam));

        if (HasTask())
        {
            return true;
        }
        return false;
    }

    ::TranslateMessage(&msg);
    ::DispatchMessageW(&msg);

    return true;
}

///
/// @brief
///
auto PlatformEventLoopWin::HandlePostedEvent(bool& callDefaultProcedure, WPARAM wParam, LPARAM lParam) -> LRESULT
{
    (void)wParam;
    (void)lParam;
    if (auto task = PopTask())
    {
        AsyncFunction::Spawn(std::move(*task));
    }
    callDefaultProcedure = false;
    return 0;
}

///
/// @brief
///
void PlatformEventLoopWin::Post(LazyTask<void> task)
{
    std::unique_lock lock(_mutex);

    if (_hwnd)
    {
        _tasks.push_front(std::move(task));

        if (!::PostMessageW(_hwnd, WM_FW_EVENT_QUEUE_POST, 0, 0))
        {
            FW_DEBUG_LOG_ERROR("PostMessage for WM_FW_EVENT_QUEUE_POST failed");
            FW_DEBUG_ASSERT(false);
        }
    }
    else
    {
        throw Exception(ErrorCode::Failure, "Thread was already exited");
    }
}

///
/// @brief
///
auto PlatformEventLoopWin::PopTask() -> Optional<LazyTask<void>>
{
    std::unique_lock lock(_mutex);
    if (!_tasks.empty())
    {
        auto task = std::move(_tasks.back());
        _tasks.pop_back();
        return task;
    }
    return {};
}

///
/// @brief
///
auto PlatformEventLoopWin::HasTask() -> Bool
{
    std::unique_lock lock(_mutex);
    return !_tasks.empty();
}
}

// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.Win.PlatformApplicationWin.hpp"

#include "Futurewalker.Application.PlatformApplicationEvent.hpp"

#include "Futurewalker.Async.AsyncFunction.hpp"

#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Core.Win.PlatformWindowMessageWin.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Create PlatformApplicationWin object.
///
auto PlatformApplicationWin::Make(Delegate delegate, Shared<PlatformApplicationContextWin> context, Shared<ThreadPool> threadPool) -> Shared<PlatformApplicationWin>
{
    auto application = Shared<PlatformApplicationWin>::Make(PassKey<PlatformApplicationWin>(), delegate, context, threadPool);
    application->_self = application;
    return application;
}

///
/// @brief
///
PlatformApplicationWin::PlatformApplicationWin(PassKey<PlatformApplicationWin>, Delegate delegate, Shared<PlatformApplicationContextWin> context, Shared<ThreadPool> threadPool)
  : PlatformApplication(delegate)
  , _context {context}
  , _threadPool {threadPool}
{
    _threadId = std::this_thread::get_id();
}

///
/// @brief
///
auto PlatformApplicationWin::Run() -> Async<void>
{
    if (!IsMainThread())
    {
        throw Exception(ErrorCode::InvalidOperation, "Application must run on main thread");
    }

    if (std::exchange(_running, true))
    {
        FW_DEBUG_LOG_ERROR("PlatformApplicationWin::Run: Event loop must run on the thread it belongs to");
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

        AsyncFunction::SpawnFn([&]() -> Async<void> {
            auto e = Event(PlatformApplicationEvent::Started());
            co_await SendApplicationEvent(e);
        });

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

        AsyncFunction::SpawnFn([&]() -> Async<void> {
            auto e = Event(PlatformApplicationEvent::Exited());
            co_await SendApplicationEvent(e);
        });

        FW_DEBUG_LOG_INFO("Exiting event loop on thread {}", std::this_thread::get_id());
    }
    catch (...)
    {
        FW_DEBUG_LOG_ERROR("PlatformEventLoopWin::Run: Unhandled exception in event loop");
        FW_DEBUG_ASSERT(false);
    }

    _context->DestroyMessageWindow({}, hwnd);

    _running = false;
}

///
/// @brief
///
auto PlatformApplicationWin::Exit() -> void
{
    if (IsMainThread())
    {
        FW_DEBUG_LOG_INFO("PlatformEventLoopWin::Exit: Called on thread {}", std::this_thread::get_id());
        ::PostQuitMessage(0);
        return;
    }
    FW_DEBUG_LOG_ERROR("PlatformEventLoopWin::Exit: Event loop must run on the thread it belongs to");
    FW_DEBUG_ASSERT(false);
}

///
/// @brief
///
auto PlatformApplicationWin::IsRunning() -> Bool
{
    return _running;
}

///
/// @brief Returns whether the application is active or not.
///
/// Application will receive `ApplicationEvent::ActiveChanged` when this property changed.
///
auto PlatformApplicationWin::IsActive() -> Bool
{
    return _active;
}

///
/// @brief Returns whether the application is foreground or not.
///
/// Application will receive `ApplicationEvent::ForegroundChanged` when this property changed.
///
auto PlatformApplicationWin::IsForeground() -> Bool
{
    return true;
}

///
/// @brief Check if the current thread is the main thread.
///
auto PlatformApplicationWin::IsMainThread() const -> Bool
{
    return _threadId == std::this_thread::get_id();
}

///
/// @brief Schedule task to event loop.
///
auto PlatformApplicationWin::Schedule() -> AsyncTask<void>
{
    struct Awaitable
    {
        Weak<PlatformApplicationWin> application;

        bool await_ready()
        {
            return false;
        }

        void await_suspend(std::coroutine_handle<> c)
        {
            if (auto app = application.Lock())
            {
                return app->Post([](std::coroutine_handle<> c) -> LazyTask<void> { co_return c.resume(); }(c));
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
auto PlatformApplicationWin::ScheduleAfter(const std::chrono::nanoseconds& delay) -> AsyncTask<void>
{
    auto self = _self;

    co_await _threadPool->ScheduleAfter(delay);

    if (auto app = self.Lock())
    {
        co_return co_await app->Schedule();
    }
    throw Exception(ErrorCode::Failure);
}

///
/// @brief
///
/// @param active
///
auto PlatformApplicationWin::SetActive(Bool const active) -> void
{
    if (_active != active)
    {
        _active = active;

        auto parameter = PlatformApplicationEvent::ActiveChanged();
        parameter.SetActive(active);
        auto event = Event(parameter);
        SyncSendApplicationEvent(event);
    }
}

///
/// @brief Set native handle for the application.
///
auto PlatformApplicationWin::SetNativeHandle(PassKey<PlatformApplicationContextWin>, HWND hwnd) -> void
{
    _hwnd = hwnd;
}

auto PlatformApplicationWin::MessageWindowProcedure(PassKey<PlatformApplicationContextWin>, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept -> LRESULT
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
Bool PlatformApplicationWin::TranslateAndDispatchMessage(MSG const& msg)
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
auto PlatformApplicationWin::HandlePostedEvent(bool& callDefaultProcedure, WPARAM wParam, LPARAM lParam) -> LRESULT
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
auto PlatformApplicationWin::Post(LazyTask<void> task) -> void
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
auto PlatformApplicationWin::PopTask() -> Optional<LazyTask<void>>
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
auto PlatformApplicationWin::HasTask() -> Bool
{
    std::unique_lock lock(_mutex);
    return !_tasks.empty();
}

///
/// @brief
///
auto PlatformApplicationWin::SyncSendApplicationEvent(Event const& event) -> void
{
    AsyncFunction::SpawnFn([&]() -> Lazy<void> {
        auto e = event;
        co_await SendApplicationEvent(e);
    });
}
}

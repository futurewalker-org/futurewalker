// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Async.ThreadPoolType.hpp"
#include "Futurewalker.Async.Concepts.hpp"

#include "Futurewalker.Base.Locator.hpp"
#include "Futurewalker.Base.Debug.hpp"

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Exception.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

#include <boost/asio/thread_pool.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/post.hpp>

#include <chrono>
#include <coroutine>
#include <optional>

namespace FW_DETAIL_NS
{
struct [[nodiscard]] ThreadPoolScheduleAwaitable : NonCopyable
{
public:
    explicit ThreadPoolScheduleAwaitable(boost::asio::thread_pool& pool)
      : _pool {pool}
    {
    }

    auto await_ready() const noexcept -> bool
    {
        return false;
    }

    template <class Promise>
    auto await_suspend(std::coroutine_handle<Promise> c)
    {
        boost::asio::post(_pool, [c] {
            if constexpr (Concepts::CancellablePromise<Promise>)
            {
                if (auto const continuation = c.promise().cancel())
                {
                    continuation.resume();
                    return;
                }
            }
            c.resume();
        });
    }

    auto await_resume() -> void
    {
    }

private:
    boost::asio::thread_pool& _pool;
};

template <class Rep, class Period>
struct [[nodiscard]] ThreadPoolScheduleAfterAwaitable : NonCopyable
{
public:
    ThreadPoolScheduleAfterAwaitable(boost::asio::thread_pool& pool, std::chrono::duration<Rep, Period> delay)
      : _pool {pool}
      , _delay {delay}
    {
    }

    auto await_ready() noexcept -> bool
    {
        return false;
    }

    template <class Promise>
    auto await_suspend(std::coroutine_handle<Promise> c) -> std::coroutine_handle<>
    {
        if constexpr (Concepts::CancellablePromise<Promise>)
        {
            if (auto const continuation = c.promise().cancel())
            {
                return continuation;
            }
        }

        _timer = boost::asio::steady_timer(_pool);
        _timer->expires_after(_delay);
        _timer->async_wait([this, c](auto const& e) {
            if constexpr (Concepts::CancellablePromise<Promise>)
            {
                if (auto const continuation = c.promise().cancel())
                {
                    continuation.resume();
                    return;
                }
            }
            _error = e;
            c.resume();
        });

        if constexpr (Concepts::CancellablePromise<Promise>)
        {
            _stopToken = c.promise().get_stop_token();

            if (_stopToken.stop_possible())
            {
                _stopCallback.emplace(_stopToken, [&] {
                    if (_timer)
                    {
                        _timer->cancel();
                    }
                });
            }
        }
        return std::noop_coroutine();
    }

    auto await_resume() -> void
    {
        _stopCallback.reset();
        _timer.reset();

        if (_error)
        {
            throw Exception(ErrorCode::Failure, "Failed to wait timer");
        }
    }

private:
    boost::asio::thread_pool& _pool;
    std::chrono::duration<Rep, Period> _delay;
    std::optional<boost::asio::steady_timer> _timer;
    boost::system::error_code _error;
    std::stop_token _stopToken;
    std::optional<std::stop_callback<std::function<void()>>> _stopCallback;
};

namespace FW_EXPORT
{
///
/// @brief Thread pool.
///
/// ThreadPool class provides global thread pool for asynchronous operations.
///
/// @note ThreadPool only supports scheduling via GetScheduler().
///
class ThreadPool : NonCopyable
{
public:
    ///
    /// @brief Create thread pool.
    ///
    /// @param[in] threadCount Number of threads in pool.
    ///
    explicit ThreadPool(SInt64 const threadCount)
      : _pool(static_cast<size_t>(threadCount))
    {
    }

    ///
    /// @brief Schedule task on ThreadPool.
    ///
    /// @return Awaitable object which can schedule awaiting coroutine on ThreadPool.
    ///
    auto Schedule()
    {
        return ThreadPoolScheduleAwaitable(_pool);
    }

    ///
    /// @brief Schedule task on ThreadPool after some delay.
    ///
    /// @param[in] delay Amount of delay before resuming task.
    ///
    /// @return Awaitable object which can schedule awaiting coroutine on ThreadPool.
    ///
    template <class Rep, class Period>
    auto ScheduleAfter(const std::chrono::duration<Rep, Period>& delay)
    {
        return ThreadPoolScheduleAfterAwaitable<Rep, Period>(_pool, delay);
    }

private:
    boost::asio::thread_pool _pool;
};

template <>
struct Locator::Resolver<ThreadPool>
{
    using Interface = ThreadPool;
    static auto Resolve() -> Shared<ThreadPool>;
};
}
}

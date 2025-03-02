// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Async.TaskHandleType.hpp"
#include "Futurewalker.Async.Concepts.hpp"
#include "Futurewalker.Async.TypeTraits.hpp" 

#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Optional.hpp"

#include <coroutine>
#include <semaphore>
#include <future>
#include <stop_token>
#include <optional> 

namespace FW_DETAIL_NS
{
template <class T>
class TaskHandlePromiseBase : NonCopyable
{
public:
    auto initial_suspend() noexcept
    {
        return std::suspend_never {};
    }

    auto final_suspend() noexcept
    {
        return std::suspend_never {};
    }

    auto get_stop_token() const
    {
        return _stopToken;
    }

    auto SetStopToken(PassKey<TaskHandle<T>>, std::stop_token stopToken)
    {
        _stopToken = std::move(stopToken);
    }

protected:
    std::stop_token _stopToken;
};

namespace FW_EXPORT
{
///
/// @brief Handle to a spawned task.
///
/// @tparam T The type of the result produced by the task.
///
/// @note This class is just a handle to a spawned task, and does not own the task itself.
///
template <class T>
class TaskHandle : NonCopyable
{
    using ResultType = std::optional<T>;

public:
    class promise_type : public TaskHandlePromiseBase<T>
    {
    public:
        auto get_return_object()
        {
            return TaskHandle<T>(*this);
        }

        template <class Self>
        auto cancel(this Self& self) -> std::coroutine_handle<>
        {
            if (self._stopToken.stop_requested())
            {
                self._promise.set_value(std::nullopt);
                std::coroutine_handle<Self>::from_promise(self).destroy();
                return std::noop_coroutine();
            }
            return nullptr;
        }

        void unhandled_exception() noexcept
        {
            _promise.set_exception(std::current_exception());
        }

        auto return_value(T value)
        {
            _promise.set_value(std::move(value));
        }

        auto GetFuture(PassKey<TaskHandle<T>>)
        {
            return _promise.get_future().share();
        }

    private:
        std::promise<ResultType> _promise;
    };

    ///
    /// @brief Waits for the task to complete.
    ///
    /// @return The result of the task if it completed successfully, otherwise empty.
    ///
    auto Wait() const -> Optional<T>
    {
        if (const auto& result = _future.get())
        {
            return *result;
        }
        return {};
    }

    ///
    /// @brief Waits for the task to complete or for the specified duration to elapse.
    ///
    /// @param[in] duration The maximum amount of time to wait for the task to complete.
    ///
    /// @return The result of the task if it completed successfully within the specified duration, otherwise empty.
    ///
    template <class Rep, class Period>
    auto WaitFor(std::chrono::duration<Rep, Period> duration) const -> Optional<T>
    {
        if (_future.wait_for(duration) == std::future_status::ready)
        {
            if (const auto& result = _future.get())
            {
                return *result;
            }
        }
        return {};
    }

    ///
    /// @brief Requests cancellation of the task.
    ///
    /// @note Cancellation is cooperative so the task might still successfully complete before it is cancelled.
    ///
    auto Cancel() -> void
    {
        _stopSource.request_stop();
    }

private:
    explicit TaskHandle(promise_type& promise)
    {
        promise.SetStopToken({}, _stopSource.get_token());
        _future = promise.GetFuture({});
    }

    std::stop_source _stopSource;
    std::shared_future<ResultType> _future;
};

///
/// @brief Handle to a spawned task with no result.
///
template <>
class TaskHandle<void> : NonCopyable
{
    using ResultType = std::optional<std::monostate>;

public:
    class promise_type : public TaskHandlePromiseBase<void>
    {
    public:
        auto get_return_object()
        {
            return TaskHandle<void>(*this);
        }

        template <class Self>
        auto cancel(this Self& self) -> std::coroutine_handle<>
        {
            if (self._stopToken.stop_requested())
            {
                self._promise.set_value(std::nullopt);
                std::coroutine_handle<Self>::from_promise(self).destroy();
                return std::noop_coroutine();
            }
            return nullptr;
        }

        void unhandled_exception() noexcept
        {
            _promise.set_exception(std::current_exception());
        }

        auto return_void() -> void
        {
            _promise.set_value(std::monostate());
        }

        auto GetFuture(PassKey<TaskHandle<void>>)
        {
            return _promise.get_future().share();
        }

    private:
        std::promise<ResultType> _promise;
    };

    ///
    /// @brief Wait for the task to complete.
    ///
    /// @return True if the task completed successfully, otherwise false.
    ///
    auto Wait() const -> Bool
    {
        return _future.get().has_value();
    }

    ///
    /// @brief Wait for the task to complete or for the specified duration to elapse.
    ///
    /// @param[in] duration The maximum amount of time to wait for the task to complete.
    ///
    /// @return True if the task completed successfully within the specified duration, otherwise false.
    ///
    template <class Rep, class Period>
    auto WaitFor(std::chrono::duration<Rep, Period> duration) const -> Bool
    {
        if (_future.wait_for(duration) == std::future_status::ready)
        {
            return _future.get().has_value();
        }
        return false;
    }

    ///
    /// @brief Requests cancellation of the task.
    ///
    /// @note Cancellation is cooperative so the task might still successfully complete before it is cancelled.
    ///
    auto Cancel() -> void
    {
        _stopSource.request_stop();
    }

private:
    explicit TaskHandle(promise_type& promise)
    {
        promise.SetStopToken({}, _stopSource.get_token());
        _future = promise.GetFuture({});
    }

    std::stop_source _stopSource;
    std::shared_future<ResultType> _future;
};
}
}

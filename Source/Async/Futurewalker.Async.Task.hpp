// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Async.TaskType.hpp"
#include "Futurewalker.Async.TypeTraits.hpp"
#include "Futurewalker.Async.Concepts.hpp"

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Exception.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

#include <coroutine>
#include <stop_token>
#include <exception>

namespace FW_DETAIL_NS
{
template <class T>
struct TaskPromiseBase : NonCopyable
{
    auto initial_suspend()
    {
        return std::suspend_always();
    }

    template <class Self>
    auto final_suspend(this Self&) noexcept
    {
        struct Awaitable
        {
            auto await_ready() noexcept -> bool
            {
                return false;
            }

            auto await_suspend(std::coroutine_handle<Self> c) noexcept -> std::coroutine_handle<>
            {
                if (const auto continuation = c.promise().cancel())
                {
                    return continuation;
                }

                if (const auto continuation = c.promise()._continuation)
                {
                    return continuation;
                }
                return std::noop_coroutine();
            }

            auto await_resume() noexcept -> void
            {
            }
        };
        return Awaitable {};
    }

    auto unhandled_exception() -> void
    {
        _exception = std::current_exception();
    }

    auto get_stop_token() const -> std::stop_token
    {
        return _stopToken;
    }

    template <class Self>
    auto cancel(this Self& self) -> std::coroutine_handle<>
    {
        if (self._stopToken.stop_requested())
        {
            return self._cancelCallback();
        }
        return nullptr;
    }

    auto SetStopToken(const std::stop_token& stopToken)
    {
        _stopToken = stopToken;
    }

    auto SetCancelCallback(std::function<std::coroutine_handle<>()> cancelCallback)
    {
        _cancelCallback = cancelCallback;
    }

    auto SetContinuation(std::coroutine_handle<> continuation) -> void
    {
        _continuation = continuation;
    }

    auto RethrowException() const -> void
    {
        if (_exception)
        {
            std::rethrow_exception(_exception);
        }
    }

protected:
    std::coroutine_handle<> _continuation;
    std::exception_ptr _exception;
    std::stop_token _stopToken;
    std::function<std::coroutine_handle<>()> _cancelCallback;
};

template <class T>
struct [[nodiscard]] TaskAwaitableBase : NonCopyable
{
    using promise_type = Task<T>::promise_type;
    std::coroutine_handle<promise_type> coroutine;

    TaskAwaitableBase(std::coroutine_handle<promise_type> coroutine) noexcept
      : coroutine {coroutine}
    {
    }

    auto await_ready() const noexcept -> bool
    {
        return !coroutine || coroutine.done();
    }

    template <class Promise>
    auto await_suspend(std::coroutine_handle<Promise> c) noexcept -> std::coroutine_handle<>
    {
        if constexpr (Concepts::CancellablePromise<Promise>)
        {
            if (const auto continuation = c.promise().cancel())
            {
                return continuation;
            }
            coroutine.promise().SetStopToken(c.promise().get_stop_token());
            coroutine.promise().SetCancelCallback([=] { return c.promise().cancel(); });
        }
        coroutine.promise().SetContinuation(c);
        return coroutine;
    }
};

namespace FW_EXPORT
{
///
/// @brief Task type.
///
/// @tparam T The type of the result produced by the task.
///
template <class T>
class [[nodiscard]] Task : NonCopyable
{
public:
    class promise_type;

    ///
    /// @brief Construct empty task.
    ///
    Task() noexcept
      : _coroutine {}
    {
    }

    ///
    /// @brief Destroy task and its coroutine frame.
    ///
    ~Task() noexcept
    {
        if (_coroutine)
        {
            _coroutine.destroy();
        }
    }

    ///
    /// @brief Move constructor.
    ///
    Task(Task&& other) noexcept
      : _coroutine {std::exchange(other._coroutine, nullptr)}
    {
    }

    ///
    /// @brief Move assignment operator.
    ///
    auto operator=(Task&& other) noexcept -> Task<T>&
    {
        Task tmp = std::move(other);
        Swap(tmp);
        return *this;
    }

    ///
    /// @brief Swap tasks.
    ///
    auto Swap(Task& other) noexcept -> void
    {
        std::swap(_coroutine, other._coroutine);
    }

    ///
    /// @brief Start executing task and await its completion.
    ///
    /// @return Awaiter object which returns the result of the task.
    ///
    /// @note Rethrows any exception thrown by the task.
    /// @note Throws Exception when the task does not have valid coroutine handle.
    ///
    friend auto operator co_await(Task self)
    {
        struct Awaitable : TaskAwaitableBase<T>
        {
            Awaitable(Task<T>&& task)
              : TaskAwaitableBase<T>(task._coroutine)
              , task {std::move(task)}
            {
            }

            auto await_resume()
            {
                if (this->coroutine)
                {
                    return this->coroutine.promise().GetResult();
                }
                throw Exception(ErrorCode::InvalidOperation, "Awaiting invalid task");
            }

            Task<T> task;
        };
        return Awaitable(std::move(self));
    }

private:
    explicit Task(std::coroutine_handle<promise_type> coroutine)
      : _coroutine {coroutine}
    {
    }

    std::coroutine_handle<promise_type> _coroutine;
};

///
/// @brief Promise type for coroutines.
///
template <class T>
class Task<T>::promise_type : public TaskPromiseBase<T>
{
public:
    using TaskPromiseBase<T>::TaskPromiseBase;

    auto get_return_object() -> Task
    {
        return Task(std::coroutine_handle<promise_type>::from_promise(*this));
    }

    auto return_value(T value) -> void
    {
        _result = std::move(value);
    }

    auto GetResult()
    {
        this->RethrowException();
        return std::move(_result.value());
    }

private:
    std::optional<T> _result;
};

///
/// @brief Promise type for coroutines.
///
template <>
class Task<void>::promise_type : public TaskPromiseBase<void>
{
public:
    using TaskPromiseBase::TaskPromiseBase;

    auto get_return_object() -> Task<void>
    {
        return Task(std::coroutine_handle<promise_type>::from_promise(*this));
    }

    auto return_void() -> void
    {
    }

    auto GetResult() -> void
    {
        this->RethrowException();
    }
};
}
}

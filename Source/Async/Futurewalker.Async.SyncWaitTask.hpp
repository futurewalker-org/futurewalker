// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Async.Concepts.hpp"
#include "Futurewalker.Async.TypeTraits.hpp" 

#include <coroutine>
#include <semaphore>

namespace FW_DETAIL_NS
{
///
/// @brief Task type for SyncWait.
///
template <class T>
struct SyncWaitTask
{
    struct promise_type;

    SyncWaitTask(const SyncWaitTask&) = delete;
    SyncWaitTask(SyncWaitTask&&) = delete;

    SyncWaitTask(std::coroutine_handle<promise_type> coroutine)
      : coroutine {coroutine}
    {
    }

    ~SyncWaitTask() noexcept
    {
        if (coroutine)
        {
            coroutine.destroy();
        }
    }

    decltype(auto) Get() &&
    {
        return std::move(coroutine.promise()).Get();
    }

    std::coroutine_handle<promise_type> coroutine;
};

///
/// @brief Base class of promise_type of SyncWaitTask.
///
template <class T>
struct SyncWaitTaskPromiseBase
{
    std::binary_semaphore completed {0};
    std::exception_ptr exception;

    auto initial_suspend() noexcept
    {
        return std::suspend_never {};
    }

    auto final_suspend() noexcept
    {
        struct Awaitable
        {
            auto await_ready() const noexcept
            {
                return false;
            }

            auto await_suspend(std::coroutine_handle<>) noexcept
            {
                // We have to notify after all local variables and temporaries are destroyed.
                promise.Set();
            }

            auto await_resume() noexcept
            {
            }

            SyncWaitTaskPromiseBase& promise;
        };
        return Awaitable(*this);
    }

    void unhandled_exception() noexcept
    {
        exception = std::current_exception();
        Set();
    }

    void RethrowException()
    {
        if (exception)
        {
            std::rethrow_exception(exception);
        }
    }

    void Wait() noexcept
    {
        completed.acquire();
    }

    void Set() noexcept
    {
        completed.release();
    }
};

template <class T>
struct SyncWaitTask<T>::promise_type : SyncWaitTaskPromiseBase<T>
{
    std::optional<std::remove_reference_t<T>> result;

    auto get_return_object() -> SyncWaitTask
    {
        return std::coroutine_handle<promise_type>::from_promise(*this);
    }

    void return_value(T value)
    {
        result = std::move(value);
    }

    decltype(auto) Get() &&
    {
        this->Wait();
        this->RethrowException();
        return std::move(*result);
    }
};

template <>
struct SyncWaitTask<void>::promise_type : SyncWaitTaskPromiseBase<void>
{
    auto get_return_object() -> SyncWaitTask<void>
    {
        return std::coroutine_handle<promise_type>::from_promise(*this);
    }

    void return_void()
    {
    }

    void Get() &&
    {
        this->Wait();
        this->RethrowException();
    }
};
}

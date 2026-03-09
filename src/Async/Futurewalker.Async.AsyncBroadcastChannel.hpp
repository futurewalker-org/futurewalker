// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Async.Concepts.hpp"

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Exception.hpp"
#include "Futurewalker.Core.Concepts.hpp"

#include <mutex>
#include <coroutine>
#include <vector>
#include <future>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Asynchronous broadcast channel.
///
/// Allows multiple consumers to wait for a value to be broadcasted.
/// Each consumer will receive a copy of the value when it is broadcasted.
///
template <Concepts::Copyable T>
class AsyncBroadcastChannel : NonCopyable
{
public:
    ~AsyncBroadcastChannel();

    auto Write(T const& value) -> Bool;
    auto Read();

private:
    struct Awaiter;
    struct Awaitable;
    void ResumeOrCancelAwaiters(std::vector<Awaiter> const& awaiters);

private:
    std::mutex _mutex;
    std::vector<Awaiter> _awaiters;
    std::promise<T> _promise;
};

template <Concepts::Copyable T>
struct AsyncBroadcastChannel<T>::Awaiter
{
    std::coroutine_handle<> handle;
    std::coroutine_handle<> (*cancel)(std::coroutine_handle<>);
};

template <Concepts::Copyable T>
struct [[nodiscard]] AsyncBroadcastChannel<T>::Awaitable : NonCopyable
{
    explicit Awaitable(AsyncBroadcastChannel& channel)
      : _channel {channel}
    {
        auto lock = std::unique_lock(_channel._mutex);
        _future = _channel._promise.get_future().share();
    }

    auto await_ready() const noexcept -> bool
    {
        return false;
    }

    template <class Promise>
    auto await_suspend(std::coroutine_handle<Promise> handle) noexcept -> std::coroutine_handle<>
    {
        if constexpr (Concepts::CancellablePromise<Promise>)
        {
            if (auto const continuation = handle.promise().cancel())
            {
                return continuation;
            }
            auto lock = std::unique_lock(_channel._mutex);
            _channel._awaiters.push_back({
                .handle = handle,
                .cancel = [](std::coroutine_handle<> h) { return std::coroutine_handle<Promise>::from_address(h.address()).promise().cancel(); },
            });
        }
        else
        {
            auto lock = std::unique_lock(_channel._mutex);
            _channel._awaiters.push_back({
                .handle = handle,
                .cancel = nullptr,
            });
        }
        return std::noop_coroutine();
    }

    auto await_resume() noexcept -> T
    {
        return _future.get();
    }

private:
    AsyncBroadcastChannel& _channel;
    std::shared_future<T> _future;
};

///
/// @brief Destructor.
///
template <Concepts::Copyable T>
AsyncBroadcastChannel<T>::~AsyncBroadcastChannel()
{
    if (!_awaiters.empty())
    {
        _promise.set_exception(std::make_exception_ptr(Exception(ErrorCode::OperationAborted)));
        ResumeOrCancelAwaiters(_awaiters);
    }
}

///
/// @brief Notifies all waiters with the given value.
///
/// Each waiter will receive a copy of the value.
/// Continuation of each awaiter will be resumed in caller's thread.
///
/// Caller's thread will be blocked until all awaiters are resumed.
///
/// @param[in] value Value to be broadcasted to all waiters.
///
/// @note Thread safe.
///
template <Concepts::Copyable T>
auto AsyncBroadcastChannel<T>::Write(T const& value) -> Bool
{
    auto lock = std::unique_lock(_mutex);
    if (!_awaiters.empty())
    {
        _promise.set_value(value);

        auto promise = std::exchange(_promise, {});
        auto awaiters = std::exchange(_awaiters, {});

        lock.unlock();
        ResumeOrCancelAwaiters(awaiters);
        return true;
    }
    return false;
}

///
/// @brief Asynchronously waits for next broadcast.
///
/// @return Awaitable object which resumes with the broadcasted value.
///
template <Concepts::Copyable T>
auto AsyncBroadcastChannel<T>::Read()
{
    return Awaitable(*this);
}

///
/// @brief Resumes or cancels all awaiters.
///
/// @param[in] awaiters List of awaiters to be resumed or cancelled.
///
template <Concepts::Copyable T>
void AsyncBroadcastChannel<T>::ResumeOrCancelAwaiters(std::vector<Awaiter> const& awaiters)
{
    for (auto const& awaiter : awaiters)
    {
        if (awaiter.cancel)
        {
            if (auto const continuation = awaiter.cancel(awaiter.handle))
            {
                continuation.resume();
                continue;
            }
        }
        awaiter.handle.resume();
    }
}
}
}

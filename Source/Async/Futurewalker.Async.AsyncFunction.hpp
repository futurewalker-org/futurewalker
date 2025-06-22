// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Async.Prelude.hpp"
#include "Futurewalker.Async.TaskHandle.hpp"
#include "Futurewalker.Async.SyncWaitTask.hpp"
#include "Futurewalker.Async.TypeTraits.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

#include <chrono>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Utility functions for asynchronous operations.
///
class AsyncFunction : NonConstructible
{
public:
    ///
    /// @brief Synchronously waits until awaitable operation completes.
    ///
    /// @param[in] awaitable An awaitable object.
    ///
    /// @return Result of awaitable operation.
    ///
    template <class Awaitable>
    static auto SyncWait(Awaitable&& awaitable)
    {
        struct DummyPromise {};
        constexpr auto makeSyncWaitTask = []<class A>(A&& a) -> SyncWaitTask<TypeTraits::AwaitResultOf<A, DummyPromise>> { co_return co_await std::forward<A>(a); };
        return makeSyncWaitTask(std::forward<Awaitable>(awaitable)).Get();
    }

    ///
    /// @brief Synchronously waits until awaitable operation completes.
    ///
    /// @param[in] invocable An invocable object which returns awaitable to be awaited.
    ///
    /// @return Result of awaitable operation.
    ///
    /// @note `invocable` will be copied/moved into awaiting coroutine's frame.
    ///
    template <class Invocable>
    static auto SyncWaitFn(Invocable&& invocable)
    {
        struct DummyPromise {};
        constexpr auto makeSyncWaitTask = []<class F>(F f) -> SyncWaitTask<TypeTraits::AwaitResultOf<std::invoke_result_t<F>, DummyPromise>> { co_return co_await std::invoke(f); };
        return makeSyncWaitTask(std::forward<Invocable>(invocable)).Get();
    }

    ///
    /// @brief Synchronously starts possibly asynchronous operation until first suspension, then returns control to caller without waiting completion.
    ///
    /// @param[in] awaitable An awaitable object.
    ///
    /// @return Handle to the spawned task.
    ///
    template <class Awaitable>
    static auto Spawn(Awaitable&& awaitable)
    {
        struct DummyPromise {};
        constexpr auto makeAsyncSpawnTask = []<class A>(A a) -> TaskHandle<TypeTraits::AwaitResultOf<A, DummyPromise>> { co_return co_await std::move(a); };
        return makeAsyncSpawnTask(std::forward<Awaitable>(awaitable));
    }

    ///
    /// @brief Synchronously starts possibly asynchronous operation until first suspension, then returns control to caller without waiting completion.
    ///
    /// @param[in] invocable An invocable object which returns awaitable to be awaited.
    ///
    /// @return Handle to the spawned task.
    ///
    /// @note `invocable` will be copied/moved into awaiting coroutine's frame.
    ///
    template <class Invocable>
    static auto SpawnFn(Invocable&& invocable)
    {
        struct DummyPromise {};
        constexpr auto makeAsyncSpawnTask = []<class F>(F f) -> TaskHandle<TypeTraits::AwaitResultOf<std::invoke_result_t<F>, DummyPromise>> { co_return co_await std::invoke(f); };
        return makeAsyncSpawnTask(std::forward<Invocable>(invocable));
    }

    ///
    /// @brief Copies invocable as parameter, then co_awaits on its result.
    ///
    /// @param invocable Invocable object.
    ///
    /// @return Awaitable which has same type to return value of invocable.
    ///
    template <class Invocable>
    static auto Fn(Invocable invocable) -> std::invoke_result_t<Invocable>
    {
        co_return co_await std::invoke(invocable);
    }
};
}
}

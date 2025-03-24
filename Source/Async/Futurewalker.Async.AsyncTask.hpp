// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Async.AsyncTaskType.hpp"
#include "Futurewalker.Async.LazyTaskType.hpp"
#include "Futurewalker.Async.AwaitableTraits.hpp"
#include "Futurewalker.Async.Task.hpp"

#include "Futurewalker.Core.Concepts.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Task type for structured concurrency.
///
/// @tparam T Result type
/// @tparam SingleThreaded true if the task is single threaded
///
template <class T, bool SingleThreaded>
class [[nodiscard]] AsyncTask : NonCopyable
{
    friend class LazyTask<T, SingleThreaded>;

public:
    struct promise_type : Task<T>::promise_type
    {
        auto get_return_object() -> AsyncTask<T, SingleThreaded>
        {
            return AsyncTask<T, SingleThreaded>(Task<T>::promise_type::get_return_object());
        }

        template <class U>
        requires (!SingleThreaded || AwaitableTraits<std::remove_reference_t<U>>::ResumesOnSameThread)
        auto await_transform(U&& u) -> decltype(auto)
        {
            return std::forward<U>(u);
        }
    };

    AsyncTask() = delete;

    ///
    /// @brief operator co_await
    ///
    /// @note We want to take `self` as value, but await_transform breaks it without NRVO.
    ///
    friend auto operator co_await(AsyncTask&& self)
    {
        return operator co_await(std::move(self._task));
    }

private:
    explicit AsyncTask(Task<T>&& task)
      : _task {std::move(task)}
    {
    }

    Task<T> _task;
};

template <class T, bool SingleThreaded>
struct AwaitableTraits<AsyncTask<T, SingleThreaded>>
{
    static auto constexpr ResumesOnSameThread = SingleThreaded;
};
}
}

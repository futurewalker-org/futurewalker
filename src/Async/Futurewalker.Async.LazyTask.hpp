// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Async.LazyTaskType.hpp"
#include "Futurewalker.Async.AsyncTask.hpp"
#include "Futurewalker.Async.AwaitableTraits.hpp"
#include "Futurewalker.Async.Task.hpp"

#include "Futurewalker.Core.Concepts.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Task type for unstructured concurrency.
///
/// @tparam T Result type
/// @tparam SingleThreaded true if the task is single-threaded
///
template <class T, bool SingleThreaded>
class [[nodiscard]] LazyTask : NonCopyable
{
public:
    struct promise_type : Task<T>::promise_type
    {
        auto get_return_object() -> LazyTask<T, SingleThreaded>
        {
            return LazyTask<T, SingleThreaded>(Task<T>::promise_type::get_return_object());
        }

        template <class U>
        requires (!SingleThreaded || AwaitableTraits<std::remove_reference_t<U>>::ResumesOnSameThread)
        auto await_transform(U&& u) -> decltype(auto)
        {
            return std::forward<U>(u);
        }
    };

    LazyTask() = delete;

    ///
    /// @brief Convert AsyncTask to LazyTask.
    ///
    explicit LazyTask(AsyncTask<T, SingleThreaded> async) noexcept
      : LazyTask(std::move(async._task))
    {
    }

    LazyTask(LazyTask&& other) noexcept
      : _task {std::move(other._task)}
    {
    }

    auto operator=(LazyTask&& other) noexcept -> LazyTask&
    {
        _task = std::move(other._task);
        return *this;
    }

    friend auto operator co_await(LazyTask&& self)
    {
        return operator co_await(std::move(self._task));
    }

private:
    explicit LazyTask(Task<T>&& task)
      : _task {std::move(task)}
    {
    }

    Task<T> _task;
};

template <class T, bool SingleThreaded>
struct AwaitableTraits<LazyTask<T, SingleThreaded>>
{
    static auto constexpr ResumesOnSameThread = SingleThreaded;
};
}
}

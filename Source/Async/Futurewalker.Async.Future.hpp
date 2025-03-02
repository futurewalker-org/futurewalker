// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Async.FutureType.hpp"
#include "Futurewalker.Async.AwaitableTraits.hpp"
#include "Futurewalker.Async.Task.hpp"
#include "Futurewalker.Async.Async.hpp" 

#include "Futurewalker.Core.Concepts.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Single threaded task type.
///
template <class T>
class [[nodiscard]] Future : NonCopyable
{
public:
    struct promise_type : Task<T>::promise_type
    {
        auto get_return_object() -> Future<T>
        {
            return Future<T>(Task<T>::promise_type::get_return_object());
        }

        template <class U>
        requires AwaitableTraits<std::remove_reference_t<U>>::ResumesOnSameThread
        auto await_transform(U&& u) -> decltype(auto)
        {
            return std::forward<U>(u);
        }
    };

    ///
    /// @brief Convert Async to Future.
    ///
    explicit Future(Async<T> async) noexcept
      : Future(std::move(async._task))
    {
    }

    Future(Future&& other) noexcept
      : _task {std::move(other._task)}
    {
    }

    auto operator=(Future&& other) noexcept -> Future&
    {
        _task = std::move(other._task);
        return *this;
    }

    friend auto operator co_await(Future&& self)
    {
        return operator co_await(std::move(self._task));
    }

private:
    explicit Future(Task<T>&& task)
      : _task {std::move(task)}
    {
    }

    Task<T> _task;
};

template <class T>
struct AwaitableTraits<Future<T>>
{
    static constexpr bool ResumesOnSameThread = true;
};
}
}

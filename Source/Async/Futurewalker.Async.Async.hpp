// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Async.AsyncType.hpp"
#include "Futurewalker.Async.AwaitableTraits.hpp"
#include "Futurewalker.Async.Task.hpp"
#include "Futurewalker.Async.FutureType.hpp"

#include "Futurewalker.Core.Concepts.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Single threaded task type (non-movable).
///
template <class T>
class [[nodiscard]] Async : NonCopyable
{
    friend class Future<T>;

public:
    struct promise_type : Task<T>::promise_type
    {
        auto get_return_object() -> Async<T>
        {
            return Async<T>(Task<T>::promise_type::get_return_object());
        }

        template <class U>
        requires AwaitableTraits<std::remove_reference_t<U>>::ResumesOnSameThread
        auto await_transform(U&& u) -> decltype(auto)
        {
            return std::forward<U>(u);
        }
    };

    ///
    /// @brief operator co_await
    ///
    /// @note We want to take `self` as value, but await_transform breaks it without NRVO.
    ///
    friend auto operator co_await(Async&& self)
    {
        return operator co_await(std::move(self._task));
    }

    Async(const Async&) = delete;

private:
    explicit Async(Task<T>&& task)
      : _task {std::move(task)}
    {
    }

    Task<T> _task;
};

template <class T>
struct AwaitableTraits<Async<T>>
{
    static constexpr bool ResumesOnSameThread = true;
};
}
}

// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Async.Async.hpp"
#include "Futurewalker.Async.ResumesOnSameThreadTask.hpp"
#include "Futurewalker.Async.AsyncFunction.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Blank.hpp"

#include <thread>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Provides access to the current thread's scheduler.
///
struct ThisThread : NonConstructible
{
    struct Scheduler : NonCopyable
    {
        virtual ~Scheduler() = 0;
        virtual auto Schedule() -> AsyncTask<void> = 0;
        virtual auto ScheduleAfter(std::chrono::nanoseconds const delay) -> AsyncTask<void> = 0;
    };
    static auto SetScheduler(Shared<Scheduler> const& scheduler) -> void;
    static auto GetScheduler() -> Shared<Scheduler>;

    ///
    /// @brief Schedule continuation on current thread's scheduler.
    ///
    /// @throw Exception if no scheduler is associated with this thread.
    ///
    static auto Schedule() -> Async<void>
    {
        if (auto const scheduler = GetScheduler())
        {
            co_return co_await AsyncFunction::Fn([&]() -> ResumesOnSameThreadTask<void> { co_await scheduler->Schedule(); });
        }
        throw Exception(ErrorCode::InvalidOperation, "No scheduler associated with this thread");
    }

    ///
    /// @brief Schedule continuation on current thread's scheduler after a delay.
    ///
    /// @throw Exception if no scheduler is associated with this thread.
    ///
    template <class Rep, class Period>
    static auto ScheduleAfter(std::chrono::duration<Rep, Period> const delay) -> Async<void>
    {
        if (auto const scheduler = GetScheduler())
        {
            co_return co_await AsyncFunction::Fn([&]() -> ResumesOnSameThreadTask<void> { co_await scheduler->ScheduleAfter(std::chrono::duration_cast<std::chrono::nanoseconds>(delay)); });
        }
        throw Exception(ErrorCode::InvalidOperation, "No scheduler associated with this thread");
    }

    ///
    /// @brief Resume execution on the current thread's scheduler after awaiting the given awaitable.
    ///
    /// @param[in] awaitable Awaitable to await.
    ///
    /// @note If the awaitable resumes on the same thread, no additional scheduling is performed.
    ///
    /// @throw Exception if no scheduler is associated with this thread.
    ///
    template <class Awaitable>
    static auto Resume(Awaitable&& awaitable)
    {
        using AwaitResult = TypeTraits::AwaitResultOf<Awaitable, Blank>;
        constexpr auto makeResumesOnSameThreadTask = []<class A>(A&& a) -> ResumesOnSameThreadTask<AwaitResult> {
            auto const thisThreadScheduler = GetScheduler();
            if (!thisThreadScheduler)
            {
                throw Exception(ErrorCode::InvalidOperation, "No scheduler associated with this thread");
            }

            if constexpr (std::is_void_v<AwaitResult>)
            {
                co_await std::forward<A>(a);
                auto const otherThreadScheduler = GetScheduler();
                if (thisThreadScheduler != otherThreadScheduler)
                {
                    co_await thisThreadScheduler->Schedule();
                }
            }
            else
            {
                auto result = co_await std::forward<A>(a);
                auto const otherThreadScheduler = GetScheduler();
                if (thisThreadScheduler != otherThreadScheduler)
                {
                    co_await thisThreadScheduler->Schedule();
                    co_return result;
                }
                co_return result;
            }
        };
        return makeResumesOnSameThreadTask(std::forward<Awaitable>(awaitable));
    }
};
}
}

// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.Prelude.hpp"
#include "Futurewalker.Application.PlatformEventLoopContext.hpp"
#include "Futurewalker.Application.PlatformEventLoop.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Async.Async.hpp"
#include "Futurewalker.Async.AsyncFunction.hpp"
#include "Futurewalker.Async.ResumesOnSameThreadTask.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"
#include "Futurewalker.Core.Exception.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief This thread.
///
class ThisThread : NonConstructible
{
public:
    static auto Schedule() -> Async<void>;

    template <class Rep, class Period>
    static auto ScheduleAfter(const std::chrono::duration<Rep, Period>& delay) -> Async<void>;

    template <class Awaitable>
    static auto ResumeAfter(Awaitable&& awaitable) -> Async<void>;

    template <class F>
    static auto ResumeAfterFn(F&& f) -> Async<void>;
};

///
/// @brief
///
/// @param delay
///
template <class Rep, class Period>
auto ThisThread::ScheduleAfter(const std::chrono::duration<Rep, Period>& delay) -> Async<void>
{
    if (auto context = Locator::GetInstance<PlatformEventLoopContext>())
    {
        if (auto loop = context->GetEventLoopForThread())
        {
            co_return co_await ResumesOnSameThreadTask(LazyTask(loop->ScheduleAfter(delay)));
        }
    }
    throw Exception(ErrorCode::Failure);
}

///
/// @brief Resumes on this thread after other tasks.
///
/// @param[in] awaitable
///
template <class Awaitable>
auto ThisThread::ResumeAfter(Awaitable&& awaitable) -> Async<void>
{
    if (auto context = Locator::GetInstance<PlatformEventLoopContext>())
    {
        if (auto loop = context->GetEventLoopForThread())
        {
            const auto tid = std::this_thread::get_id();
            co_await ResumesOnSameThreadTask([](auto awaitable, auto loop, auto tid) mutable -> LazyTask<void> {
                co_await std::move(awaitable);

                if (std::this_thread::get_id() != tid)
                {
                    co_await loop->Schedule();
                }
            }(std::forward<Awaitable>(awaitable), loop, tid));
        }
    }
}

///
/// @brief
///
/// @param[in] f
/// @param[in] forceSchedule
///
template <class F>
auto ThisThread::ResumeAfterFn(F&& f) -> Async<void>
{
    co_await ResumeAfter(AsyncFunction::Fn(std::forward<F>(f)));
}
}
}

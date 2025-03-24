// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.NonConstructible.hpp"

#include "Futurewalker.Async.ThreadPool.hpp"
#include "Futurewalker.Async.AsyncTask.hpp"

#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Main thread.
///
class MainThread : NonConstructible
{
public:
    static auto IsMainThread() -> Bool;

    static auto Schedule() -> AsyncTask<void>;

    template <class Rep, class Period>
    static auto ScheduleAfter(const std::chrono::duration<Rep, Period>& delay) -> AsyncTask<void>;

private:
    static auto ScheduleAfterNanos(const std::chrono::nanoseconds& delay) -> AsyncTask<void>;
};

///
/// @brief 
///
/// @param delay 
///
template <class Rep, class Period>
auto MainThread::ScheduleAfter(const std::chrono::duration<Rep, Period>& delay) -> AsyncTask<void>
{
    co_await ScheduleAfterNanos(std::chrono::duration_cast<std::chrono::nanoseconds>(delay));
}
}
}

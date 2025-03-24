// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.MainThread.hpp"
#include "Futurewalker.Application.PlatformMainThread.hpp"

#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Check if current thread is main thread or not.
///
/// @return true when current calling thread is main thread, otherwise returns false.
///
/// @note This function is thread safe.
///
auto MainThread::IsMainThread() -> Bool
{
    if (auto const platform = Locator::GetInstance<PlatformMainThread>())
    {
        return platform->IsMainThread();
    }
    FW_DEBUG_ASSERT(false);
    return false;
}

///
/// @brief Schedule task to the main thread.
///
auto MainThread::Schedule() -> AsyncTask<void>
{
    if (auto const platform = Locator::GetInstance<PlatformMainThread>())
    {
        co_return co_await platform->Schedule();
    }
    FW_DEBUG_ASSERT(false);
}

///
/// @brief Schedule task to the main thread after delay.
///
/// @param delay
///
auto MainThread::ScheduleAfterNanos(const std::chrono::nanoseconds& delay) -> AsyncTask<void>
{
    if (auto const platform = Locator::GetInstance<PlatformMainThread>())
    {
        co_return co_await platform->ScheduleAfter(delay);
    }
    FW_DEBUG_ASSERT(false);
}
}

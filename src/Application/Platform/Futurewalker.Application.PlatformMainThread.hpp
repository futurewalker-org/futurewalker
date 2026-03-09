// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformMainThreadType.hpp"

#include "Futurewalker.Async.AsyncTask.hpp"

#include "Futurewalker.Core.Memory.hpp"

#include <chrono>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Main thread.
///
class PlatformMainThread : NonCopyable
{
public:
    virtual ~PlatformMainThread() = 0;

    ///
    /// @brief Check if current thread is main thread.
    ///
    virtual auto IsMainThread() const -> Bool = 0;

    ///
    /// @brief Schedule task to the main thread.
    ///
    virtual auto Schedule() -> AsyncTask<void> = 0;

    ///
    /// @brief Schedule task to the main thread after delay.
    ///
    virtual auto ScheduleAfter(const std::chrono::nanoseconds& delay) -> AsyncTask<void> = 0;
};
}
}

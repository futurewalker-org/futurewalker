// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.PlatformEventLoopType.hpp"

#include "Futurewalker.Async.Async.hpp"
#include "Futurewalker.Async.AsyncTask.hpp"
#include "Futurewalker.Async.Lazy.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Function.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Platform abstraction of event loop.
///
/// @note Each thread can have at most one PlatformEventLoop object attached to it.
/// @note Use PlatformEventLoopContext to create PlatformEventLoop object.
///
class PlatformEventLoop : NonCopyable
{
public:
    virtual ~PlatformEventLoop() = 0;

    struct RunDelegate
    {
        Function<Async<void>()> startTask;
        Function<Async<void>()> exitTask;
    };

    ///
    /// @brief Consumes each event in event queue until Exit() is called.
    ///
    /// @param[in] delegate Delegate
    ///
    /// @note This function awaits until event loop exists.
    /// @note This function should be called from the thread this event loop belongs to.
    ///
    virtual auto Run(RunDelegate const& delegate) -> Async<void> = 0;

    ///
    /// @brief Exits currently running event loop on this queue.
    ///
    /// @note This function should be called from the thread this event loop belongs to.
    ///
    virtual auto Exit() -> void = 0;

    ///
    /// @brief Schedule task to this queue.
    ///
    /// @note Posted task will be run in event loop in order.
    /// @note This function can be called from any thread.
    ///
    /// @throw Exception when the thread this event loop belongs to was already exited.
    ///
    virtual auto Schedule() -> AsyncTask<void> = 0;

    ///
    /// @brief Schedule task to this queue after delay.
    ///
    /// @param[in] delay Delay
    ///
    /// @note This function can be called from any thread.
    ///
    /// @throw Exception when the thread this event loop belongs to was already exited.
    ///
    virtual auto ScheduleAfter(const std::chrono::nanoseconds& delay) -> AsyncTask<void> = 0;
};
}
}

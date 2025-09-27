// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Async.ThisThread.hpp"

namespace FW_DETAIL_NS
{
namespace
{
thread_local Shared<ThisThread::Scheduler> _scheduler;
}

ThisThread::Scheduler::~Scheduler() = default;

///
/// @brief Associate a scheduler with the current thread.
///
/// @note Each scheduler must be associated with only one thread.
///
auto ThisThread::SetScheduler(Shared<Scheduler> const& scheduler) -> void
{
    _scheduler = scheduler;
}

///
/// @brief Get scheduler associated with the current thread.
///
auto ThisThread::GetScheduler() -> Shared<Scheduler>
{
    return _scheduler;
}
}

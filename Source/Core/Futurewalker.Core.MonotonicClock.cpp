// SPDX-License-Identifier: MIT

#include "Futurewalker.Core.MonotonicClock.hpp"

#include <chrono>

namespace FW_DETAIL_NS
{
///
/// @brief Get current monotonic time.
///
auto MonotonicClock::GetCurrentTime() -> MonotonicTime
{
    const auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch());
    return MonotonicTime(nanos.count() * 1e-9);
}
}

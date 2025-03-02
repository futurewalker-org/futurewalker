// SPDX-License-Identifier: MIT

#include "Futurewalker.Core.SystemClock.hpp"

#include <chrono>

namespace FW_DETAIL_NS
{
///
/// @brief Get current system time.
///
auto SystemClock::GetCurrentTime() -> SystemTime
{
    const auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
    return SystemTime(nanos.count() * 1e-9);
}
}

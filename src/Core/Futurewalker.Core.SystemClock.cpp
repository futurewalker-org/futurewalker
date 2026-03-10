// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Core.SystemClock.hpp"

#include <chrono>

namespace FW_DETAIL_NS
{
///
/// @brief Get current system time.
///
auto SystemClock::GetNow() -> SystemTime
{
    auto const nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
    return SystemTime::MakeFromNanoseconds(nanos.count());
}
}

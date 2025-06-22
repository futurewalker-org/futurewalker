// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.MonotonicClockType.hpp"
#include "Futurewalker.Core.TimePoint.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Time point of monotonic clocks.
///
/// @note Do not rely on epoch of MonotonicTime as it may differ on each execution.
///
using MonotonicTime = TimePoint<MonotonicClock>;
}
}

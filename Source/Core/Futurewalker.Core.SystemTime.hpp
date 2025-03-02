// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.SystemClockType.hpp"
#include "Futurewalker.Core.TimePoint.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Represents system time in Unix time.
///
using SystemTime = TimePoint<SystemClock>;
}
}

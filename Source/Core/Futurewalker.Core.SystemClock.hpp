// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.SystemClockType.hpp"
#include "Futurewalker.Core.SystemTime.hpp"
#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief System time clock.
///
class SystemClock : NonConstructible
{
public:
    static auto GetCurrentTime() -> SystemTime;
};
}
}

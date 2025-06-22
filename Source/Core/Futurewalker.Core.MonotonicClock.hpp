// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.MonotonicClockType.hpp"
#include "Futurewalker.Core.MonotonicTime.hpp"
#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief A clock which never rolls back during execution.
///
class MonotonicClock : NonConstructible
{
public:
    static auto GetNow() -> MonotonicTime;
};
}
}

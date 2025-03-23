// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Win.Prelude.hpp"

#include <WinUser.h>

namespace FW_DETAIL_NS
{
inline constexpr auto WM_FW_EVENT_QUEUE_POST = WM_APP + 0;
inline constexpr auto WM_FW_EVENT_QUEUE_WAKE = WM_APP + 1;
}

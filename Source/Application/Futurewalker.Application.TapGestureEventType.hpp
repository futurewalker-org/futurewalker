// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class TapGestureEvent;

///
/// @brief Event type of tap gesture.
///
enum class TapGestureEventType
{
    Down,
    Up,
    Tap,
};
}
}

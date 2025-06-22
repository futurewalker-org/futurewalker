// SPDX-License-Identifier: MPL-2.0
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

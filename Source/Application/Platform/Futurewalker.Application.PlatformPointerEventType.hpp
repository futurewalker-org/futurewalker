// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.Platform.Prelude.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformPointerEvent;

enum PlatformPointerEventType
{
    Down,
    Up,
    Enter,
    Leave,
    Move,
    Cancel,
};
}
}

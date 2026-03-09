// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.Prelude.hpp"

#include "Futurewalker.Core.EnumClassFlag.hpp" 

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformKeyboardLayoutWin;

enum class PlatformModifierFlagsWin
{
    None = 0,
    Shift = 1 << 0,
    Control = 1 << 1,
    LAlt = 1 << 2,
    RAlt = 1 << 3, // AltGr = Control + RAlt
    All = Shift | Control | LAlt | RAlt,
};
FW_ENABLE_ENUM_CLASS_BITWISE_OPERATORS(PlatformModifierFlagsWin);
}
}

// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

#include "Futurewalker.Core.EnumClassFlag.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class TapGestureRecognizer;

///
/// @brief Action flags for tap gesture recognizer.
///
enum class TapGestureActionFlags
{
    None = 0,
    SingleTap = 1 << 0,
    DoubleTap = 1 << 1,
};
FW_ENABLE_ENUM_CLASS_BITWISE_OPERATORS(TapGestureActionFlags);
}
}

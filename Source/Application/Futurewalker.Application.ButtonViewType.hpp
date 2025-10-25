// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

#include "Futurewalker.Core.EnumClassFlag.hpp"
namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class ButtonView;

///
/// @brief Action flags for button view.
///
enum class ButtonViewActionFlags
{
    None = 0,
    Press = 1 << 0,
};
FW_ENABLE_ENUM_CLASS_BITWISE_OPERATORS(ButtonViewActionFlags);
}
}

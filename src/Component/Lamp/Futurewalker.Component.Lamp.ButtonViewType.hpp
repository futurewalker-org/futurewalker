// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.Prelude.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
class ButtonView;

///
/// @brief Action flags for button view.
///
enum class ButtonViewActionFlag
{
    None = 0,
    Press = 1 << 0,
};
}
}

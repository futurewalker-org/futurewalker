// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class KeyEvent;

///
/// @brief Flags for modifier keys.
///
enum class ModifierKeyFlag : uint32_t
{
    None = 0,
    Shift = 1 << 0,
    Control = 1 << 1,
    Alt = 1 << 2,
    Meta = 1 << 3,
};
}
}

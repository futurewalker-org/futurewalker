// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class TapGestureRecognizer;

///
/// @brief Action flags for tap gesture recognizer.
///
enum class TapGestureActionFlag
{
    None = 0,
    SingleTap = 1 << 0,
    DoubleTap = 1 << 1,
};
}
}

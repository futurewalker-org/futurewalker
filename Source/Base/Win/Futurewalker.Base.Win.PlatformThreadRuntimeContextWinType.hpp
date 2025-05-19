// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Base.Win.Prelude.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformThreadRuntimeContextWin;

///
/// @brief COM runtime threading mode.
///
enum class PlatformThreadRuntimeInitTypeWin
{
    SingleThreaded,
    MultiThreaded,
};
}
}

// SPDX-License-Identifier: MPL-2.0
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

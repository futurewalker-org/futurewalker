// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Base.Win.PlatformThreadRuntimeContextWinType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Manages per-thread runtime initialization.
///
class PlatformThreadRuntimeContextWin : NonCopyable
{
public:
    PlatformThreadRuntimeContextWin(PlatformThreadRuntimeInitTypeWin initType);
    virtual ~PlatformThreadRuntimeContextWin();
};
}
}

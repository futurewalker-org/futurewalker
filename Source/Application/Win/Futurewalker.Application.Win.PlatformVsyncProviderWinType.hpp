// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.Prelude.hpp"

#include "Futurewalker.Core.Function.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.MonotonicTime.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformVsyncProviderWin;

///
/// @brief Vsync frame info.
///
struct PlatformVsyncFrameInfo
{
    ///
    /// @brief Frame timestamp for next presentation.
    ///
    MonotonicTime targetTimestamp;
};

///
/// @brief Vsync callback function.
///
using PlatformVsyncCallbackFunction = Function<void(Shared<void> userData, PlatformVsyncFrameInfo const& frameInfo)>;
}
}

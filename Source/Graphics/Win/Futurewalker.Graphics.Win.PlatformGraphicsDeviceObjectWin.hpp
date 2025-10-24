// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.Win.PlatformGraphicsDeviceObjectWinType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Graphics device object.
///
class PlatformGraphicsDeviceObjectWin  : NonCopyable
{
public:
    virtual ~PlatformGraphicsDeviceObjectWin() = 0;

    ///
    /// @brief Notify operation was failed due to device loss.
    ///
    virtual auto NotifyDeviceLost() -> void = 0;

    ///
    /// @brief Handle device loss.
    ///
    virtual auto HandleDeviceLost() -> void = 0;
};
}
}

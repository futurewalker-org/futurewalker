// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Event.Prelude.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class EventParameter;

///
/// @brief Describes in which stage events are being propagated.
///
enum class EventPhase
{
    None,

    ///
    /// @brief Events are being propagated forward to target.
    ///
    Capture,

    ///
    /// @brief Events are being sent to target.
    ///
    Target,

    ///
    /// @brief Events are beging propagated back to origin.
    ///
    //Bubble,
};
}
}

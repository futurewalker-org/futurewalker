// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class Popup;

///
/// @brief Options for popup.
///
struct PopupOptions
{
    ///
    /// @brief Whether the popup has a system rendered frame if available.
    ///
    Bool hasFrame = false;

    ///
    /// @brief Whether the popup is allowed to be active when its owner is active.
    ///
    Bool allowActiveOwnerPopup = false;
};
}
}

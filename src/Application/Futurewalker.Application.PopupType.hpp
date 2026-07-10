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

///
/// @brief Edge of the anchor rectangle to which the popup is anchored.
///
enum class PopupAnchorEdge
{
    Leading,
    Top,
    Trailing,
    Bottom,
};

///
/// @brief Alignment of popup along the anchor edge.
///
enum class PopupAnchorAlignment
{
    Start,
    Center,
    End,
};
}
}

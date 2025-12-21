// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.Prelude.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
class PopupMenu;

///
/// @brief Edge of the anchor rectangle to which the popup menu is anchored.
///
enum class PopupMenuAnchorEdge
{
    Leading,
    Top,
    Trailing,
    Bottom,
};

///
/// @brief Alignment of popup menu along the anchor edge.
///
enum class PopupMenuAnchorAlignment
{
    Start,
    Center,
    End,
};
}
}

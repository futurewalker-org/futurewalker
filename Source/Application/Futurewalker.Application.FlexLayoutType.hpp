// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class FlexLayout;

using FlexFactor = Float<float64_t, FlexLayout>;

///
/// @brief Direction of main axis of FlexLayout.
///
enum class FlexLayoutDirection
{
    Column,        // Vertical forward direction.
    ColumnReverse, // Vertical backward direction.
    Row,           // Horizontal forward direction.
    RowReverse,    // Horizontal backward direction.
};

///
/// @brief Main axis size of FlexLayout.
///
enum class FlexLayoutMainAxisSize
{
    Min, // Minimize size to wrap all children.
    Max, // Fill available space if possible, otherwise fallback to Min.
};

///
/// @brief Cross axis size of FlexLayout.
///
enum class FlexLayoutCrossAxisSize
{
    Min, // Minimum size to wrap all children.
    Max, // Fill available space if possible, otherwise fallback to Min.
};

///
/// @brief Main axis alignment of FlexLayout.
///
enum class FlexLayoutMainAxisAlignment
{
    Start,        // Align to starting edge.
    End,          // Align to ending edge.
    Center,       // Align to center.
    SpaceBetween, // Equally distribute remaining space between items.
    SpaceAround,  // Similar to SpaceBetween, but each edge will also receive half of space between items.
};

///
/// @brief Cross axis alignment of FlexLayout.
///
enum class FlexLayoutCrossAxisAlignment
{
    Start,   // Align to starting edge.
    End,     // Align to ending edge.
    Center,  // Align to center.
    Stretch, // Stretch all items to fill cross axis.
};

///
/// @brief Main axis flexibility of FlexLayout child.
///
enum class FlexLayoutMainAxisFlexibility
{
    Flexible, // Use loose constraints.
    Expand,   // Use tight constraints to fill space.
};
}
}

// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Text.Prelude.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Direct of text selection.
///
/// The end of selection in the direction is the active end of selected range.
/// The active end of selection is used for some user interactions such as shift+arrow key selection.
///
enum class TextSelectionDirection
{
    None = 0,
    Forward,
    Backward,
};
}
}

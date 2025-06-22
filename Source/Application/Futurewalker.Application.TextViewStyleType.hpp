// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class TextViewStyle;

///
/// @brief Horizontal alignment of text.
///
enum class TextViewHorizontalAlignment
{
    Leading,
    Center,
    Trailing,
};

///
/// @brief Vertical alignment of text.
///
enum class TextViewVerticalAlignment
{
    Top,
    Middle,
    Bottom,
};
}
}
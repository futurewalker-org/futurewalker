// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Color.Prelude.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
template <class Tag>
class RGBColorT;

///
/// @brief Generic RGB color.
///
/// @note This class does not carry information about the color space in which the channels are encoded.
///
using RGBColor = RGBColorT<void>;
}
}

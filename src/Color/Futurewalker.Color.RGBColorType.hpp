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
using RGBColor = RGBColorT<void>;
}
}

// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Color.Prelude.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
template <class Tag>
class RGBAColorT;

///
/// @brief Generic RGBA color.
///
using RGBAColor = RGBAColorT<void>;
}
}

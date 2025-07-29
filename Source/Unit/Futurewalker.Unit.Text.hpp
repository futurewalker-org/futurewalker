// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Identifier.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Count or index of Unicode code points.
///
using CodePoint = Integer<int64_t, class UnitTagCodePoint>;

///
/// @brief Count or index of Unicode code units.
///
using CodeUnit = Integer<int64_t, class UnitTagCodeUnit>;
}
}

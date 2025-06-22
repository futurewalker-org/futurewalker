// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Degree of angle.
///
using Degree = Float<float64_t, class UnitTagDegree>;

///
/// @brief Radian of angle.
///
using Radian = Float<float64_t, class UnitTagRadian>;
}
}

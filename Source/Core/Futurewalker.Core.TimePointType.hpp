// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
template <class Clock>
class TimePoint;

///
/// @brief Time interval in seconds.
///
using TimeInterval = Float<float64_t, class UnitTagTimeInterval>;
}
}

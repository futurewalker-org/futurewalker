// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Animation.AnimationInterpolation.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Make linear interpolation function.
///
auto AnimationInterpolation::MakeLinear() -> AnimationInterpolationFunction
{
    return [](Float64 t) -> Float64 { return Float64::IsFinite(t) ? t : 0.0; };
}
}

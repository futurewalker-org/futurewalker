// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Animation.AnimationInterpolationType.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Animation interpolation functions.
///
struct AnimationInterpolation : NonConstructible
{
    static auto MakeLinear() -> AnimationInterpolationFunction;
};
}
}

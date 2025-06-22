// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Animation.Prelude.hpp"

#include "Futurewalker.Core.Memory.hpp"

#include <list>

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class AnimationTimer;
using AnimationTimerList = std::list<Shared<AnimationTimer>>;
}
}

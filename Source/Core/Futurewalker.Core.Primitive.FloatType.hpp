// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Core.Prelude.hpp"
#include "Futurewalker.Core.Concepts.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
template <Concepts::FloatingPoint T, class Tag = void>
class Float;

using Float32 = Float<float32_t>;
using Float64 = Float<float64_t>;
}
}

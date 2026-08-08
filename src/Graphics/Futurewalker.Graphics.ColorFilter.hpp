// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.ColorFilterType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"

#include "Futurewalker.Geometry.Matrix.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Color filter.
///
class ColorFilter : NonCopyable
{
public:
    static auto MakeMatrixFilter(Matrix5x5<Float64> const& matrix, Bool const clamp) -> Shared<ColorFilter>;

    virtual ~ColorFilter() = 0;
};
}
}

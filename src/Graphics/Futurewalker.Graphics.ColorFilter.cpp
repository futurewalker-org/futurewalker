// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.ColorFilter.hpp"
#include "Futurewalker.Graphics.SkiaColorFilter.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
///
/// @brief
///
auto ColorFilter::MakeMatrixFilter(Matrix5x5<Float64> const& matrix, Bool const clamp) -> Shared<ColorFilter>
{
    return SkiaColorFilter::MakeMatrixFilter(matrix, clamp);
}

ColorFilter::~ColorFilter() = default;
}

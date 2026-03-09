// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.MaskFilter.hpp"
#include "Futurewalker.Graphics.SkiaMaskFilter.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
///
/// @brief Create a blur mask filter.
///
/// @param style Blur style.
/// @param sigma Standard deviation of blur.
///
auto MaskFilter::MakeBlur(BlurStyle const style, Dp const sigma) -> Shared<MaskFilter>
{
    return SkiaMaskFilter::MakeBlur(style, sigma);
}

MaskFilter::~MaskFilter() = default;
}

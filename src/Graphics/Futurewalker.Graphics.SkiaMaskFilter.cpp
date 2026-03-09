// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.SkiaMaskFilter.hpp"
#include "Futurewalker.Graphics.SkiaFunction.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
auto SkiaMaskFilter::MakeBlur(BlurStyle const style, Dp const sigma) -> Shared<SkiaMaskFilter>
{
    auto const skiaStyle = SkiaFunction::BlurStyleToSkBlurStyle(style);
    auto const skiaSigma = static_cast<SkScalar>(sigma);
    auto const skiaFilter = SkMaskFilter::MakeBlur(skiaStyle, skiaSigma);
    return Shared<SkiaMaskFilter>::Make(skiaFilter);
}

SkiaMaskFilter::SkiaMaskFilter(sk_sp<SkMaskFilter> filter)
  : _filter {std::move(filter)}
{
}

auto SkiaMaskFilter::GetSkiaMaskFilter() const -> sk_sp<SkMaskFilter>
{
    return _filter;
}
}

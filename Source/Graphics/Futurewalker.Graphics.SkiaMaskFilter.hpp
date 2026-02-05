// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.MaskFilter.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.Memory.hpp"

#include <include/core/SkMaskFilter.h>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
class SkiaMaskFilter : public MaskFilter
{
public:
    static auto MakeBlur(BlurStyle const style, Dp const sigma) -> Shared<SkiaMaskFilter>;

    explicit SkiaMaskFilter(sk_sp<SkMaskFilter> filter);

    auto GetSkiaMaskFilter() const -> sk_sp<SkMaskFilter>;

private:
    sk_sp<SkMaskFilter> _filter;
};
}
}

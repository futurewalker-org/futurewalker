// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.ColorFilter.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.Memory.hpp"

#include <include/core/SkColorFilter.h>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
class SkiaColorFilter : public ColorFilter
{
public:
    static auto MakeMatrixFilter(Matrix5x5<Float64> const& matrix, Bool const clamp) -> Shared<SkiaColorFilter>;

    explicit SkiaColorFilter(sk_sp<SkColorFilter> filter);

    auto GetSkiaColorFilter() const -> sk_sp<SkColorFilter>;

private:
    sk_sp<SkColorFilter> _filter;
};
}
}

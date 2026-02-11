// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.SkiaTypefaceType.hpp"
#include "Futurewalker.Graphics.Typeface.hpp"

#include <include/core/SkTypeface.h>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
class SkiaTypeface final : public Typeface
{
public:
    explicit SkiaTypeface(sk_sp<SkTypeface> const& typeface);

    auto GetMetrics(FontSize const size) const -> FontMetrics override;

    auto GetSkTypeface() const -> sk_sp<SkTypeface>;

private:
    sk_sp<SkTypeface> _typeface;
};
}
}

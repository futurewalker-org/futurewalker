// SPDX-License-Identifier: MIT

#include "Futurewalker.Graphics.SkiaTypeface.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
SkiaTypeface::SkiaTypeface(sk_sp<SkTypeface> const& typeface)
  : _typeface {typeface}
{
}

auto SkiaTypeface::GetTypeface() const -> sk_sp<SkTypeface>
{
    return _typeface;
}
}

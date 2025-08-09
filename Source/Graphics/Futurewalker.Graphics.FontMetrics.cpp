// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.FontMetrics.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
auto FontMetrics::GetAscent() const -> Dp
{
    return _ascent;
}

auto FontMetrics::SetAscent(Dp const ascent) -> void
{
    _ascent = ascent;
}

auto FontMetrics::GetDescent() const -> Dp
{
    return _descent;
}

auto FontMetrics::SetDescent(Dp const descent) -> void
{
    _descent = descent;
}

auto FontMetrics::GetLeading() const -> Dp
{
    return _leading;
}

auto FontMetrics::SetLeading(Dp const leading) -> void
{
    _leading = leading;
}
}

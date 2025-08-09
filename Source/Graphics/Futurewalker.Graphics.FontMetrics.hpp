// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.FontMetricsType.hpp"

#include "Futurewalker.Unit.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Layout metrics of a font.
///
class FontMetrics
{
public:
    auto GetAscent() const -> Dp;
    auto SetAscent(Dp const ascent) -> void;

    auto GetDescent() const -> Dp;
    auto SetDescent(Dp const descent) -> void;

    auto GetLeading() const -> Dp;
    auto SetLeading(Dp const leading) -> void;

private:
    Dp _ascent = 0;
    Dp _descent = 0;
    Dp _leading = 0;
};
}
}

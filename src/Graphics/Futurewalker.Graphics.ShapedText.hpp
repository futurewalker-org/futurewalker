// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.ShapedTextType.hpp"
#include "Futurewalker.Graphics.GlyphRunType.hpp"
#include "Futurewalker.Graphics.FontMetrics.hpp"
#include "Futurewalker.Graphics.Glyph.hpp"
#include "Futurewalker.Graphics.Scene.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.Optional.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Geometry.hpp"

#include <vector>
#include <span>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Represents result of text shaping.
///
class ShapedText
{
public:
    class Line
    {
    public:
        auto GetMetrics() const -> FontMetrics;
        auto SetMetrics(FontMetrics const& metrics) -> void;

        auto GetAdvance() const -> Dp;
        auto SetAdvance(Dp const advance) -> void;

        auto GetRunCount() const -> SInt64;
        auto GetRuns() const -> std::vector<Shared<GlyphRun>>;
        auto SetRuns(std::vector<Shared<GlyphRun>> const& runs) -> void;

    private:
        FontMetrics _metrics;
        Dp _advance = 0;
        std::vector<Shared<GlyphRun>> _runs;
    };

    explicit ShapedText(std::span<Line const> liens);

    auto GetLineCount() const -> SInt64;
    auto GetLines() const -> std::vector<Line> const&;

private:
    std::vector<Line> _lines;
};
}
}

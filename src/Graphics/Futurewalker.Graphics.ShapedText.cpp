// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.ShapedText.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
auto ShapedText::Line::GetMetrics() const -> FontMetrics
{
    return _metrics;
}

auto ShapedText::Line::SetMetrics(FontMetrics const& metrics) -> void
{
    _metrics = metrics;
}

auto ShapedText::Line::GetAdvance() const -> Dp
{
    return _advance;
}

auto ShapedText::Line::SetAdvance(Dp const advance) -> void
{
    _advance = advance;
}

auto ShapedText::Line::GetRunCount() const -> SInt64
{
    return std::ssize(_runs);
}

auto ShapedText::Line::GetRuns() const -> std::vector<Shared<GlyphRun>>
{
    return _runs;
}

auto ShapedText::Line::SetRuns(std::vector<Shared<GlyphRun>> const& runs) -> void
{
    _runs = runs;
}

ShapedText::ShapedText(std::span<Line const> lines)
  : _lines {lines.begin(), lines.end()}
{
}

auto ShapedText::GetLineCount() const -> SInt64
{
    return std::ssize(_lines);
}

auto ShapedText::GetLines() const -> std::vector<Line> const&
{
    return _lines;
}
}

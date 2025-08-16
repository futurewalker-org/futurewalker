// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.TextLayoutInfoType.hpp"
#include "Futurewalker.Graphics.Glyph.hpp"
#include "Futurewalker.Graphics.GlyphRunType.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Core.Optional.hpp"
#include "Futurewalker.Core.Memory.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Layout information of text runs.
///
class TextLayoutInfo
{
public:
    auto AddGlyphRun(Point<Dp> const& position, Shared<GlyphRun> const& glyphRun) -> void;

    auto GetGlyphRunCount() const -> SInt64;
    auto GetGlyphRun(SInt64 const glyphRunIndex) const -> Shared<GlyphRun>;
    auto GetGlyphRunPosition(SInt64 const glyphRunIndex) const -> Optional<Point<Dp>>;
    auto GetGlyphRunCharacterIndex(SInt64 const glyphRunIndex) const -> Optional<CodePoint>;
    auto GetGlyphRunIndex(CodePoint const characterIndex) const -> Optional<SInt64>;

    auto GetGlyphCount() const -> GlyphIndex;
    auto GetGlyphPosition(GlyphIndex const glyphIndex) const -> Optional<Point<Dp>>;
    auto GetCharacterIndex(GlyphIndex const glyphIndex) const -> Optional<CodePoint>;
    auto GetGlyphIndex(CodePoint const characterIndex) const -> GlyphIndex;

    auto GetBoundsRect() const -> Rect<Dp>;

private:
    auto FindRunByGlyphIndex(GlyphIndex const glyphIndex) const -> std::pair<Shared<GlyphRun>, GlyphIndex>;
    auto FindRunByCharacterIndex(CodePoint const characterIndex) const -> std::pair<Shared<GlyphRun>, CodePoint>;

private:
    CodePoint _textSize = 0;
    GlyphIndex _glyphCount = 0;
    SInt64 _glyphRunCount = 0;
    Rect<Dp> _glyphRunBounds;
    std::vector<Shared<GlyphRun>> _glyphRuns;
    std::vector<Point<Dp>> _glyphRunPositions;
};
}
}

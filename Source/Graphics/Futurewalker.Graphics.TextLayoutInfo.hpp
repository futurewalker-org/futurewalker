// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.TextLayoutInfoType.hpp"
#include "Futurewalker.Graphics.Glyph.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Core.Optional.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Layout information of text run.
///
class TextLayoutInfo
{
public:
    auto GetSize() const -> Size<Dp>;
    auto SetSize(Size<Dp> const& size) -> void;

    auto GetGlyphCount() const -> GlyphIndex;
    auto GetGlyphPosition(GlyphIndex const glyphIndex) const -> Optional<Point<Dp>>;
    auto GetCodePoint(GlyphIndex const glyphIndex) const -> Optional<CodeUnit>;
    auto GetGlyphIndex(CodeUnit const textPosition) const -> GlyphIndex;

private:
    Size<Dp> _size;
};
}
}

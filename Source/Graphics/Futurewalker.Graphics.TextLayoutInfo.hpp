// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Graphics.TextLayoutInfoType.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Geometry.hpp"

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

    struct GlyphInfo
    {
        CodePoint codePoint;
        Point<Dp> position;
    };
    auto SetGlyphInfo(std::vector<GlyphInfo> glyphInfo) -> void;

    auto GetGlyphCount() const -> GlyphIndex;
    auto GetGlyphPosition(GlyphIndex index) const -> Point<Dp>;
    auto GetGlyphIndex(CodePoint codePoint) const -> GlyphIndex;
    auto GetCodePoint(GlyphIndex glyphIndex) const -> CodePoint;

private:
    Size<Dp> _size;
    std::vector<GlyphInfo> _glyphInfo;
};
}
}

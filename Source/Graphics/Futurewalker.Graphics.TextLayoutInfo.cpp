// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.TextLayoutInfo.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
auto TextLayoutInfo::GetSize() const -> Size<Dp>
{
    return _size;
}

auto TextLayoutInfo::SetSize(Size<Dp> const& size) -> void
{
    _size = size;
}

auto TextLayoutInfo::GetGlyphCount() const -> GlyphIndex
{
    return 0;
}

auto TextLayoutInfo::GetGlyphPosition(GlyphIndex const glyphIndex) const -> Optional<Point<Dp>>
{
    if (0 <= glyphIndex && glyphIndex < GetGlyphCount())
    {
        // TODO.
    }
    return {};
}

auto TextLayoutInfo::GetCodePoint(GlyphIndex const glyphIndex) const -> Optional<CodeUnit>
{
    if (0 <= glyphIndex && glyphIndex < GetGlyphCount())
    {
        // TODO.
    }
    return {};
}

auto TextLayoutInfo::GetGlyphIndex(CodeUnit const textPosition) const -> GlyphIndex
{
    // TODO.
    (void)textPosition;
    return 0;
}
}

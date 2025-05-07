// SPDX-License-Identifier: MIT

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

auto TextLayoutInfo::SetGlyphInfo(std::vector<GlyphInfo> glyphInfo) -> void
{
    _glyphInfo = std::move(glyphInfo);

    std::sort(_glyphInfo.begin(), _glyphInfo.end(), [](const auto& l, const auto& r) { return l.codePoint < r.codePoint; });
}

auto TextLayoutInfo::GetGlyphCount() const -> GlyphIndex
{
    return GlyphIndex(_glyphInfo.size());
}

auto TextLayoutInfo::GetGlyphPosition(GlyphIndex glyphIndex) const -> Point<Dp>
{
    if (0 <= glyphIndex && glyphIndex < GetGlyphCount())
    {
        return _glyphInfo[static_cast<size_t>(glyphIndex)].position;
    }
    return {};
}

auto TextLayoutInfo::GetGlyphIndex(CodePoint codePoint) const -> GlyphIndex
{
    auto i = GlyphIndex(-1);
    for (auto const& info : _glyphInfo)
    {
        if (info.codePoint < codePoint)
        {
            break;
        }
        ++i;
    }
    auto const minIndex = GlyphIndex(0);
    auto const maxIndex = GlyphIndex::Max(0, std::ssize(_glyphInfo) - 1);
    return GlyphIndex::Clamp(i, minIndex, maxIndex);
}

auto TextLayoutInfo::GetCodePoint(GlyphIndex glyphIndex) const -> CodePoint
{
    if (0 <= glyphIndex && glyphIndex < GetGlyphCount())
    {
        return _glyphInfo[static_cast<size_t>(glyphIndex)].codePoint;
    }
    return 0;
}
}

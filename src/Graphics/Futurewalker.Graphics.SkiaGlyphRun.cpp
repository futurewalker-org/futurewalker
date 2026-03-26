// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.SkiaGlyphRun.hpp"

#include <include/core/SkFontMetrics.h>

namespace FW_GRAPHICS_DETAIL_NS
{
SkiaGlyphRun::~SkiaGlyphRun()
{
    ReleaseBuffer();
}

auto SkiaGlyphRun::GetText() const -> Text
{
    return _text;
}

auto SkiaGlyphRun::GetGlyphCount() const -> GlyphIndex
{
    return _glyphCount;
}

auto SkiaGlyphRun::GetGlyph(GlyphIndex const glyphIndex) const -> Optional<GlyphId>
{
    if (0 <= glyphIndex && glyphIndex < _glyphCount)
    {
        return GlyphId(_buffer.glyphs[static_cast<size_t>(glyphIndex)]);
    }
    return {};
}

auto SkiaGlyphRun::GetGlyphPosition(GlyphIndex const glyphIndex) const -> Optional<Point<Dp>>
{
    if (0 <= glyphIndex && glyphIndex < _glyphCount)
    {
        auto const position = _buffer.positions[static_cast<size_t>(glyphIndex)];
        return Point<Dp>(position.fX, position.fY);
    }
    return {};
}

auto SkiaGlyphRun::GetCharacterIndex(GlyphIndex const glyphIndex) const -> Optional<CodePoint>
{
    if (0 <= glyphIndex && glyphIndex < _glyphCount)
    {
        auto const clusterIndex = CodeUnit(_buffer.clusters[static_cast<size_t>(glyphIndex)]);
        return _text.GetCodePointIndexByU8Index(clusterIndex);
    }
    return {};
}

auto SkiaGlyphRun::GetGlyphIndex(CodePoint const textPosition) const -> Optional<GlyphIndex>
{
    if (0 <= textPosition)
    {
        auto const u8TextPosition = _text.GetU8IndexByCodePointIndex(textPosition);
        for (auto i = GlyphIndex(0); i < _glyphCount; ++i)
        {
            auto const clusterIndex = CodeUnit(_buffer.clusters[static_cast<size_t>(i)]);
            if (u8TextPosition < clusterIndex)
            {
                return GlyphIndex::Max(0, GlyphIndex(i - 1));
            }
        }
    }
    return {};
}

auto SkiaGlyphRun::GetMetrics() const -> FontMetrics
{
    auto skFontMetrics = SkFontMetrics();
    _font.getMetrics(&skFontMetrics);

    return FontMetrics {
        .ascent = -skFontMetrics.fAscent,
        .descent = skFontMetrics.fDescent,
        .leading = skFontMetrics.fLeading,
        .xHeight = skFontMetrics.fXHeight,
        .capHeight = skFontMetrics.fCapHeight,
    };
}

auto SkiaGlyphRun::GetAdvance() const -> Dp
{
    return Dp(_advance);
}

auto SkiaGlyphRun::AllocBuffer(SInt64 glyphCount) -> Buffer
{
    ReleaseBuffer();
    _glyphCount = GlyphIndex(glyphCount);
    _buffer.glyphs = new SkGlyphID[static_cast<size_t>(glyphCount)];
    _buffer.positions = new SkPoint[static_cast<size_t>(glyphCount)];
    _buffer.clusters = new uint32_t[static_cast<size_t>(glyphCount)];
    return _buffer;
}

auto SkiaGlyphRun::SetFont(SkFont const& font) -> void
{
    _font = font;
}

auto SkiaGlyphRun::SetAdvance(SkScalar const& advance) -> void
{
    _advance = advance;
}

auto SkiaGlyphRun::SetText(Text const& text) -> void
{
    _text = text;
}

auto SkiaGlyphRun::Draw(SkCanvas* canvas, SkPaint const& paint) const -> void
{
    if (canvas)
    {
        auto const count = static_cast<int>(_glyphCount);
        auto const utf8TextData = reinterpret_cast<char const*>(static_cast<char8_t const*>(_text.GetString().GetView().GetData()));
        auto const utf8TextCount = static_cast<int>(_text.GetU8CodeUnitCount());
        canvas->drawGlyphs({_buffer.glyphs, count}, {_buffer.positions, count}, {_buffer.clusters, count}, std::string_view(utf8TextData, utf8TextCount), SkPoint(), _font, paint);
    }
}
auto SkiaGlyphRun::ReleaseBuffer() -> void
{
    if (_buffer.glyphs)
    {
        delete[] _buffer.glyphs;
        _buffer.glyphs = nullptr;
    }
    if (_buffer.positions)
    {
        delete[] _buffer.positions;
        _buffer.positions = nullptr;
    }
    if (_buffer.clusters)
    {
        delete[] _buffer.clusters;
        _buffer.clusters = nullptr;
    }
    _glyphCount = 0;
}
}

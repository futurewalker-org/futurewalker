// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.SkiaGlyphRun.hpp"
#include "Futurewalker.Graphics.GlyphRun.hpp"

#include "Futurewalker.Core.String.hpp"

#include <include/core/SkCanvas.h>
#include <include/core/SkFont.h>

namespace FW_GRAPHICS_DETAIL_NS
{
class SkiaGlyphRun final : public GlyphRun
{
public:
    ~SkiaGlyphRun();

    auto GetText() const -> Text override;
    auto GetMetrics() const -> FontMetrics override;
    auto GetAdvance() const -> Dp override;
    auto GetGlyphCount() const -> GlyphIndex override;
    auto GetGlyph(GlyphIndex const glyphIndex) const -> Optional<GlyphId> override;
    auto GetGlyphPosition(GlyphIndex const glyphIndex) const -> Optional<Point<Dp>> override;
    auto GetCharacterIndex(GlyphIndex const glyphIndex) const -> Optional<CodePoint> override;
    auto GetGlyphIndex(CodePoint const textPosition) const -> Optional<GlyphIndex> override;

public:
    struct Buffer
    {
        SkGlyphID* glyphs = nullptr;
        SkPoint* positions = nullptr;
        uint32_t* clusters = nullptr;
    };
    auto AllocBuffer(SInt64 glyphCount) -> Buffer;
    auto SetFont(SkFont const& font) -> void;
    auto SetAdvance(SkScalar const& advance) -> void;
    auto SetText(Text const& text) -> void;
    auto Draw(SkCanvas* canvas, SkPaint const& paint) const -> void;

private:
    auto ReleaseBuffer() -> void;

private:
    Buffer _buffer;
    GlyphIndex _glyphCount = 0;
    SkFont _font;
    SkScalar _advance = 0;
    Text _text;
};
}

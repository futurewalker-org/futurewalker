// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.GlyphRunType.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"

#include "Futurewalker.Graphics.FontMetrics.hpp"
#include "Futurewalker.Graphics.Glyph.hpp"
#include "Futurewalker.Graphics.Scene.hpp"

#include "Futurewalker.Geometry.Point.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Color.RGBAColor.hpp"

#include "Futurewalker.Core.Optional.hpp"
#include "Futurewalker.Core.String.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Sequence of positioned glyphs with single font and style.
///
class GlyphRun : NonCopyable
{
public:
    virtual ~GlyphRun() = 0;

    ///
    /// @brief Get text of the run.
    ///
    virtual auto GetText() const -> String = 0;

    ///
    /// @brief Get metrics of the run.
    ///
    virtual auto GetMetrics() const -> FontMetrics = 0;

    ///
    /// @brief Get advance of the run.
    ///
    virtual auto GetAdvance() const -> Dp = 0;

    ///
    /// @brief Get total number of glyphs in the run.
    ///
    virtual auto GetGlyphCount() const -> GlyphIndex = 0;

    ///
    /// @brief Get the glyph ID by its index.
    ///
    /// @param[in] glyphIndex Index of the glyph in the run.
    ///
    /// @return ID of the glyph, or an empty optional if the index is out of bounds.
    ///
    virtual auto GetGlyph(GlyphIndex const glyphIndex) const -> Optional<GlyphId> = 0;

    ///
    /// @brief Get the position of a glyph by its index.
    ///
    /// @param[in] glyphIndex Index of the glyph in the run.
    ///
    /// @return Position of the glyph, or an empty optional if the index is out of bounds.
    ///
    virtual auto GetGlyphPosition(GlyphIndex const glyphIndex) const -> Optional<Point<Dp>> = 0;

    ///
    /// @brief Get the text position of the glyph by its index.
    ///
    /// @param[in] glyphIndex Index of the glyph in the run.
    ///
    /// @return Text position of the glyph, or an empty optional if the index is out of bounds.
    ///
    virtual auto GetClusterIndex(GlyphIndex const glyphIndex) const -> Optional<CodeUnit> = 0;

    ///
    /// @brief Get the index of a glyph by its position in the text.
    ///
    /// @param[in] textPosition Position in the whole text.
    ///
    /// @return Index of the glyph in the run.
    ///
    /// @note If the run does not contain a glyph for the given text position, it returns closest index of a glyph.
    ///
    virtual auto GetGlyphIndex(CodeUnit const textPosition) const -> GlyphIndex = 0;
};
}
}

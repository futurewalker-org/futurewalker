// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.Prelude.hpp"
#include "Futurewalker.Graphics.ShapedTextType.hpp"
#include "Futurewalker.Graphics.FontSize.hpp"
#include "Futurewalker.Graphics.Typeface.hpp"
#include "Futurewalker.Graphics.ShapedText.hpp"

#include "Futurewalker.Text.Text.hpp" 

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"

#include <array>

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Text shaper.
///
class TextShaper : NonCopyable
{
public:
    static auto Make() -> Shared<TextShaper>;

public:
    virtual ~TextShaper() = 0;

    ///
    /// @brief Direction of the text for shaping.
    ///
    enum class Direction
    {
        DefaultLtr, ///< Left to Right
        DefaultRtl, ///< Right to Left
    };

    using FourByteTag = std::array<char, 4>;

    ///
    /// @brief Shape text with single style.
    ///
    /// @param text Text to shape.
    /// @param typeface Typeface to use for shaping.
    /// @param size Font size.
    /// @param maxWidth Maximum width for the text layout.
    /// @param bcp47ScriptTag BCP47 script tag of the text.
    /// @param direction Direction of the text.
    ///
    virtual auto ShapeText(Text const& text, Shared<Typeface> const& typeface, FontSize const size, Dp const maxWidth, FourByteTag const& bcp47ScriptTag, Direction const& direction) -> ShapedText = 0;

    ///
    /// @brief Shape a glyph.
    ///
    /// @param codePoint Unicode code point of the glyph.
    /// @param typeface Typeface to use for shaping.
    /// @param size Font size.
    /// @param bcp47ScriptTag BCP47 script tag of the text.
    /// @param direction Direction of the text.
    ///
    virtual auto ShapeGlyph(char32_t const codePoint, Shared<Typeface> const& typeface, FontSize const size, FourByteTag const& bcp47ScriptTag, Direction const& direction) -> ShapedText = 0;
};
}
}

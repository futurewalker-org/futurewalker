// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Graphics.Prelude.hpp"
#include "Futurewalker.Graphics.ShapedTextType.hpp"
#include "Futurewalker.Graphics.FontSize.hpp"
#include "Futurewalker.Graphics.Typeface.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.String.hpp"

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
    virtual auto Shape(String const& text, Shared<Typeface> const& typeface, FontSize const size, Dp const maxWidth) -> Shared<ShapedText> = 0;
};
}
}

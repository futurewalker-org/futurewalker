// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.Prelude.hpp"

#include "Futurewalker.Core.Identifier.hpp"
#include "Futurewalker.Core.Primitive.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Index of glyphs.
///
using GlyphIndex = Integer<int64_t, class UnitTagGlyphIndex>;

///
/// @brief Identifier of glyphs.
///
using GlyphId = Identifier<UInt32, class UnitTagGlyphId>;
}
}

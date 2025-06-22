// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Count or index of Unicode code points.
///
using CodePoint = Integer<int64_t, class UnitTagCodePoint>;

///
/// @brief Count or index of Unicode code units.
///
using CodeUnit = Integer<int64_t, class UnitTagCodeUnit>;

///
/// @brief Index of glyphs.
///
using GlyphIndex = Integer<int64_t, class UnitTagGlyphIndex>;
}
}

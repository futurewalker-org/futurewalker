// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.FontMetricsType.hpp"

#include "Futurewalker.Unit.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Layout metrics of a font.
///
struct FontMetrics
{
    ///
    /// @brief Distance between baseline to highest ascender.
    ///
    /// @note This value is typically positive.
    ///
    Dp ascent = 0;

    ///
    /// @brief Distance between baseline to lowest descender.
    ///
    /// @note This value is typically positive.
    ///
    Dp descent = 0;

    ///
    /// @brief Distance between lines of text.
    ///
    /// @note This value is typically positive.
    ///
    Dp leading = 0;

    ///
    /// @brief Height of lowercase 'x' character.
    ///
    /// @note This value is typically positive.
    ///
    Dp xHeight = 0;

    ///
    /// @brief Height of capital letters.
    ///
    /// @note This value is typically positive.
    ///
    Dp capHeight = 0;
};
}
}

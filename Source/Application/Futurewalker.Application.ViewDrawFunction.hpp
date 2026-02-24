// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.CornerRadius.hpp"

#include "Futurewalker.Graphics.SceneType.hpp"
#include "Futurewalker.Graphics.MaskFilterType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Color.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Utility functions for drawing views.
///
struct ViewDrawFunction : NonConstructible
{
    static auto DrawRoundRect(Graphics::Scene& scene, Rect<Dp> const& rect, CornerRadius const& radius, RGBAColor const& color, LayoutDirection const& direction) -> void;
    static auto DrawRoundRectBorder(Graphics::Scene& scene, Rect<Dp> const& rect, CornerRadius const& radius, RGBAColor const& color, Dp const& width, LayoutDirection const& direction) -> void;
    static auto DrawRoundRectShadow(Graphics::Scene& scene, Rect<Dp> const& rect, CornerRadius const& radius, RGBAColor const& color, Dp const& blurRadius, Graphics::BlurStyle const blurStyle, LayoutDirection const& direction) -> void;
};
}
}

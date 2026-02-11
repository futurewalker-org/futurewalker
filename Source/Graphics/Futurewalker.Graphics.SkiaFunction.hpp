// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Graphics.Prelude.hpp"
#include "Futurewalker.Graphics.MaskFilterType.hpp"
#include "Futurewalker.Graphics.FontStyleType.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Color.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

#include <include/core/SkRRect.h>
#include <include/core/SkColor.h>
#include <include/core/SkBlurTypes.h>
#include <include/core/SkFontStyle.h>

#include <array>

namespace FW_GRAPHICS_DETAIL_NS
{
struct SkiaFunction : NonConstructible
{
    static auto PointToSkPoint(Point<Dp> const& point) -> SkPoint;
    static auto RectToSkRect(Rect<Dp> const& rect) -> SkRect;
    static auto RadiusToSkVector(Radius<Dp> const& radius) -> SkVector;
    static auto RoundRectToSkRRect(RoundRect<Dp> const& roundRect) -> SkRRect;
    static auto RGBAColorToSkColor4f(RGBAColor const& color) -> SkColor4f;
    static auto BlurStyleToSkBlurStyle(BlurStyle const style) -> SkBlurStyle;
    static auto FontSlantToSkFontSlant(FontSlant const slant) -> SkFontStyle::Slant;
    static auto FontStyleToSkFontStyle(FontStyle const& fontStyle) -> SkFontStyle;
};
}

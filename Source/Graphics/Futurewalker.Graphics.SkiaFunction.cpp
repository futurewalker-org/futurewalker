// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Graphics.SkiaFunction.hpp"

namespace FW_GRAPHICS_DETAIL_NS
{
auto SkiaFunction::PointToSkPoint(Point<Dp> const& point) -> SkPoint
{
    return {
        .fX = static_cast<SkScalar>(point.GetX()),
        .fY = static_cast<SkScalar>(point.GetY()),
    };
}

auto SkiaFunction::RectToSkRect(Rect<Dp> const& rect) -> SkRect
{
    return {
        .fLeft = static_cast<SkScalar>(rect.GetLeft()),
        .fTop = static_cast<SkScalar>(rect.GetTop()),
        .fRight = static_cast<SkScalar>(rect.GetRight()),
        .fBottom = static_cast<SkScalar>(rect.GetBottom()),
    };
}

auto SkiaFunction::RadiusToSkVector(Radius<Dp> const& radius) -> SkVector
{
    return {
        .fX = static_cast<SkScalar>(radius.GetX()),
        .fY = static_cast<SkScalar>(radius.GetY()),
    };
}

auto SkiaFunction::RoundRectToSkRRect(RoundRect<Dp> const& roundRect) -> SkRRect
{
    auto const rect = RectToSkRect(roundRect.GetRect());
    auto const radii = std::array<SkVector, 4> {
        RadiusToSkVector(roundRect.GetTopLeftRadius()),
        RadiusToSkVector(roundRect.GetTopRightRadius()),
        RadiusToSkVector(roundRect.GetBottomRightRadius()),
        RadiusToSkVector(roundRect.GetBottomLeftRadius()),
    };
    auto rr = SkRRect();
    rr.setRectRadii(rect, radii.data());
    return rr;
}

auto SkiaFunction::RGBAColorToSkColor4f(RGBAColor const& color) -> SkColor4f
{
    return {
        .fR = static_cast<SkScalar>(color.GetRedF64()),
        .fG = static_cast<SkScalar>(color.GetGreenF64()),
        .fB = static_cast<SkScalar>(color.GetBlueF64()),
        .fA = static_cast<SkScalar>(color.GetAlphaF64()),
    };
}
}

// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ViewDrawFunction.hpp"

#include "Futurewalker.Graphics.Scene.hpp"
#include "Futurewalker.Graphics.MaskFilter.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Fill inside of round rectangle.
///
auto ViewDrawFunction::DrawRoundRect(Graphics::Scene& scene, Rect<Dp> const& rect, CornerRadius const& radius, RGBAColor const& color, LayoutDirection const& direction) -> void
{
    if (!rect.IsEmpty() && color.GetAlpha() > 0)
    {
        auto const roundRect = radius.GetRoundRect(rect, direction);
        scene.AddRoundRect({
            .roundRect = roundRect,
            .color = color,
            .drawStyle = Graphics::DrawStyle::Fill,
            .antiAlias = true,
        });
    }
}

///
/// @brief Draw border of round rectangle.
///
auto ViewDrawFunction::DrawRoundRectBorder(Graphics::Scene& scene, Rect<Dp> const& rect, CornerRadius const& radius, RGBAColor const& color, Dp const& width, LayoutDirection const& direction) -> void
{
    if (!rect.IsEmpty() && color.GetAlpha() > 0 && width > 0)
    {
        auto const halfWidth = width / 2;
        auto const borderRect = Rect<Dp>::Inflate(rect, -halfWidth, -halfWidth);
        auto const borderRadius = CornerRadius::Offset(radius, -halfWidth);
        auto const borderRoundRect = borderRadius.GetRoundRect(borderRect, direction);
        scene.AddRoundRect({
            .roundRect = borderRoundRect,
            .color = color,
            .drawStyle = Graphics::DrawStyle::Stroke,
            .antiAlias = true,
            .strokeWidth = width,
        });
    }
}

///
/// @brief Draw shadow of round rectangle.
///
auto ViewDrawFunction::DrawRoundRectShadow(
  Graphics::Scene& scene,
  Rect<Dp> const& rect,
  CornerRadius const& radius,
  RGBAColor const& color,
  Dp const& blurRadius,
  Graphics::BlurStyle const blurStyle,
  LayoutDirection const& direction) -> void
{
    if (!rect.IsEmpty() && color.GetAlpha() > 0 && blurRadius > 0)
    {
        auto path = Graphics::Path();
        path.AddRoundRect(radius.GetRoundRect(rect, direction));

        scene.AddPath({
            .path = path,
            .color = color,
            .drawStyle = Graphics::DrawStyle::Fill,
            .antiAlias = true,
            .maskFilter = Graphics::MaskFilter::MakeBlur(blurStyle, blurRadius / 2),
        });
    }
}
}

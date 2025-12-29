// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.Prelude.hpp"

#include "Futurewalker.Application.CornerRadius.hpp"
#include "Futurewalker.Application.EdgeInsets.hpp"

#include "Futurewalker.Attribute.StaticAttribute.hpp"

#include "Futurewalker.Graphics.FontSize.hpp"
#include "Futurewalker.Graphics.FontWeight.hpp"

#include "Futurewalker.Color.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Semantic style variables.
///
struct Style : NonConstructible
{
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorPrimary);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorPrimaryContainer);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorOnPrimary);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorOnPrimaryContainer);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorSecondary);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorSecondaryContainer);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorOnSecondary);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorOnSecondaryContainer);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorTertiary);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorTertiaryContainer);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorOnTertiary);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorOnTertiaryContainer);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorSurface);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorSurfaceVariant);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorSurfaceDim);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorSurfaceBright);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorSurfaceContainerLowest);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorSurfaceContainerLow);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorSurfaceContainer);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorSurfaceContainerHigh);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorSurfaceContainerHighest);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorOnSurface);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorOnSurfaceVariant);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorOutline);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorOutlineVariant);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorError);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorErrorContainer);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorOnError);
    FW_STATIC_ATTRIBUTE(RGBAColor, ColorOnErrorContainer);

    FW_STATIC_ATTRIBUTE(Channel, AlphaHighlightHover);
    FW_STATIC_ATTRIBUTE(Channel, AlphaHighlightPress);

    FW_STATIC_ATTRIBUTE(Channel, AlphaNone);
    FW_STATIC_ATTRIBUTE(Channel, AlphaOpaque);
    FW_STATIC_ATTRIBUTE(Channel, AlphaEnabled);
    FW_STATIC_ATTRIBUTE(Channel, AlphaDisabled);

    FW_STATIC_ATTRIBUTE(Graphics::FontSize, FontSizeBodySmall);
    FW_STATIC_ATTRIBUTE(Graphics::FontSize, FontSizeBodyMedium);
    FW_STATIC_ATTRIBUTE(Graphics::FontSize, FontSizeBodyLarge);
    FW_STATIC_ATTRIBUTE(Graphics::FontSize, FontSizeLabelSmall);
    FW_STATIC_ATTRIBUTE(Graphics::FontSize, FontSizeLabelMedium);
    FW_STATIC_ATTRIBUTE(Graphics::FontSize, FontSizeLabelLarge);
    FW_STATIC_ATTRIBUTE(Graphics::FontSize, FontSizeTitleSmall);
    FW_STATIC_ATTRIBUTE(Graphics::FontSize, FontSizeTitleMedium);
    FW_STATIC_ATTRIBUTE(Graphics::FontSize, FontSizeTitleLarge);
    FW_STATIC_ATTRIBUTE(Graphics::FontSize, FontSizeHeadlineSmall);
    FW_STATIC_ATTRIBUTE(Graphics::FontSize, FontSizeHeadlineMedium);
    FW_STATIC_ATTRIBUTE(Graphics::FontSize, FontSizeHeadlineLarge);
    FW_STATIC_ATTRIBUTE(Graphics::FontSize, FontSizeDisplaySmall);
    FW_STATIC_ATTRIBUTE(Graphics::FontSize, FontSizeDisplayMedium);
    FW_STATIC_ATTRIBUTE(Graphics::FontSize, FontSizeDisplayLarge);

    FW_STATIC_ATTRIBUTE(Graphics::FontWeight, FontWeightBodySmall);
    FW_STATIC_ATTRIBUTE(Graphics::FontWeight, FontWeightBodyMedium);
    FW_STATIC_ATTRIBUTE(Graphics::FontWeight, FontWeightBodyLarge);
    FW_STATIC_ATTRIBUTE(Graphics::FontWeight, FontWeightLabelSmall);
    FW_STATIC_ATTRIBUTE(Graphics::FontWeight, FontWeightLabelMedium);
    FW_STATIC_ATTRIBUTE(Graphics::FontWeight, FontWeightLabelLarge);
    FW_STATIC_ATTRIBUTE(Graphics::FontWeight, FontWeightTitleSmall);
    FW_STATIC_ATTRIBUTE(Graphics::FontWeight, FontWeightTitleMedium);
    FW_STATIC_ATTRIBUTE(Graphics::FontWeight, FontWeightTitleLarge);
    FW_STATIC_ATTRIBUTE(Graphics::FontWeight, FontWeightHeadlineSmall);
    FW_STATIC_ATTRIBUTE(Graphics::FontWeight, FontWeightHeadlineMedium);
    FW_STATIC_ATTRIBUTE(Graphics::FontWeight, FontWeightHeadlineLarge);
    FW_STATIC_ATTRIBUTE(Graphics::FontWeight, FontWeightDisplaySmall);
    FW_STATIC_ATTRIBUTE(Graphics::FontWeight, FontWeightDisplayMedium);
    FW_STATIC_ATTRIBUTE(Graphics::FontWeight, FontWeightDisplayLarge);

    FW_STATIC_ATTRIBUTE(CornerRadius, CornerRadiusNone);
    FW_STATIC_ATTRIBUTE(CornerRadius, CornerRadiusExtraSmall);
    FW_STATIC_ATTRIBUTE(CornerRadius, CornerRadiusSmall);
    FW_STATIC_ATTRIBUTE(CornerRadius, CornerRadiusMedium);
    FW_STATIC_ATTRIBUTE(CornerRadius, CornerRadiusLarge);
    FW_STATIC_ATTRIBUTE(CornerRadius, CornerRadiusLargeIncreased);
    FW_STATIC_ATTRIBUTE(CornerRadius, CornerRadiusExtraLarge);
    FW_STATIC_ATTRIBUTE(CornerRadius, CornerRadiusExtraLargeIncreased);
    FW_STATIC_ATTRIBUTE(CornerRadius, CornerRadiusLargest);

    FW_STATIC_ATTRIBUTE(Dp, SizeNone);
    FW_STATIC_ATTRIBUTE(Dp, Size10);
    FW_STATIC_ATTRIBUTE(Dp, Size20);
    FW_STATIC_ATTRIBUTE(Dp, Size40);
    FW_STATIC_ATTRIBUTE(Dp, Size60);
    FW_STATIC_ATTRIBUTE(Dp, Size80);
    FW_STATIC_ATTRIBUTE(Dp, Size100);
    FW_STATIC_ATTRIBUTE(Dp, Size120);
    FW_STATIC_ATTRIBUTE(Dp, Size160);
    FW_STATIC_ATTRIBUTE(Dp, Size200);
    FW_STATIC_ATTRIBUTE(Dp, Size240);
    FW_STATIC_ATTRIBUTE(Dp, Size280);
    FW_STATIC_ATTRIBUTE(Dp, Size320);
    FW_STATIC_ATTRIBUTE(Dp, Size360);
    FW_STATIC_ATTRIBUTE(Dp, Size400);
    FW_STATIC_ATTRIBUTE(Dp, Size480);
    FW_STATIC_ATTRIBUTE(Dp, Size520);

    FW_STATIC_ATTRIBUTE(Dp, IconSizeNone);
    FW_STATIC_ATTRIBUTE(Dp, IconSizeExtraSmall);
    FW_STATIC_ATTRIBUTE(Dp, IconSizeSmall);
    FW_STATIC_ATTRIBUTE(Dp, IconSizeMedium);
    FW_STATIC_ATTRIBUTE(Dp, IconSizeLarge);
    FW_STATIC_ATTRIBUTE(Dp, IconSizeExtraLarge);

    FW_STATIC_ATTRIBUTE(Dp, SpaceNone);
    FW_STATIC_ATTRIBUTE(Dp, SpaceExtraSmall);
    FW_STATIC_ATTRIBUTE(Dp, SpaceSmall);
    FW_STATIC_ATTRIBUTE(Dp, SpaceMedium);
    FW_STATIC_ATTRIBUTE(Dp, SpaceLarge);
    FW_STATIC_ATTRIBUTE(Dp, SpaceExtraLarge);

    FW_STATIC_ATTRIBUTE(EdgeInsets, PaddingNone);
    FW_STATIC_ATTRIBUTE(EdgeInsets, PaddingExtraSmall);
    FW_STATIC_ATTRIBUTE(EdgeInsets, PaddingSmall);
    FW_STATIC_ATTRIBUTE(EdgeInsets, PaddingMedium);
    FW_STATIC_ATTRIBUTE(EdgeInsets, PaddingLarge);
    FW_STATIC_ATTRIBUTE(EdgeInsets, PaddingExtraLarge);
};
}
}

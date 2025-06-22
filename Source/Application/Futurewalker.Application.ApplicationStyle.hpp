// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Prelude.hpp"

#include "Futurewalker.Attribute.StaticAttribute.hpp"

#include "Futurewalker.Graphics.FontSize.hpp"
#include "Futurewalker.Graphics.FontWeight.hpp"

#include "Futurewalker.Color.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Semantic style variables.
///
class ApplicationStyle : NonConstructible
{
public:
    static StaticAttribute<RGBAColor> const ColorPrimary;
    static StaticAttribute<RGBAColor> const ColorPrimaryContainer;
    static StaticAttribute<RGBAColor> const ColorOnPrimary;
    static StaticAttribute<RGBAColor> const ColorOnPrimaryContainer;
    static StaticAttribute<RGBAColor> const ColorSecondary;
    static StaticAttribute<RGBAColor> const ColorSecondaryContainer;
    static StaticAttribute<RGBAColor> const ColorOnSecondary;
    static StaticAttribute<RGBAColor> const ColorOnSecondaryContainer;
    static StaticAttribute<RGBAColor> const ColorTertiary;
    static StaticAttribute<RGBAColor> const ColorTertiaryContainer;
    static StaticAttribute<RGBAColor> const ColorOnTertiary;
    static StaticAttribute<RGBAColor> const ColorOnTertiaryContainer;
    static StaticAttribute<RGBAColor> const ColorSurface;
    static StaticAttribute<RGBAColor> const ColorSurfaceDim;
    static StaticAttribute<RGBAColor> const ColorSurfaceBright;
    static StaticAttribute<RGBAColor> const ColorSurfaceContainerLowest;
    static StaticAttribute<RGBAColor> const ColorSurfaceContainerLow;
    static StaticAttribute<RGBAColor> const ColorSurfaceContainer;
    static StaticAttribute<RGBAColor> const ColorSurfaceContainerHigh;
    static StaticAttribute<RGBAColor> const ColorSurfaceContainerHighest;
    static StaticAttribute<RGBAColor> const ColorOnSurface;
    static StaticAttribute<RGBAColor> const ColorOnSurfaceVariant;
    static StaticAttribute<RGBAColor> const ColorOutline;
    static StaticAttribute<RGBAColor> const ColorOutlineVariant;
    static StaticAttribute<RGBAColor> const ColorError;
    static StaticAttribute<RGBAColor> const ColorErrorContainer;
    static StaticAttribute<RGBAColor> const ColorOnError;
    static StaticAttribute<RGBAColor> const ColorOnErrorContainer;

    static StaticAttribute<Graphics::FontSize> const FontSizeBodySmall;
    static StaticAttribute<Graphics::FontSize> const FontSizeBodyMedium;
    static StaticAttribute<Graphics::FontSize> const FontSizeBodyLarge;
    static StaticAttribute<Graphics::FontSize> const FontSizeLabelSmall;
    static StaticAttribute<Graphics::FontSize> const FontSizeLabelMedium;
    static StaticAttribute<Graphics::FontSize> const FontSizeLabelLarge;
    static StaticAttribute<Graphics::FontSize> const FontSizeTitleSmall;
    static StaticAttribute<Graphics::FontSize> const FontSizeTitleMedium;
    static StaticAttribute<Graphics::FontSize> const FontSizeTitleLarge;
    static StaticAttribute<Graphics::FontSize> const FontSizeHeadlineSmall;
    static StaticAttribute<Graphics::FontSize> const FontSizeHeadlineMedium;
    static StaticAttribute<Graphics::FontSize> const FontSizeHeadlineLarge;
    static StaticAttribute<Graphics::FontSize> const FontSizeDisplaySmall;
    static StaticAttribute<Graphics::FontSize> const FontSizeDisplayMedium;
    static StaticAttribute<Graphics::FontSize> const FontSizeDisplayLarge;

    static StaticAttribute<Graphics::FontWeight> const FontWeightBodySmall;
    static StaticAttribute<Graphics::FontWeight> const FontWeightBodyMedium;
    static StaticAttribute<Graphics::FontWeight> const FontWeightBodyLarge;
    static StaticAttribute<Graphics::FontWeight> const FontWeightLabelSmall;
    static StaticAttribute<Graphics::FontWeight> const FontWeightLabelMedium;
    static StaticAttribute<Graphics::FontWeight> const FontWeightLabelLarge;
    static StaticAttribute<Graphics::FontWeight> const FontWeightTitleSmall;
    static StaticAttribute<Graphics::FontWeight> const FontWeightTitleMedium;
    static StaticAttribute<Graphics::FontWeight> const FontWeightTitleLarge;
    static StaticAttribute<Graphics::FontWeight> const FontWeightHeadlineSmall;
    static StaticAttribute<Graphics::FontWeight> const FontWeightHeadlineMedium;
    static StaticAttribute<Graphics::FontWeight> const FontWeightHeadlineLarge;
    static StaticAttribute<Graphics::FontWeight> const FontWeightDisplaySmall;
    static StaticAttribute<Graphics::FontWeight> const FontWeightDisplayMedium;
    static StaticAttribute<Graphics::FontWeight> const FontWeightDisplayLarge;
};
}
}

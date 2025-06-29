// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ApplicationStyle.hpp"
#include "Futurewalker.Application.ApplicationColor.hpp"

namespace FW_DETAIL_NS
{
StaticAttribute<RGBAColor> const ApplicationStyle::ColorPrimary = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Primary40>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorPrimaryContainer = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Primary90>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorOnPrimary = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Primary100>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorOnPrimaryContainer = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Primary10>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorSecondary = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Secondary40>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorSecondaryContainer = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Secondary90>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorOnSecondary = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Secondary100>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorOnSecondaryContainer = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Secondary10>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorTertiary = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Tertiary40>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorTertiaryContainer = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Tertiary90>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorOnTertiary = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Tertiary100>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorOnTertiaryContainer = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Tertiary10>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorSurface = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Neutral98>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorSurfaceDim = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Neutral87>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorSurfaceBright = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Neutral98>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorSurfaceContainerLowest = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Neutral100>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorSurfaceContainerLow = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Neutral96>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorSurfaceContainer = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Neutral94>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorSurfaceContainerHigh = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Neutral92>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorSurfaceContainerHighest = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Neutral90>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorOnSurface = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Neutral10>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorOnSurfaceVariant = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Neutral30>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorOutline = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::NeutralVariant50>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorOutlineVariant = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::NeutralVariant90>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorError = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Error40>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorErrorContainer = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Error90>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorOnError = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Error100>();
StaticAttribute<RGBAColor> const ApplicationStyle::ColorOnErrorContainer = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationColor::Error10>();

StaticAttribute<Graphics::FontSize> const ApplicationStyle::FontSizeBodySmall = StaticAttribute<Graphics::FontSize>::MakeWithDefaultValue(11);
StaticAttribute<Graphics::FontSize> const ApplicationStyle::FontSizeBodyMedium = StaticAttribute<Graphics::FontSize>::MakeWithDefaultValue(14);
StaticAttribute<Graphics::FontSize> const ApplicationStyle::FontSizeBodyLarge = StaticAttribute<Graphics::FontSize>::MakeWithDefaultValue(16);
StaticAttribute<Graphics::FontSize> const ApplicationStyle::FontSizeLabelSmall = StaticAttribute<Graphics::FontSize>::MakeWithDefaultValue(11);
StaticAttribute<Graphics::FontSize> const ApplicationStyle::FontSizeLabelMedium = StaticAttribute<Graphics::FontSize>::MakeWithDefaultValue(12);
StaticAttribute<Graphics::FontSize> const ApplicationStyle::FontSizeLabelLarge = StaticAttribute<Graphics::FontSize>::MakeWithDefaultValue(14);
StaticAttribute<Graphics::FontSize> const ApplicationStyle::FontSizeTitleSmall = StaticAttribute<Graphics::FontSize>::MakeWithDefaultValue(14);
StaticAttribute<Graphics::FontSize> const ApplicationStyle::FontSizeTitleMedium = StaticAttribute<Graphics::FontSize>::MakeWithDefaultValue(16);
StaticAttribute<Graphics::FontSize> const ApplicationStyle::FontSizeTitleLarge = StaticAttribute<Graphics::FontSize>::MakeWithDefaultValue(22);
StaticAttribute<Graphics::FontSize> const ApplicationStyle::FontSizeHeadlineSmall = StaticAttribute<Graphics::FontSize>::MakeWithDefaultValue(24);
StaticAttribute<Graphics::FontSize> const ApplicationStyle::FontSizeHeadlineMedium = StaticAttribute<Graphics::FontSize>::MakeWithDefaultValue(28);
StaticAttribute<Graphics::FontSize> const ApplicationStyle::FontSizeHeadlineLarge = StaticAttribute<Graphics::FontSize>::MakeWithDefaultValue(32);
StaticAttribute<Graphics::FontSize> const ApplicationStyle::FontSizeDisplaySmall = StaticAttribute<Graphics::FontSize>::MakeWithDefaultValue(36);
StaticAttribute<Graphics::FontSize> const ApplicationStyle::FontSizeDisplayMedium = StaticAttribute<Graphics::FontSize>::MakeWithDefaultValue(45);
StaticAttribute<Graphics::FontSize> const ApplicationStyle::FontSizeDisplayLarge = StaticAttribute<Graphics::FontSize>::MakeWithDefaultValue(57);

StaticAttribute<Graphics::FontWeight> const ApplicationStyle::FontWeightBodySmall = StaticAttribute<Graphics::FontWeight>::MakeWithDefaultValue(400);
StaticAttribute<Graphics::FontWeight> const ApplicationStyle::FontWeightBodyMedium = StaticAttribute<Graphics::FontWeight>::MakeWithDefaultValue(400);
StaticAttribute<Graphics::FontWeight> const ApplicationStyle::FontWeightBodyLarge = StaticAttribute<Graphics::FontWeight>::MakeWithDefaultValue(400);
StaticAttribute<Graphics::FontWeight> const ApplicationStyle::FontWeightLabelSmall = StaticAttribute<Graphics::FontWeight>::MakeWithDefaultValue(500);
StaticAttribute<Graphics::FontWeight> const ApplicationStyle::FontWeightLabelMedium = StaticAttribute<Graphics::FontWeight>::MakeWithDefaultValue(500);
StaticAttribute<Graphics::FontWeight> const ApplicationStyle::FontWeightLabelLarge = StaticAttribute<Graphics::FontWeight>::MakeWithDefaultValue(500);
StaticAttribute<Graphics::FontWeight> const ApplicationStyle::FontWeightTitleSmall = StaticAttribute<Graphics::FontWeight>::MakeWithDefaultValue(500);
StaticAttribute<Graphics::FontWeight> const ApplicationStyle::FontWeightTitleMedium = StaticAttribute<Graphics::FontWeight>::MakeWithDefaultValue(500);
StaticAttribute<Graphics::FontWeight> const ApplicationStyle::FontWeightTitleLarge = StaticAttribute<Graphics::FontWeight>::MakeWithDefaultValue(400);
StaticAttribute<Graphics::FontWeight> const ApplicationStyle::FontWeightHeadlineSmall = StaticAttribute<Graphics::FontWeight>::MakeWithDefaultValue(400);
StaticAttribute<Graphics::FontWeight> const ApplicationStyle::FontWeightHeadlineMedium = StaticAttribute<Graphics::FontWeight>::MakeWithDefaultValue(400);
StaticAttribute<Graphics::FontWeight> const ApplicationStyle::FontWeightHeadlineLarge = StaticAttribute<Graphics::FontWeight>::MakeWithDefaultValue(400);
StaticAttribute<Graphics::FontWeight> const ApplicationStyle::FontWeightDisplaySmall = StaticAttribute<Graphics::FontWeight>::MakeWithDefaultValue(400);
StaticAttribute<Graphics::FontWeight> const ApplicationStyle::FontWeightDisplayMedium = StaticAttribute<Graphics::FontWeight>::MakeWithDefaultValue(400);
StaticAttribute<Graphics::FontWeight> const ApplicationStyle::FontWeightDisplayLarge = StaticAttribute<Graphics::FontWeight>::MakeWithDefaultValue(400);
}

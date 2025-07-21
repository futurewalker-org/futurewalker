// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ButtonViewStyle.hpp"
#include "Futurewalker.Application.ApplicationStyle.hpp"

namespace FW_DETAIL_NS
{
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(ButtonViewStyle::BackgroundColor, ApplicationStyle::ColorSecondaryContainer);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(ButtonViewStyle::HighlightColor, ApplicationStyle::ColorOnSecondaryContainer);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(ButtonViewStyle::BorderColor, ApplicationStyle::ColorOutlineVariant);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(ButtonViewStyle::HoverHighlightAlpha, ApplicationStyle::AlphaHighlightHover);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(ButtonViewStyle::PressHighlightAlpha, ApplicationStyle::AlphaHighlightPress);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(ButtonViewStyle::CornerRadius, ApplicationStyle::CornerRadiusSmall);
FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(ButtonViewStyle::BorderWidth, 0);
}

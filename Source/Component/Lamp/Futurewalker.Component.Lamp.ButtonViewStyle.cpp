// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.ButtonViewStyle.hpp"
#include "Futurewalker.Component.Lamp.Style.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(ButtonViewStyle::BackgroundColor, Style::ColorSecondaryContainer);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(ButtonViewStyle::BackgroundAlpha, Style::AlphaEnabled);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(ButtonViewStyle::DisabledBackgroundColor, Style::ColorSecondaryContainer);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(ButtonViewStyle::DisabledBackgroundAlpha, Style::AlphaDisabled);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(ButtonViewStyle::BorderColor, Style::ColorOutlineVariant);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(ButtonViewStyle::BorderAlpha, Style::AlphaEnabled);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(ButtonViewStyle::DisabledBorderColor, Style::ColorOutlineVariant);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(ButtonViewStyle::DisabledBorderAlpha, Style::AlphaDisabled);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(ButtonViewStyle::HighlightColor, Style::ColorOnSecondaryContainer);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(ButtonViewStyle::HoverHighlightAlpha, Style::AlphaHighlightHover);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(ButtonViewStyle::PressHighlightAlpha, Style::AlphaHighlightPress);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(ButtonViewStyle::CornerRadius, Style::CornerRadiusSmall);
FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(ButtonViewStyle::BorderWidth, 0);
}
}

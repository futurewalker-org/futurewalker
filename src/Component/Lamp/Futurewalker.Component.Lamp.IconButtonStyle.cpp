// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.IconButtonStyle.hpp"
#include "Futurewalker.Component.Lamp.ButtonViewStyle.hpp"
#include "Futurewalker.Component.Lamp.IconViewStyle.hpp"
#include "Futurewalker.Component.Lamp.Style.hpp"

namespace FW_LAMP_DETAIL_NS
{
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(IconButtonStyle::BackgroundColor, Style::ColorSurface);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(IconButtonStyle::BackgroundAlpha, Style::AlphaNone);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(IconButtonStyle::DisabledBackgroundColor, IconButtonStyle::BackgroundAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(IconButtonStyle::DisabledBackgroundAlpha, IconButtonStyle::BackgroundAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(IconButtonStyle::BorderColor, Style::ColorOutlineVariant);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(IconButtonStyle::BorderAlpha, Style::AlphaNone);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(IconButtonStyle::BorderWidth, Style::SizeNone);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(IconButtonStyle::DisabledBorderColor, IconButtonStyle::BorderColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(IconButtonStyle::DisabledBorderAlpha, IconButtonStyle::BorderAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(IconButtonStyle::DisabledBorderWidth, IconButtonStyle::BorderWidth);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(IconButtonStyle::FocusedBorderColor, ButtonViewStyle::FocusedBorderColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(IconButtonStyle::FocusedBorderAlpha, ButtonViewStyle::FocusedBorderAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(IconButtonStyle::FocusedBorderWidth, ButtonViewStyle::FocusedBorderWidth);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(IconButtonStyle::HighlightColor, ButtonViewStyle::HighlightColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(IconButtonStyle::CornerRadius, Style::CornerRadiusLargest);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(IconButtonStyle::Padding, Style::PaddingExtraSmall);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(IconButtonStyle::HoverHighlightAlpha, Style::AlphaHighlightHover);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(IconButtonStyle::PressHighlightAlpha, Style::AlphaHighlightPress);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(IconButtonStyle::IconSize, IconViewStyle::Size);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(IconButtonStyle::IconColor, IconViewStyle::Color);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(IconButtonStyle::IconAlpha, IconViewStyle::Alpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(IconButtonStyle::IconDisabledColor, IconViewStyle::DisabledColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(IconButtonStyle::IconDisabledAlpha, IconViewStyle::DisabledAlpha);
}

// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.MenuViewStyle.hpp"
#include "Futurewalker.Component.Lamp.Style.hpp"
#include "Futurewalker.Component.Lamp.TextViewStyle.hpp"
#include "Futurewalker.Component.Lamp.IconViewStyle.hpp"

namespace FW_LAMP_DETAIL_NS
{
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuViewStyle::IconColor, IconViewStyle::Color);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuViewStyle::DisabledIconColor, IconViewStyle::DisabledColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuViewStyle::TextColor, TextViewStyle::Color);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuViewStyle::DisabledTextColor, TextViewStyle::DisabledColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuViewStyle::HighlightColor, Style::ColorOnSurface);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuViewStyle::BackgroundColor, Style::ColorSurfaceContainerLow);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuViewStyle::DisabledBackgroundColor, Style::ColorSurfaceContainerLow);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuViewStyle::BorderColor, Style::ColorSecondary);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuViewStyle::DisabledBorderColor, Style::ColorSecondary);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuViewStyle::IconAlpha, IconViewStyle::Alpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuViewStyle::DisabledIconAlpha, IconViewStyle::DisabledAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuViewStyle::TextAlpha, TextViewStyle::Alpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuViewStyle::DisabledTextAlpha, TextViewStyle::DisabledAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuViewStyle::HoverHighlightAlpha, Style::AlphaHighlightHover);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuViewStyle::PressHighlightAlpha, Style::AlphaHighlightPress);
FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(MenuViewStyle::BorderWidth, 0.0);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuViewStyle::Padding, Style::PaddingExtraSmall);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuViewStyle::CornerRadius, Style::CornerRadiusExtraSmall);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuViewStyle::ItemPadding, Style::PaddingSmall);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuViewStyle::ItemSpacing, Style::SpaceExtraSmall);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuViewStyle::ItemCornerRadius, Style::CornerRadiusExtraSmall);
}

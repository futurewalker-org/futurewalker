// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.MenuItemButtonStyle.hpp"
#include "Futurewalker.Component.Lamp.MenuViewStyle.hpp"
#include "Futurewalker.Component.Lamp.Style.hpp"
#include "Futurewalker.Component.Lamp.ButtonViewStyle.hpp"

namespace FW_LAMP_DETAIL_NS
{
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::BackgroundColor, MenuViewStyle::BackgroundColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::BackgroundAlpha, Style::AlphaNone);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::DisabledBackgroundColor, MenuViewStyle::DisabledBackgroundColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::DisabledBackgroundAlpha, Style::AlphaNone);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::BorderColor, ButtonViewStyle::BorderColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::BorderAlpha, ButtonViewStyle::BorderAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::BorderWidth, ButtonViewStyle::BorderWidth);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::DisabledBorderColor, ButtonViewStyle::DisabledBorderColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::DisabledBorderAlpha, ButtonViewStyle::DisabledBorderAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::DisabledBorderWidth, ButtonViewStyle::DisabledBorderWidth);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::FocusedBorderColor, ButtonViewStyle::FocusedBorderColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::FocusedBorderAlpha, ButtonViewStyle::FocusedBorderAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::FocusedBorderWidth, ButtonViewStyle::FocusedBorderWidth);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::HighlightColor, ButtonViewStyle::HighlightColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::CornerRadius, Style::CornerRadiusSmall);
FW_STATIC_ATTRIBUTE_DEFAULT_FUNCTION(MenuItemButtonStyle::Padding, [](auto const& s, auto const& m) { return s.GetVertical() + m.GetHorizontal(); }, Style::PaddingExtraSmall, Style::PaddingSmall);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::HoverHighlightAlpha, ButtonViewStyle::HoverHighlightAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::PressHighlightAlpha, ButtonViewStyle::PressHighlightAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::TextColor, MenuViewStyle::TextColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::TextAlpha, MenuViewStyle::TextAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::DisabledTextColor, MenuViewStyle::DisabledTextColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::DisabledTextAlpha, MenuViewStyle::DisabledTextAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::IconColor, MenuViewStyle::IconColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::IconAlpha, MenuViewStyle::IconAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::DisabledIconColor, MenuViewStyle::DisabledIconColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(MenuItemButtonStyle::DisabledIconAlpha, MenuViewStyle::DisabledIconAlpha);
}

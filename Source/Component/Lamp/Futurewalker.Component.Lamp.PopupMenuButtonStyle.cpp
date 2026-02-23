// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.PopupMenuButtonStyle.hpp"
#include "Futurewalker.Component.Lamp.ButtonViewStyle.hpp"
#include "Futurewalker.Component.Lamp.Style.hpp"

namespace FW_LAMP_DETAIL_NS
{
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(PopupMenuButtonStyle::BackgroundColor, ButtonViewStyle::BackgroundColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(PopupMenuButtonStyle::BackgroundAlpha, ButtonViewStyle::BackgroundAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(PopupMenuButtonStyle::DisabledBackgroundColor, ButtonViewStyle::DisabledBackgroundColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(PopupMenuButtonStyle::DisabledBackgroundAlpha, ButtonViewStyle::DisabledBackgroundAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(PopupMenuButtonStyle::BorderColor, ButtonViewStyle::BorderColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(PopupMenuButtonStyle::BorderAlpha, ButtonViewStyle::BorderAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(PopupMenuButtonStyle::BorderWidth, ButtonViewStyle::BorderWidth);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(PopupMenuButtonStyle::DisabledBorderColor, ButtonViewStyle::DisabledBorderColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(PopupMenuButtonStyle::DisabledBorderAlpha, ButtonViewStyle::DisabledBorderAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(PopupMenuButtonStyle::DisabledBorderWidth, ButtonViewStyle::DisabledBorderWidth);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(PopupMenuButtonStyle::FocusedBorderColor, ButtonViewStyle::FocusedBorderColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(PopupMenuButtonStyle::FocusedBorderAlpha, ButtonViewStyle::FocusedBorderAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(PopupMenuButtonStyle::FocusedBorderWidth, ButtonViewStyle::FocusedBorderWidth);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(PopupMenuButtonStyle::HighlightColor, ButtonViewStyle::HighlightColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(PopupMenuButtonStyle::CornerRadius, Style::CornerRadiusSmall);
FW_STATIC_ATTRIBUTE_DEFAULT_FUNCTION(PopupMenuButtonStyle::Padding, [](auto const& s, auto const& m) { return s.GetVertical() + m.GetHorizontal(); }, Style::PaddingSmall, Style::PaddingMedium);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(PopupMenuButtonStyle::HoverHighlightAlpha, ButtonViewStyle::HoverHighlightAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(PopupMenuButtonStyle::PressHighlightAlpha, ButtonViewStyle::PressHighlightAlpha);
}

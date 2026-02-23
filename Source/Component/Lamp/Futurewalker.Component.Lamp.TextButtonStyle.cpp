// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.TextButtonStyle.hpp"
#include "Futurewalker.Component.Lamp.TextViewStyle.hpp"
#include "Futurewalker.Component.Lamp.ButtonViewStyle.hpp"
#include "Futurewalker.Component.Lamp.Style.hpp"

namespace FW_LAMP_DETAIL_NS
{
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::BackgroundColor, ButtonViewStyle::BackgroundColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::BackgroundAlpha, ButtonViewStyle::BackgroundAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::DisabledBackgroundColor, ButtonViewStyle::DisabledBackgroundColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::DisabledBackgroundAlpha, ButtonViewStyle::DisabledBackgroundAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::BorderColor, ButtonViewStyle::BorderColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::BorderAlpha, ButtonViewStyle::BorderAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::BorderWidth, ButtonViewStyle::BorderWidth);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::DisabledBorderColor, ButtonViewStyle::DisabledBorderColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::DisabledBorderAlpha, ButtonViewStyle::DisabledBorderAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::DisabledBorderWidth, ButtonViewStyle::DisabledBorderWidth);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::FocusedBorderColor, ButtonViewStyle::FocusedBorderColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::FocusedBorderAlpha, ButtonViewStyle::FocusedBorderAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::FocusedBorderWidth, ButtonViewStyle::FocusedBorderWidth);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::HighlightColor, ButtonViewStyle::HighlightColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::CornerRadius, ButtonViewStyle::CornerRadius);
FW_STATIC_ATTRIBUTE_DEFAULT_FUNCTION(TextButtonStyle::Padding, [](auto const& s, auto const& m) { return s.GetVertical() + m.GetHorizontal(); }, Style::PaddingSmall, Style::PaddingMedium);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::HoverHighlightAlpha, Style::AlphaHighlightHover);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::PressHighlightAlpha, Style::AlphaHighlightPress);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::TextColor, Style::ColorOnSecondaryContainer);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::DisabledTextColor, TextViewStyle::DisabledColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::TextFontSize, TextViewStyle::FontSize);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::TextFontWeight, TextViewStyle::FontWeight);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::TextFontWidth, TextViewStyle::FontWidth);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::TextFontSlant, TextViewStyle::FontSlant);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::TextFontFamily, TextViewStyle::FontFamily);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::TextHorizontalAlignment, TextViewStyle::HorizontalAlignment);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::TextVerticalAlignment, TextViewStyle::VerticalAlignment);
}

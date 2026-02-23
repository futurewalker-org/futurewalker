// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.TextEditStyle.hpp"
#include "Futurewalker.Component.Lamp.Style.hpp"
#include "Futurewalker.Component.Lamp.TextViewStyle.hpp"

namespace FW_LAMP_DETAIL_NS
{
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::BackgroundColor, Style::ColorSurfaceContainer);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::BackgroundAlpha, Style::AlphaEnabled);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::DisabledBackgroundColor, Style::ColorSurfaceVariant);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::DisabledBackgroundAlpha, Style::AlphaDisabled);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::TextColor, TextViewStyle::Color);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::TextAlpha, TextViewStyle::Alpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::DisabledTextColor, TextViewStyle::DisabledColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::DisabledTextAlpha, TextViewStyle::DisabledAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::BorderColor, Style::ColorOutline);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::BorderAlpha, Style::AlphaEnabled);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::BorderWidth, Style::Size10);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::DisabledBorderColor, TextEditStyle::BorderColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::DisabledBorderAlpha, Style::AlphaDisabled);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::DisabledBorderWidth, Style::Size10);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::FocusedBorderColor, Style::ColorPrimary);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::FocusedBorderAlpha, Style::AlphaEnabled);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::FocusedBorderWidth, Style::Size20);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::BorderRadius, Style::CornerRadiusSmall);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::Padding, Style::PaddingExtraSmall);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::FontSize, TextViewStyle::FontSize);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::FontWeight, TextViewStyle::FontWeight);
FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(TextEditStyle::FontWidth, Graphics::FontWidth::Normal());
FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(TextEditStyle::FontSlant, Graphics::FontSlant::Upright);
FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(TextEditStyle::FontFamily, Graphics::FontFamily());
}

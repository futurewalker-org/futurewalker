// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.TextViewStyle.hpp"
#include "Futurewalker.Component.Lamp.Style.hpp"

#include "Futurewalker.Graphics.FontFamily.hpp"

namespace FW_LAMP_DETAIL_NS
{
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextViewStyle::Color, Style::ColorOnSurface);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextViewStyle::Alpha, Style::AlphaEnabled);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextViewStyle::DisabledColor, Style::ColorOnSurface);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextViewStyle::DisabledAlpha, Style::AlphaDisabled);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextViewStyle::FontSize, Style::FontSizeLabelMedium);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextViewStyle::FontWeight, Style::FontWeightLabelMedium);
FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(TextViewStyle::FontWidth, Graphics::FontWidth::Normal());
FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(TextViewStyle::FontSlant, Graphics::FontSlant::Upright);
FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(TextViewStyle::FontFamily, Graphics::FontFamily());
FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(TextViewStyle::HorizontalAlignment, TextViewHorizontalAlignment::Leading);
FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(TextViewStyle::VerticalAlignment, TextViewVerticalAlignment::Middle);
}

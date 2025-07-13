// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.TextViewStyle.hpp"
#include "Futurewalker.Application.ApplicationStyle.hpp"

namespace FW_DETAIL_NS
{
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextViewStyle::Color, ApplicationStyle::ColorOnSurface);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextViewStyle::FontSize, ApplicationStyle::FontSizeLabelMedium);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextViewStyle::FontWeight, ApplicationStyle::FontWeightLabelMedium);
FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(TextViewStyle::FontWidth, Graphics::FontWidth::Normal());
FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(TextViewStyle::FontSlant, Graphics::FontSlant::Upright);
FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(TextViewStyle::FontFamily, Graphics::FontFamily());
FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(TextViewStyle::HorizontalAlignment, TextViewHorizontalAlignment::Center);
FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(TextViewStyle::VerticalAlignment, TextViewVerticalAlignment::Middle);
}

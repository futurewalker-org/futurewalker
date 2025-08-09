// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.TextEditStyle.hpp"
#include "Futurewalker.Application.TextViewStyle.hpp"
#include "Futurewalker.Application.ApplicationStyle.hpp"

namespace FW_DETAIL_NS
{
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::BackgroundColor, TextViewStyle::Color);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::TextColor, ApplicationStyle::ColorSurfaceContainer);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::FontSize, ApplicationStyle::FontSizeLabelMedium);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextEditStyle::FontWeight, ApplicationStyle::FontWeightLabelMedium);
FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(TextEditStyle::FontWidth, Graphics::FontWidth::Normal());
FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(TextEditStyle::FontSlant, Graphics::FontSlant::Upright);
FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(TextEditStyle::FontFamily, Graphics::FontFamily());
}

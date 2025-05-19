// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.TextViewStyle.hpp"
#include "Futurewalker.Application.ApplicationStyle.hpp"

namespace FW_DETAIL_NS
{
StaticAttribute<RGBAColor> const TextViewStyle::Color = StaticAttribute<RGBAColor>::MakeWithDefaultReference<&ApplicationStyle::ColorOnSurface>();
StaticAttribute<Graphics::FontSize> const TextViewStyle::FontSize = StaticAttribute<Graphics::FontSize>::MakeWithDefaultReference<&ApplicationStyle::FontSizeLabelMedium>();
StaticAttribute<Graphics::FontWeight> const TextViewStyle::FontWeight = StaticAttribute<Graphics::FontWeight>::MakeWithDefaultReference<&ApplicationStyle::FontWeightLabelMedium>();
StaticAttribute<Graphics::FontWidth> const TextViewStyle::FontWidth = StaticAttribute<Graphics::FontWidth>::MakeWithDefaultValue(Graphics::FontWidth::Normal());
StaticAttribute<Graphics::FontSlant> const TextViewStyle::FontSlant = StaticAttribute<Graphics::FontSlant>::MakeWithDefaultValue(Graphics::FontSlant::Upright);
StaticAttribute<Graphics::FontFamily> const TextViewStyle::FontFamily = StaticAttribute<Graphics::FontFamily>::MakeWithDefaultValue(Graphics::FontFamily());
StaticAttribute<TextViewHorizontalAlignment> const TextViewStyle::HorizontalAlignment = StaticAttribute<TextViewHorizontalAlignment>::MakeWithDefaultValue(TextViewHorizontalAlignment::Center);
StaticAttribute<TextViewVerticalAlignment> const TextViewStyle::VerticalAlignment = StaticAttribute<TextViewVerticalAlignment>::MakeWithDefaultValue(TextViewVerticalAlignment::Middle);
}

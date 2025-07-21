// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.TextButtonStyle.hpp"
#include "Futurewalker.Application.ButtonViewStyle.hpp"
#include "Futurewalker.Application.TextViewStyle.hpp"

namespace FW_DETAIL_NS
{
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::BackgroundColor, ButtonViewStyle::BackgroundColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::HighlightColor, ButtonViewStyle::HighlightColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::BorderColor, ButtonViewStyle::BorderColor);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::BorderWidth, ButtonViewStyle::BorderWidth);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::CornerRadius, ButtonViewStyle::CornerRadius);
FW_STATIC_ATTRIBUTE_DEFAULT_VALUE(TextButtonStyle::Padding, EdgeInsets::MakeUniform(8));
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::HoverHighlightAlpha, ButtonViewStyle::HoverHighlightAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::PressHighlightAlpha, ButtonViewStyle::PressHighlightAlpha);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::TextColor, TextViewStyle::Color);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::TextFontSize, TextViewStyle::FontSize);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::TextFontWeight, TextViewStyle::FontWeight);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::TextFontWidth, TextViewStyle::FontWidth);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::TextFontSlant, TextViewStyle::FontSlant);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::TextFontFamily, TextViewStyle::FontFamily);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::TextHorizontalAlignment, TextViewStyle::HorizontalAlignment);
FW_STATIC_ATTRIBUTE_DEFAULT_REFERENCE(TextButtonStyle::TextVerticalAlignment, TextViewStyle::VerticalAlignment);
}

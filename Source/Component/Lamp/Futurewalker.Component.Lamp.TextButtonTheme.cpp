// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.TextButtonTheme.hpp"
#include "Futurewalker.Component.Lamp.TextButtonStyle.hpp"
#include "Futurewalker.Component.Lamp.Style.hpp"

#include "Futurewalker.Attribute.AttributeNode.hpp"

namespace FW_LAMP_DETAIL_NS
{
TextButtonTheme::TextButtonTheme(Type const type)
  : _type(type)
{
}

auto TextButtonTheme::Apply(AttributeNode& target) -> void
{
    if (_type == Type::Filled)
    {
        AttributeNode::SetReference<&TextButtonStyle::BackgroundColor>(target, Style::ColorPrimary);
        AttributeNode::SetReference<&TextButtonStyle::TextColor>(target, Style::ColorOnPrimary);
        AttributeNode::SetReference<&TextButtonStyle::HighlightColor>(target, Style::ColorOnPrimary);
        AttributeNode::SetReference<&TextButtonStyle::BorderColor>(target, Style::ColorOutline);
        AttributeNode::SetReference<&TextButtonStyle::BorderWidth>(target, Style::SizeNone);
    }
    else if (_type == Type::Tonal)
    {
        AttributeNode::SetReference<&TextButtonStyle::BackgroundColor>(target, Style::ColorSecondaryContainer);
        AttributeNode::SetReference<&TextButtonStyle::TextColor>(target, Style::ColorOnSecondaryContainer);
        AttributeNode::SetReference<&TextButtonStyle::HighlightColor>(target, Style::ColorOnSecondaryContainer);
        AttributeNode::SetReference<&TextButtonStyle::BorderColor>(target, Style::ColorOutlineVariant);
        AttributeNode::SetReference<&TextButtonStyle::BorderWidth>(target, Style::SizeNone);
    }
    else if (_type == Type::Outlined)
    {
        AttributeNode::SetReference<&TextButtonStyle::BackgroundColor>(target, Style::ColorSurface);
        AttributeNode::SetReference<&TextButtonStyle::BackgroundAlpha>(target, Style::AlphaNone);
        AttributeNode::SetReference<&TextButtonStyle::DisabledBackgroundAlpha>(target, Style::AlphaNone);
        AttributeNode::SetReference<&TextButtonStyle::TextColor>(target, Style::ColorOnSecondaryContainer);
        AttributeNode::SetReference<&TextButtonStyle::HighlightColor>(target, Style::ColorOnSecondaryContainer);
        AttributeNode::SetReference<&TextButtonStyle::BorderColor>(target, Style::ColorOutlineVariant);
        AttributeNode::SetReference<&TextButtonStyle::BorderWidth>(target, Style::Size10);
    }
    else if (_type == Type::Text)
    {
        AttributeNode::SetReference<&TextButtonStyle::BackgroundColor>(target, Style::ColorSurface);
        AttributeNode::SetReference<&TextButtonStyle::BackgroundAlpha>(target, Style::AlphaNone);
        AttributeNode::SetReference<&TextButtonStyle::DisabledBackgroundAlpha>(target, Style::AlphaNone);
        AttributeNode::SetReference<&TextButtonStyle::TextColor>(target, Style::ColorOnSecondaryContainer);
        AttributeNode::SetReference<&TextButtonStyle::HighlightColor>(target, Style::ColorOnSecondaryContainer);
        AttributeNode::SetReference<&TextButtonStyle::BorderColor>(target, Style::ColorOutlineVariant);
        AttributeNode::SetReference<&TextButtonStyle::BorderWidth>(target, Style::SizeNone);
    }
}
}

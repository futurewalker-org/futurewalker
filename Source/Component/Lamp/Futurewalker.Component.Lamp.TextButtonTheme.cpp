// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.TextButtonTheme.hpp"
#include "Futurewalker.Component.Lamp.TextButtonStyle.hpp"
#include "Futurewalker.Component.Lamp.Style.hpp"

#include "Futurewalker.Attribute.AttributeNode.hpp"

namespace FW_LAMP_DETAIL_NS
{
auto TextButtonTheme::Make(Type const type) -> Shared<TextButtonTheme>
{
    return Shared<TextButtonTheme>::Make(type);
}

TextButtonTheme::TextButtonTheme(Type const type)
  : _type(type)
{
}

auto TextButtonTheme::Apply(AttributeNode& target) -> void
{
    if (_type == Type::Filled)
    {
        AttributeNode::SetReference<&TextButtonStyle::BackgroundColor>(target, Style::ColorPrimary);
        AttributeNode::SetReference<&TextButtonStyle::BackgroundAlpha>(target, Style::AlphaOpaque);
        AttributeNode::SetReference<&TextButtonStyle::TextColor>(target, Style::ColorOnPrimary);
        AttributeNode::SetReference<&TextButtonStyle::HighlightColor>(target, Style::ColorOnPrimary);
        AttributeNode::SetReference<&TextButtonStyle::BorderColor>(target, Style::ColorOutline);
        AttributeNode::SetReference<&TextButtonStyle::BorderWidth>(target, Style::SizeNone);
    }
    else if (_type == Type::Tonal)
    {
        AttributeNode::SetReference<&TextButtonStyle::BackgroundColor>(target, Style::ColorSecondaryContainer);
        AttributeNode::SetReference<&TextButtonStyle::BackgroundAlpha>(target, Style::AlphaOpaque);
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

auto TextButtonTheme::Clear(AttributeNode& target) -> void
{
    AttributeNode::Clear<&TextButtonStyle::BackgroundColor>(target);
    AttributeNode::Clear<&TextButtonStyle::BackgroundAlpha>(target);
    AttributeNode::Clear<&TextButtonStyle::TextColor>(target);
    AttributeNode::Clear<&TextButtonStyle::HighlightColor>(target);
    AttributeNode::Clear<&TextButtonStyle::BorderColor>(target);
    AttributeNode::Clear<&TextButtonStyle::BorderWidth>(target);
}
}

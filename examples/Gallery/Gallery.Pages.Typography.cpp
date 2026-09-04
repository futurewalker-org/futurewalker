// SPDX-License-Identifier: MPL-2.0 OR CC0-1.0
#include "Gallery.Pages.Typography.hpp"

#include <Futurewalker.Application.FlexLayout.hpp>
#include <Futurewalker.Application.PaddingView.hpp>
#include <Futurewalker.Application.ScrollView.hpp>
#include <Futurewalker.Application.BoxView.hpp>
#include <Futurewalker.Application.ClipView.hpp>
#include <Futurewalker.Application.Icon.hpp>

#include <Futurewalker.Resource.ResourceManager.hpp>
#include <Futurewalker.Resource.Resource.hpp>

#include <Futurewalker.Graphics.SvgDocument.hpp>

#include <Futurewalker.Component.Lamp.TextView.hpp>
#include <Futurewalker.Component.Lamp.Style.hpp>

#include "Resource/Gallery.hpp"
#include "Resource/Gallery.String.TypographyPage.hpp"

namespace Futurewalker::Gallery
{
namespace
{
auto MakeTextView(StaticAttributeRef<Graphics::FontSize> const fontSize, String const text) -> Shared<View>
{
    auto textView = Lamp::TextView::MakeWithText(text);
    AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*textView, fontSize);
    return textView;
}

auto MakeTextViewWithColors(StaticAttributeRef<Graphics::FontSize> const fontSize, RGBAColor const& fg, RGBAColor const& bg, String const& text)
{
    auto textView = Lamp::TextView::MakeWithText(text);
    AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*textView, fontSize);
    AttributeNode::SetValue<Lamp::TextViewStyle::Color>(*textView, fg);
    auto padding =PaddingView::MakeWithPaddingAndContent(Lamp::Style::PaddingExtraSmall, textView);
    auto box = BoxView::MakeWithContent(padding);
    box->SetBackgroundColor(bg);
    box->SetBackgroundAlpha(1.0);
    return box;
}

auto MakeFontSizeList()
{
    auto innerColumn = FlexLayout::Make();
    innerColumn->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Start);
    innerColumn->AddChild(MakeTextView(Lamp::Style::FontSizeBodySmall, u8"Body Small"));
    innerColumn->AddChild(MakeTextView(Lamp::Style::FontSizeBodyMedium, u8"Body Medium"));
    innerColumn->AddChild(MakeTextView(Lamp::Style::FontSizeBodyLarge, u8"Body Large"));
    innerColumn->AddChild(MakeTextView(Lamp::Style::FontSizeLabelSmall, u8"Label Small"));
    innerColumn->AddChild(MakeTextView(Lamp::Style::FontSizeLabelMedium, u8"Label Medium"));
    innerColumn->AddChild(MakeTextView(Lamp::Style::FontSizeLabelLarge, u8"Label Large"));
    innerColumn->AddChild(MakeTextView(Lamp::Style::FontSizeTitleSmall, u8"Title Small"));
    innerColumn->AddChild(MakeTextView(Lamp::Style::FontSizeTitleMedium, u8"Title Medium"));
    innerColumn->AddChild(MakeTextView(Lamp::Style::FontSizeTitleLarge, u8"Title Large"));
    innerColumn->AddChild(MakeTextView(Lamp::Style::FontSizeHeadlineSmall, u8"Headline Small"));
    innerColumn->AddChild(MakeTextView(Lamp::Style::FontSizeHeadlineMedium, u8"Headline Medium"));
    innerColumn->AddChild(MakeTextView(Lamp::Style::FontSizeHeadlineLarge, u8"Headline Large"));
    innerColumn->AddChild(MakeTextView(Lamp::Style::FontSizeDisplaySmall, u8"Display Small"));
    innerColumn->AddChild(MakeTextView(Lamp::Style::FontSizeDisplayMedium, u8"Display Medium"));
    innerColumn->AddChild(MakeTextView(Lamp::Style::FontSizeDisplayLarge, u8"Display Large"));

    auto padding = PaddingView::MakeWithPaddingAndContent(Lamp::Style::PaddingMedium, innerColumn);
    auto clip = ClipView::MakeWithContent(padding);
    clip->SetCornerRadius(Lamp::Style::CornerRadiusSmall);
    auto container = BoxView::MakeWithContent(clip);
    container->SetBackgroundColor(Lamp::Style::ColorSurfaceContainer);
    container->SetBorderColor(Lamp::Style::ColorOutlineVariant);
    container->SetCornerRadius(Lamp::Style::CornerRadiusSmall);
    container->SetBorderWidth(1);
    return container;
}

auto MakeGammaTestList()
{
    auto innerColumn = FlexLayout::Make();
    innerColumn->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Start);
    auto const text = String(u8"Welcome to the Futurewalker Gallery!");
    innerColumn->AddChild(MakeTextViewWithColors(Lamp::Style::FontSizeDisplayMedium, RGBAColor(0, 0, 0, 1), RGBAColor(1, 1, 1, 1), text));
    innerColumn->AddChild(MakeTextViewWithColors(Lamp::Style::FontSizeDisplayMedium, RGBAColor(1, 1, 1, 1), RGBAColor(0, 0, 0, 1), text));
    innerColumn->AddChild(MakeTextViewWithColors(Lamp::Style::FontSizeDisplayMedium, RGBAColor(0, 0, 0, 1), RGBAColor::MakeFromHex(0xFEF7FFFF), text));
    innerColumn->AddChild(MakeTextViewWithColors(Lamp::Style::FontSizeDisplayMedium, RGBAColor(0, 0, 0, 1), RGBAColor::MakeFromHex(0xF3EDF7FF), text));
    innerColumn->AddChild(MakeTextViewWithColors(Lamp::Style::FontSizeDisplayMedium, RGBAColor(1, 0, 0, 1), RGBAColor(0, 1, 0, 1), text));
    innerColumn->AddChild(MakeTextViewWithColors(Lamp::Style::FontSizeDisplayMedium, RGBAColor(0, 1, 0, 1), RGBAColor(1, 0, 0, 1), text));
    innerColumn->AddChild(MakeTextViewWithColors(Lamp::Style::FontSizeBodyLarge, RGBAColor(0, 0, 0, 1), RGBAColor(1, 1, 1, 1), text));
    innerColumn->AddChild(MakeTextViewWithColors(Lamp::Style::FontSizeBodyLarge, RGBAColor(1, 1, 1, 1), RGBAColor(0, 0, 0, 1), text));
    innerColumn->AddChild(MakeTextViewWithColors(Lamp::Style::FontSizeBodyLarge, RGBAColor(0, 0, 0, 1), RGBAColor::MakeFromHex(0xFEF7FFFF), text));
    innerColumn->AddChild(MakeTextViewWithColors(Lamp::Style::FontSizeBodyLarge, RGBAColor(0, 0, 0, 1), RGBAColor::MakeFromHex(0xF3EDF7FF), text));
    innerColumn->AddChild(MakeTextViewWithColors(Lamp::Style::FontSizeBodyLarge, RGBAColor(1, 0, 0, 1), RGBAColor(0, 1, 0, 1), text));
    innerColumn->AddChild(MakeTextViewWithColors(Lamp::Style::FontSizeBodyLarge, RGBAColor(0, 1, 0, 1), RGBAColor(1, 0, 0, 1), text));
    auto padding = PaddingView::MakeWithPaddingAndContent(Lamp::Style::PaddingMedium, innerColumn);
    auto clip = ClipView::MakeWithContent(padding);
    clip->SetCornerRadius(Lamp::Style::CornerRadiusSmall);
    auto container = BoxView::MakeWithContent(clip);
    container->SetBackgroundColor(Lamp::Style::ColorSurfaceContainer);
    container->SetBorderColor(Lamp::Style::ColorOutlineVariant);
    container->SetCornerRadius(Lamp::Style::CornerRadiusSmall);
    container->SetBorderWidth(1);
    return container;
}
}

auto TypographyPage::Make() -> Shared<TypographyPage>
{
    return View::MakeDerived<TypographyPage>();
}

TypographyPage::TypographyPage(PassKey<View> key)
  : View(key)
{
}

auto TypographyPage::Initialize() -> void
{
    auto resource = ResourceManager::GetResource(M::Gallery);

    auto column = FlexLayout::Make();
    column->SetDirection(FlexLayoutDirection::Column);
    column->SetMainAxisSize(FlexLayoutMainAxisSize::Max);
    column->SetMainAxisAlignment(FlexLayoutMainAxisAlignment::Start);
    column->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Stretch);
    column->SetCrossAxisSize(FlexLayoutCrossAxisSize::Max);
    {
        auto text = Lamp::TextView::MakeWithText(resource->LoadString(R::Gallery::String::TypographyPage::Title));
        AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeHeadlineMedium);
        column->AddChild(text);
    }
    {
        auto text = Lamp::TextView::MakeWithText(resource->LoadString(R::Gallery::String::TypographyPage::FontSizeDescription));
        AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeBodyMedium);
        AttributeNode::SetReference<Lamp::TextViewStyle::Color>(*text, Lamp::Style::ColorOnSurfaceVariant);
        FlexLayout::SetChildMargin(text, EdgeInsets(0, 10, 0, 0));
        column->AddChild(text);
    }
    {
        auto fontSizeListContainer = MakeFontSizeList();
        FlexLayout::SetChildMargin(fontSizeListContainer, EdgeInsets(0, 10, 0, 0));
        column->AddChild(fontSizeListContainer);
    }
    {
        auto text = Lamp::TextView::MakeWithText(resource->LoadString(R::Gallery::String::TypographyPage::AntiAliasingDescription));
        AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeBodyMedium);
        AttributeNode::SetReference<Lamp::TextViewStyle::Color>(*text, Lamp::Style::ColorOnSurfaceVariant);
        FlexLayout::SetChildMargin(text, EdgeInsets(0, 10, 0, 0));
        column->AddChild(text);
    }
    {
        auto gammaTestContainer = MakeGammaTestList();
        FlexLayout::SetChildMargin(gammaTestContainer, EdgeInsets(0, 10, 0, 0));
        column->AddChild(gammaTestContainer);
    }
    auto columnPadding = PaddingView::MakeWithPaddingAndContent(Lamp::Style::PaddingLarge, column);
    auto scroll = ScrollView::MakeWithContent(columnPadding);
    scroll->SetDirection(ScrollViewDirection::Vertical);
    AddChildFront(scroll);
}
}

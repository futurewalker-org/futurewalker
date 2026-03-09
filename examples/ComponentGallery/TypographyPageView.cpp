#include "TypographyPageView.hpp"

#include <Futurewalker.Application.FlexLayout.hpp>
#include <Futurewalker.Application.PaddingView.hpp>
#include <Futurewalker.Application.ScrollView.hpp>
#include <Futurewalker.Application.BoxView.hpp>
#include <Futurewalker.Application.ClipView.hpp>

#include <Futurewalker.Component.Lamp.TextView.hpp>
#include <Futurewalker.Component.Lamp.Style.hpp>

namespace Futurewalker::ComponentGallery
{
namespace
{
auto MakeTextView(StaticAttributeRef<Graphics::FontSize> const fontSize, String const text) -> Shared<View>
{
    auto textView = Lamp::TextView::MakeWithText(text);
    AttributeNode::SetReference<&Lamp::TextViewStyle::FontSize>(*textView, fontSize);
    return textView;
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

}
auto TypographyPageView::Make() -> Shared<TypographyPageView>
{
    return View::MakeDerived<TypographyPageView>();
}

TypographyPageView::TypographyPageView(PassKey<View> key)
  : View(key)
{
}

auto TypographyPageView::Initialize() -> void
{
    auto column = FlexLayout::Make();
    column->SetDirection(FlexLayoutDirection::Column);
    column->SetMainAxisSize(FlexLayoutMainAxisSize::Max);
    column->SetMainAxisAlignment(FlexLayoutMainAxisAlignment::Start);
    column->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Stretch);
    column->SetCrossAxisSize(FlexLayoutCrossAxisSize::Max);
    {
        auto text = Lamp::TextView::MakeWithText(u8"Typography");
        AttributeNode::SetReference<&Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeHeadlineMedium);
        column->AddChild(text);
    }
    {
        auto fontSizeListContainer = MakeFontSizeList();
        FlexLayout::SetChildMargin(fontSizeListContainer, EdgeInsets(0, 10, 0, 0));
        column->AddChild(fontSizeListContainer);
    }
    auto columnPadding = PaddingView::MakeWithPaddingAndContent(Lamp::Style::PaddingLarge, column);
    auto scroll = ScrollView::MakeWithContent(columnPadding);
    scroll->SetDirection(ScrollViewDirection::Vertical);
    AddChildFront(scroll);
}
}

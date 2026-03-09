#include "HomePageView.hpp"

#include <Futurewalker.Application.FlexLayout.hpp>
#include <Futurewalker.Application.PaddingView.hpp>

#include <Futurewalker.Component.Lamp.TextView.hpp>
#include <Futurewalker.Component.Lamp.Style.hpp>

namespace Futurewalker::ComponentGallery
{
auto HomePageView::Make() -> Shared<HomePageView>
{
    return View::MakeDerived<HomePageView>();
}

HomePageView::HomePageView(PassKey<View> key)
  : View(key)
{
}

auto HomePageView::Initialize() -> void
{
    auto column = FlexLayout::Make();
    column->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Start);
    {
        {
            auto text = Lamp::TextView::Make();
            text->SetText(u8"Futurewalker Gallery");
            AttributeNode::SetReference<&Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeDisplaySmall);
            FlexLayout::SetChildMargin(text, EdgeInsets(0, 0, 0, 10));
            column->AddChild(text);
        }
        {
            auto text = Lamp::TextView::Make();
            text->SetText(u8"Welcome to the Futurewalker Gallery! \nThis is a collection of default UI components and design tokens.");
            AttributeNode::SetReference<&Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeBodyLarge);
            column->AddChild(text);
        }
    }
    auto padding = PaddingView::MakeWithPaddingAndContent(Lamp::Style::PaddingLarge, column);
    AddChildBack(padding);
}
}

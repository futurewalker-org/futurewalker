#include "TextEditPageView.hpp"

#include <Futurewalker.Application.FlexLayout.hpp>
#include <Futurewalker.Application.PaddingView.hpp>
#include <Futurewalker.Application.BoxView.hpp>
#include <Futurewalker.Application.ScrollView.hpp>
#include <Futurewalker.Application.ThemeView.hpp>
#include <Futurewalker.Application.SizedView.hpp>
#include <Futurewalker.Application.ClipView.hpp>

#include <Futurewalker.Component.Lamp.Style.hpp>
#include <Futurewalker.Component.Lamp.TextView.hpp>
#include <Futurewalker.Component.Lamp.TextEdit.hpp>

namespace Futurewalker::ComponentGallery
{
namespace
{
auto MakeTextEdits()
{
    auto innerColumn = FlexLayout::Make();
    innerColumn->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Start);
    {
        auto row = FlexLayout::Make();
        row->SetDirection(FlexLayoutDirection::Row);
        row->SetMainAxisSize(FlexLayoutMainAxisSize::Min);
        row->SetCrossAxisSize(FlexLayoutCrossAxisSize::Min);
        auto textEdit = Lamp::TextEdit::Make();
        row->AddChild(SizedView::MakeWithContent(300, -1, textEdit));
        innerColumn->AddChild(row);
    }
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

auto TextEditPageView::Make() -> Shared<TextEditPageView>
{
    return View::MakeDerived<TextEditPageView>();
}

TextEditPageView::TextEditPageView(PassKey<View> key)
  : View(key)
{
}

auto TextEditPageView::Initialize() -> void
{
    auto column = FlexLayout::Make();
    column->SetDirection(FlexLayoutDirection::Column);
    column->SetMainAxisSize(FlexLayoutMainAxisSize::Max);
    column->SetMainAxisAlignment(FlexLayoutMainAxisAlignment::Start);
    column->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Stretch);
    column->SetCrossAxisSize(FlexLayoutCrossAxisSize::Max);
    {
        auto title = Lamp::TextView::MakeWithText(u8"Text Edit");
        AttributeNode::SetReference<&Lamp::TextViewStyle::FontSize>(*title, Lamp::Style::FontSizeHeadlineMedium);
        column->AddChild(title);
        auto fontSizeListContainer = MakeTextEdits();
        FlexLayout::SetChildMargin(fontSizeListContainer, EdgeInsets(0, 10, 0, 0));
        column->AddChild(fontSizeListContainer);
    }
    auto columnPadding = PaddingView::MakeWithPaddingAndContent(Lamp::Style::PaddingLarge, column);
    auto scroll = ScrollView::MakeWithContent(columnPadding);
    scroll->SetDirection(ScrollViewDirection::Vertical);
    AddChildFront(scroll);
}
}

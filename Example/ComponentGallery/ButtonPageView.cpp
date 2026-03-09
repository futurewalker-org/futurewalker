#include "ButtonPageView.hpp"

#include <Futurewalker.Application.FlexLayout.hpp>
#include <Futurewalker.Application.PaddingView.hpp>
#include <Futurewalker.Application.BoxView.hpp>
#include <Futurewalker.Application.ClipView.hpp>
#include <Futurewalker.Application.ScrollView.hpp>
#include <Futurewalker.Application.ThemeView.hpp>

#include <Futurewalker.Component.Lamp.Style.hpp>
#include <Futurewalker.Component.Lamp.TextView.hpp>
#include <Futurewalker.Component.Lamp.TextButton.hpp>

namespace Futurewalker::ComponentGallery
{
namespace
{
auto MakeButtons()
{
    auto innerColumn = FlexLayout::Make();
    innerColumn->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Start);
    {
        auto text = Lamp::TextView::MakeWithText(u8"Enabled Buttons");
        AttributeNode::SetReference<&Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeLabelMedium);
        innerColumn->AddChild(text);
    }
    {
        auto row = FlexLayout::Make();
        row->SetDirection(FlexLayoutDirection::Row);
        row->SetMainAxisSize(FlexLayoutMainAxisSize::Min);
        row->SetCrossAxisSize(FlexLayoutCrossAxisSize::Min);
        auto makeTextButton = [](auto type, auto text, auto enabled) {
            auto theme = Lamp::TextButtonTheme::Make(type);
            auto button1 = Lamp::TextButton::MakeWithText(text);
            auto buttonTheme = ThemeView::MakeWithContent(theme, button1);
            auto buttonPadding = PaddingView::MakeWithPaddingAndContent(EdgeInsets::MakeUniform(5), buttonTheme);
            buttonPadding->SetEnabled(enabled);
            return buttonPadding;
        };
        row->AddChild(makeTextButton(Lamp::TextButtonTheme::Type::Filled, u8"Filled Button", true));
        row->AddChild(makeTextButton(Lamp::TextButtonTheme::Type::Tonal, u8"Tonal Button", true));
        row->AddChild(makeTextButton(Lamp::TextButtonTheme::Type::Outlined, u8"Outlined Button", true));
        row->AddChild(makeTextButton(Lamp::TextButtonTheme::Type::Text, u8"Text Button", true));
        innerColumn->AddChild(row);
    }
    {
        auto text = Lamp::TextView::MakeWithText(u8"Disabled Buttons");
        AttributeNode::SetReference<&Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeLabelMedium);
        innerColumn->AddChild(text);
    }
    {
        auto row = FlexLayout::Make();
        row->SetDirection(FlexLayoutDirection::Row);
        row->SetMainAxisSize(FlexLayoutMainAxisSize::Min);
        row->SetCrossAxisSize(FlexLayoutCrossAxisSize::Min);
        auto makeTextButton = [](auto type, auto text, auto enabled) {
            auto theme = Lamp::TextButtonTheme::Make(type);
            auto button1 = Lamp::TextButton::MakeWithText(text);
            auto buttonTheme = ThemeView::MakeWithContent(theme, button1);
            auto buttonPadding = PaddingView::MakeWithPaddingAndContent(EdgeInsets::MakeUniform(5), buttonTheme);
            buttonPadding->SetEnabled(enabled);
            return buttonPadding;
        };
        row->AddChild(makeTextButton(Lamp::TextButtonTheme::Type::Filled, u8"Filled Button", false));
        row->AddChild(makeTextButton(Lamp::TextButtonTheme::Type::Tonal, u8"Tonal Button", false));
        row->AddChild(makeTextButton(Lamp::TextButtonTheme::Type::Outlined, u8"Outlined Button", false));
        row->AddChild(makeTextButton(Lamp::TextButtonTheme::Type::Text, u8"Text Button", false));
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

auto ButtonPageView::Make() -> Shared<ButtonPageView>
{
    return View::MakeDerived<ButtonPageView>();
}

ButtonPageView::ButtonPageView(PassKey<View> key)
  : View(key)
{
}

auto ButtonPageView::Initialize() -> void
{
    auto column = FlexLayout::Make();
    column->SetDirection(FlexLayoutDirection::Column);
    column->SetMainAxisSize(FlexLayoutMainAxisSize::Max);
    column->SetMainAxisAlignment(FlexLayoutMainAxisAlignment::Start);
    column->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Stretch);
    column->SetCrossAxisSize(FlexLayoutCrossAxisSize::Max);
    {
        auto title = Lamp::TextView::MakeWithText(u8"Text Button");
        AttributeNode::SetReference<&Lamp::TextViewStyle::FontSize>(*title, Lamp::Style::FontSizeHeadlineMedium);
        column->AddChild(title);
        auto fontSizeListContainer = MakeButtons();
        FlexLayout::SetChildMargin(fontSizeListContainer, EdgeInsets(0, 10, 0, 0));
        column->AddChild(fontSizeListContainer);
    }
    auto columnPadding = PaddingView::MakeWithPaddingAndContent(Lamp::Style::PaddingLarge, column);
    auto scroll = ScrollView::MakeWithContent(columnPadding);
    scroll->SetDirection(ScrollViewDirection::Vertical);
    AddChildFront(scroll);
}
}

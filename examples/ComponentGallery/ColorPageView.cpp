#include "ColorPageView.hpp"
#include "ColorSchemeView.hpp"
#include "ColorSwatchView.hpp"

#include <Futurewalker.Application.FlexLayout.hpp>
#include <Futurewalker.Application.PaddingView.hpp>
#include <Futurewalker.Application.AlignView.hpp>
#include <Futurewalker.Application.ScrollView.hpp>
#include <Futurewalker.Application.ThemeView.hpp>
#include <Futurewalker.Application.SizedView.hpp>
#include <Futurewalker.Application.BoxView.hpp>
#include <Futurewalker.Application.ClipView.hpp>

#include <Futurewalker.Component.Lamp.Theme.hpp>
#include <Futurewalker.Component.Lamp.TextView.hpp>
#include <Futurewalker.Component.Lamp.Style.hpp>

namespace Futurewalker::ComponentGallery
{
namespace
{
auto MakeColorPaletteContainer(ThemeBrightness brightness)
{
    auto innerColumn = FlexLayout::Make();
    innerColumn->SetDirection(FlexLayoutDirection::Column);
    innerColumn->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Start);

    auto swatchView = ColorSwatchView::Make();
    auto colorTheme = ThemeView::MakeWithContent(swatchView);
    colorTheme->SetTheme(Shared<Lamp::Theme>::Make(brightness));
    innerColumn->AddChild(colorTheme);

    auto padding = PaddingView::MakeWithPaddingAndContent(Lamp::Style::PaddingMedium, innerColumn);
    auto container = BoxView::MakeWithContent(padding);
    container->SetBackgroundColor(Lamp::Style::ColorSurfaceContainer);
    container->SetBorderColor(Lamp::Style::ColorOutlineVariant);
    container->SetCornerRadius(Lamp::Style::CornerRadiusSmall);
    container->SetBorderWidth(1);
    return container;
}

auto MakeColorSchemeViewContainer(String const text, ThemeBrightness brightness) -> Shared<View>
{
    auto innerColumn = FlexLayout::Make();
    innerColumn->SetDirection(FlexLayoutDirection::Column);
    innerColumn->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Start);

    auto const title = Lamp::TextView::MakeWithText(text);
    AttributeNode::SetReference<&Lamp::TextViewStyle::FontSize>(*title, Lamp::Style::FontSizeTitleMedium);
    innerColumn->AddChild(title);

    auto space = SizedView::Make();
    space->SetHeight(Lamp::Style::Size100);
    innerColumn->AddChild(space);

    auto colorView = ColorSchemeView::Make(brightness);
    auto colorViewAlign = AlignView::MakeWithContent(colorView);
    colorViewAlign->SetAlignment(Alignment::MiddleLeading());
    auto colorTheme = ThemeView::MakeWithContent(colorViewAlign);
    colorTheme->SetTheme(Shared<Lamp::Theme>::Make(brightness));
    innerColumn->AddChild(colorTheme);

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

auto ColorPageView::Make() -> Shared<ColorPageView>
{
    return View::MakeDerived<ColorPageView>();
}

ColorPageView::ColorPageView(PassKey<View> key)
  : View(key)
{
}

auto ColorPageView::Initialize() -> void
{
    auto column = FlexLayout::Make();
    column->SetDirection(FlexLayoutDirection::Column);
    column->SetMainAxisSize(FlexLayoutMainAxisSize::Max);
    column->SetMainAxisAlignment(FlexLayoutMainAxisAlignment::Start);
    column->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Stretch);
    column->SetCrossAxisSize(FlexLayoutCrossAxisSize::Max);
    {
        auto text = Lamp::TextView::MakeWithText(u8"Color Palettes");
        AttributeNode::SetReference<&Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeHeadlineMedium);
        column->AddChild(text);
    }
    {
        auto colorPaletteContainer = MakeColorPaletteContainer(ThemeBrightness::Dark);
        FlexLayout::SetChildMargin(colorPaletteContainer, EdgeInsets(0, 10, 0, 0));
        column->AddChild(colorPaletteContainer);
    }
    {
        auto space = SizedView::Make();
        space->SetHeight(Lamp::Style::Size200);
        column->AddChild(space);
    }
    {
        auto text = Lamp::TextView::MakeWithText(u8"Color Roles");
        AttributeNode::SetReference<&Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeHeadlineMedium);
        column->AddChild(text);
    }
    {
        auto colorSchemeViewContainer = MakeColorSchemeViewContainer(u8"Light Theme", ThemeBrightness::Light);
        FlexLayout::SetChildMargin(colorSchemeViewContainer, EdgeInsets(0, 10, 0, 0));
        column->AddChild(colorSchemeViewContainer);
    }
    {
        auto colorSchemeViewContainer = MakeColorSchemeViewContainer(u8"Dark Theme", ThemeBrightness::Dark);
        FlexLayout::SetChildMargin(colorSchemeViewContainer, EdgeInsets(0, 10, 0, 0));
        column->AddChild(colorSchemeViewContainer);
    }
    auto columnPadding = PaddingView::MakeWithPaddingAndContent(Lamp::Style::PaddingLarge, column);
    auto scroll = ScrollView::MakeWithContent(columnPadding);
    scroll->SetDirection(ScrollViewDirection::Vertical);
    AddChildFront(scroll);
}
}

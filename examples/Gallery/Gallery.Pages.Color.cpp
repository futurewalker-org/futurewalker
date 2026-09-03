// SPDX-License-Identifier: MPL-2.0 OR CC0-1.0
#include "Gallery.Pages.Color.hpp"
#include "Gallery.Views.ColorSchemeView.hpp"
#include "Gallery.Views.ColorPaletteView.hpp"

#include <Futurewalker.Application.FlexLayout.hpp>
#include <Futurewalker.Application.PaddingView.hpp>
#include <Futurewalker.Application.AlignView.hpp>
#include <Futurewalker.Application.ScrollView.hpp>
#include <Futurewalker.Application.ThemeView.hpp>
#include <Futurewalker.Application.SizedView.hpp>
#include <Futurewalker.Application.BoxView.hpp>
#include <Futurewalker.Application.ClipView.hpp>
#include <Futurewalker.Application.Icon.hpp>
#include <Futurewalker.Application.ApplicationTheme.hpp>

#include <Futurewalker.Component.Lamp.Theme.hpp>
#include <Futurewalker.Component.Lamp.TextView.hpp>
#include <Futurewalker.Component.Lamp.Style.hpp>
#include <Futurewalker.Component.Lamp.TextButton.hpp>
#include <Futurewalker.Component.Lamp.IconButton.hpp>

#include <Futurewalker.Resource.ResourceManager.hpp>
#include <Futurewalker.Resource.Resource.hpp>

#include <Futurewalker.Graphics.SvgDocument.hpp>

#include "Resource/Gallery.hpp"
#include "Resource/Gallery.String.ColorPage.hpp"
#include "Resource/Gallery.Icon.hpp"

namespace Futurewalker::Gallery
{
namespace
{
auto MakeColorPaletteContainer()
{
    auto innerColumn = FlexLayout::Make();
    innerColumn->SetDirection(FlexLayoutDirection::Column);
    innerColumn->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Start);

    auto paletteView = ColorPaletteView::Make();
    innerColumn->AddChild(paletteView);

    auto padding = PaddingView::MakeWithPaddingAndContent(Lamp::Style::PaddingMedium, innerColumn);
    auto container = BoxView::MakeWithContent(padding);
    container->SetBackgroundColor(Lamp::Style::ColorSurfaceContainerLow);
    container->SetBorderColor(Lamp::Style::ColorSurfaceContainer);
    container->SetCornerRadius(Lamp::Style::CornerRadiusSmall);
    container->SetBorderWidth(1);
    return container;
}

auto MakeColorSchemeViewContainer() -> Shared<View>
{
    auto innerColumn = FlexLayout::Make();
    innerColumn->SetDirection(FlexLayoutDirection::Column);
    innerColumn->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Start);

    auto space = SizedView::Make();
    space->SetHeight(Lamp::Style::Size100);
    innerColumn->AddChild(space);

    auto colorView = ColorSchemeView::Make();
    auto colorViewAlign = AlignView::MakeWithContent(colorView);
    colorViewAlign->SetAlignment(Alignment::MiddleLeading());
    innerColumn->AddChild(colorViewAlign);

    auto padding = PaddingView::MakeWithPaddingAndContent(Lamp::Style::PaddingMedium, innerColumn);
    auto clip = ClipView::MakeWithContent(padding);
    clip->SetCornerRadius(Lamp::Style::CornerRadiusSmall);
    auto container = BoxView::MakeWithContent(clip);
    container->SetBackgroundColor(Lamp::Style::ColorSurfaceContainerLow);
    container->SetBorderColor(Lamp::Style::ColorSurfaceContainer);
    container->SetCornerRadius(Lamp::Style::CornerRadiusSmall);
    container->SetBorderWidth(1);
    return container;
}
}

auto ColorPage::Make() -> Shared<ColorPage>
{
    return View::MakeDerived<ColorPage>();
}

ColorPage::ColorPage(PassKey<View> key)
  : View(key)
{
}

auto ColorPage::Initialize() -> void
{
    auto resource = ResourceManager::GetResource(M::Gallery);

    auto loadIcon = [&](auto fileId) {
        auto file = resource->LoadFile(fileId);
        auto svg = Graphics::SvgDocument::LoadFromStream(file);
        return Icon::MakeFromSvgDocument(std::move(svg));
    };

    auto column = FlexLayout::Make();
    column->SetDirection(FlexLayoutDirection::Column);
    column->SetMainAxisSize(FlexLayoutMainAxisSize::Max);
    column->SetMainAxisAlignment(FlexLayoutMainAxisAlignment::Start);
    column->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Stretch);
    column->SetCrossAxisSize(FlexLayoutCrossAxisSize::Max);
    {
        auto row = FlexLayout::Make();
        row->SetDirection(FlexLayoutDirection::Row);
        row->SetMainAxisSize(FlexLayoutMainAxisSize::Max);
        row->SetMainAxisAlignment(FlexLayoutMainAxisAlignment::Start);
        row->SetCrossAxisSize(FlexLayoutCrossAxisSize::Min);
        row->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Start);
        auto text = Lamp::TextView::MakeWithText(resource->LoadString(R::Gallery::String::ColorPage::Title));
        AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeHeadlineMedium);
        row->AddChild(text);
        FlexLayout::SetChildGrowFactor(text, 1);
        FlexLayout::SetChildShrinkFactor(text, 1);
        FlexLayout::SetChildMainAxisFlexibility(text, FlexLayoutMainAxisFlexibility::Expand);
        auto button = Lamp::IconButton::MakeWithIcon(loadIcon(R::Gallery::Icon::SunMoon));
        row->AddChild(button);
        column->AddChild(row);
    }
    {
        auto text = Lamp::TextView::MakeWithText(resource->LoadString(R::Gallery::String::ColorPage::ColorPaletteDescription));
        AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeBodyMedium);
        AttributeNode::SetReference<Lamp::TextViewStyle::Color>(*text, Lamp::Style::ColorOnSurfaceVariant);
        FlexLayout::SetChildMargin(text, EdgeInsets(0, 10, 0, 0));
        column->AddChild(text);
    }
    {
        auto colorPaletteContainer = MakeColorPaletteContainer();
        _colorPaletteThemeView = ThemeView::MakeWithContent(colorPaletteContainer);
        FlexLayout::SetChildMargin(_colorPaletteThemeView, EdgeInsets(0, 10, 0, 0));
        column->AddChild(_colorPaletteThemeView);
    }
    {
        auto space = SizedView::Make();
        space->SetHeight(Lamp::Style::Size200);
        column->AddChild(space);
    }
    {
        auto text = Lamp::TextView::MakeWithText(resource->LoadString(R::Gallery::String::ColorPage::ColorSchemeDescription));
        AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeBodyMedium);
        AttributeNode::SetReference<Lamp::TextViewStyle::Color>(*text, Lamp::Style::ColorOnSurfaceVariant);
        FlexLayout::SetChildMargin(text, EdgeInsets(0, 10, 0, 0));
        column->AddChild(text);
    }
    {
        auto colorSchemeViewContainer = MakeColorSchemeViewContainer();
        _colorSchemeThemeView = ThemeView::MakeWithContent(colorSchemeViewContainer);
        FlexLayout::SetChildMargin(_colorSchemeThemeView, EdgeInsets(0, 10, 0, 0));
        column->AddChild(_colorSchemeThemeView);
    }
    auto columnPadding = PaddingView::MakeWithPaddingAndContent(Lamp::Style::PaddingLarge, column);
    auto scroll = ScrollView::MakeWithContent(columnPadding);
    scroll->SetDirection(ScrollViewDirection::Vertical);
    AddChildFront(scroll);

    EventReceiver::Connect(*this, *this, &ColorPage::ReceiveEvent);
}

auto ColorPage::ReceiveEvent(Event<>& event) -> Bool
{
    if (event.Is<Lamp::IconButtonEvent::Press>())
    {
        auto const currentBrightness = _currentBrightness.GetValueOr(ApplicationTheme::GetCurrentBrightness());
        _currentBrightness = currentBrightness == ThemeBrightness::Light ? ThemeBrightness::Dark : ThemeBrightness::Light;
        _colorPaletteThemeView->SetTheme(Shared<Lamp::Theme>::Make(*_currentBrightness));
        _colorSchemeThemeView->SetTheme(Shared<Lamp::Theme>::Make(*_currentBrightness));
        return true;
    }
    return false;
}
}

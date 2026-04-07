// SPDX-License-Identifier: MPL-2.0

#include "ColorSchemeView.hpp"

#include <Futurewalker.Application.FlexLayout.hpp>
#include <Futurewalker.Application.SizedView.hpp>
#include <Futurewalker.Application.AlignView.hpp>
#include <Futurewalker.Application.PaddingView.hpp>
#include <Futurewalker.Application.BoxView.hpp>

#include <Futurewalker.Component.Lamp.Style.hpp>
#include <Futurewalker.Component.Lamp.TextView.hpp>

namespace Futurewalker::ThemeViewer
{
namespace
{
auto GetThemeName(ThemeBrightness brightness) -> String
{
    switch (brightness)
    {
        case ThemeBrightness::Light:
            return u8"Light Theme";
        case ThemeBrightness::Dark:
            return u8"Dark Theme";
        default:
            return u8"Unknown Theme";
    }
}

auto MakeColorTile(Size<Dp> size, StaticAttributeRef<RGBAColor> baseColor, StaticAttributeRef<RGBAColor> onColor, String colorName)
{
    auto text = Lamp::TextView::MakeWithText(colorName);
    AttributeNode::SetReference<&Lamp::TextViewStyle::Color>(*text, onColor);
    auto align = AlignView::MakeWithContent(text);
    align->SetAlignment(Alignment::TopLeading());
    auto container = BoxView::MakeWithContent(align);
    container->SetBackgroundColor(baseColor);
    auto sized = SizedView::MakeWithContent(container);
    sized->SetWidth(size.width);
    sized->SetHeight(size.height);
    return sized;
}

auto MakeColorTileSet(StaticAttributeRef<RGBAColor> baseColor, String baseColorName, StaticAttributeRef<RGBAColor> onColor, String onColorName)
{
    auto column = FlexLayout::Make();
    column->SetDirection(FlexLayoutDirection::Column);
    column->AddChild(MakeColorTile({200, 100}, baseColor, onColor, baseColorName));
    column->AddChild(MakeColorTile({200, 50}, onColor, baseColor, onColorName));
    return column;
}
}

auto ColorSchemeView::Make(ThemeBrightness brightness) -> Shared<ColorSchemeView>
{
    return View::MakeDerived<ColorSchemeView>(brightness);
}

ColorSchemeView::ColorSchemeView(PassKey<View> key, ThemeBrightness brightness)
  : View(key)
  , _brightness {brightness}
{
}

auto ColorSchemeView::Initialize() -> void
{
    auto column = FlexLayout::Make();
    column->SetDirection(FlexLayoutDirection::Column);
    column->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Stretch);
    column->SetCrossAxisSize(FlexLayoutCrossAxisSize::Min);

    auto title = Lamp::TextView::MakeWithText(GetThemeName(_brightness));

    AttributeNode::SetReference<&Lamp::TextViewStyle::FontSize>(*title, Lamp::Style::FontSizeTitleMedium);
    AttributeNode::SetValue<&Lamp::TextViewStyle::FontWeight>(*title, Graphics::FontWeight::Bold());
    AttributeNode::SetValue<&Lamp::TextViewStyle::HorizontalAlignment>(*title, TextViewHorizontalAlignment::Leading);
    column->AddChild(title);

    auto innerRow = FlexLayout::Make();
    innerRow->SetDirection(FlexLayoutDirection::Row);
    innerRow->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Stretch);
    column->AddChild(innerRow);

    auto leftColumn = FlexLayout::Make();
    leftColumn->SetDirection(FlexLayoutDirection::Column);
    FlexLayout::SetChildMargin(leftColumn, EdgeInsets(0, 0, 10, 0));
    innerRow->AddChild(leftColumn);
    {
        {
            auto row = FlexLayout::Make();
            row->SetDirection(FlexLayoutDirection::Row);
            row->AddChild(MakeColorTileSet(Lamp::Style::ColorPrimary, u8"Primary", Lamp::Style::ColorOnPrimary, u8"On Primary"));
            row->AddChild(MakeColorTileSet(Lamp::Style::ColorSecondary, u8"Secondary", Lamp::Style::ColorOnSecondary, u8"On Secondary"));
            row->AddChild(MakeColorTileSet(Lamp::Style::ColorTertiary, u8"Tertiary", Lamp::Style::ColorOnTertiary, u8"On Tertiary"));
            FlexLayout::SetChildMargin(row, EdgeInsets(0, 0, 0, 5));
            leftColumn->AddChild(row);
        }
        {
            auto row = FlexLayout::Make();
            row->SetDirection(FlexLayoutDirection::Row);
            row->AddChild(MakeColorTileSet(Lamp::Style::ColorPrimaryContainer, u8"Primary Container", Lamp::Style::ColorOnPrimaryContainer, u8"On Primary Container"));
            row->AddChild(MakeColorTileSet(Lamp::Style::ColorSecondaryContainer, u8"Secondary Container", Lamp::Style::ColorOnSecondaryContainer, u8"On Secondary Container"));
            row->AddChild(MakeColorTileSet(Lamp::Style::ColorTertiaryContainer, u8"Tertiary Container", Lamp::Style::ColorOnTertiaryContainer, u8"On Tertiary Container"));
            FlexLayout::SetChildMargin(row, EdgeInsets(0, 0, 0, 5));
            leftColumn->AddChild(row);
        }
        {
            auto row = FlexLayout::Make();
            row->SetDirection(FlexLayoutDirection::Row);
            row->AddChild(MakeColorTile({200, 100}, Lamp::Style::ColorSurfaceDim, Lamp::Style::ColorOnSurface, u8"Surface Dim"));
            row->AddChild(MakeColorTile({200, 100}, Lamp::Style::ColorSurface, Lamp::Style::ColorOnSurface, u8"Surface"));
            row->AddChild(MakeColorTile({200, 100}, Lamp::Style::ColorSurfaceBright, Lamp::Style::ColorOnSurface, u8"Surface Bright"));
            FlexLayout::SetChildMargin(row, EdgeInsets(0, 0, 0, 5));
            leftColumn->AddChild(row);
        }
        {
            auto row = FlexLayout::Make();
            row->SetDirection(FlexLayoutDirection::Row);
            row->AddChild(MakeColorTile({600 / 5, 100}, Lamp::Style::ColorSurfaceContainerLowest, Lamp::Style::ColorOnSurface, u8"Surf. Cont. Lowest"));
            row->AddChild(MakeColorTile({600 / 5, 100}, Lamp::Style::ColorSurfaceContainerLow, Lamp::Style::ColorOnSurface, u8"Surf. Cont. Low"));
            row->AddChild(MakeColorTile({600 / 5, 100}, Lamp::Style::ColorSurfaceContainer, Lamp::Style::ColorOnSurface, u8"Surf. Container"));
            row->AddChild(MakeColorTile({600 / 5, 100}, Lamp::Style::ColorSurfaceContainerHigh, Lamp::Style::ColorOnSurface, u8"Surf. Cont. High"));
            row->AddChild(MakeColorTile({600 / 5, 100}, Lamp::Style::ColorSurfaceContainerHighest, Lamp::Style::ColorOnSurface, u8"Surf. Cont. Highest"));
            FlexLayout::SetChildMargin(row, EdgeInsets(0, 0, 0, 5));
            leftColumn->AddChild(row);
        }
        {
            auto row = FlexLayout::Make();
            row->SetDirection(FlexLayoutDirection::Row);
            row->AddChild(MakeColorTile({600 / 4, 50}, Lamp::Style::ColorOnSurface, Lamp::Style::ColorSurface, u8"On Surface"));
            row->AddChild(MakeColorTile({600 / 4, 50}, Lamp::Style::ColorOnSurfaceVariant, Lamp::Style::ColorSurfaceVariant, u8"On Surface Var."));
            row->AddChild(MakeColorTile({600 / 4, 50}, Lamp::Style::ColorOutline, Lamp::Style::ColorSurface, u8"Outline"));
            row->AddChild(MakeColorTile({600 / 4, 50}, Lamp::Style::ColorOutlineVariant, Lamp::Style::ColorOnSurface, u8"Outline Variant"));
            leftColumn->AddChild(row);
        }
    }

    auto rightColumn = FlexLayout::Make();
    rightColumn->SetDirection(FlexLayoutDirection::Column);
    rightColumn->SetMainAxisAlignment(FlexLayoutMainAxisAlignment::Start);
    innerRow->AddChild(rightColumn);
    {
        rightColumn->AddChild(MakeColorTileSet(Lamp::Style::ColorError, u8"Error", Lamp::Style::ColorOnError, u8"On Error"));
        rightColumn->AddChild(SizedView::Make(0, 5));
        rightColumn->AddChild(MakeColorTileSet(Lamp::Style::ColorErrorContainer, u8"Error Container", Lamp::Style::ColorOnErrorContainer, u8"On Error Container"));
    }

    auto padding = PaddingView::MakeWithContent(column);
    padding->SetPadding(Lamp::Style::PaddingMedium);
    auto outerContainer = BoxView::MakeWithContent(padding);
    outerContainer->SetBackgroundColor(Lamp::Style::ColorSurface);
    outerContainer->SetBorderColor(Lamp::Style::ColorOutline);
    outerContainer->SetCornerRadius(Lamp::Style::CornerRadiusMedium);
    outerContainer->SetBorderWidth(1);
    AddChildBack(outerContainer);
}
}

#include "ColorSwatchView.hpp"

#include <Futurewalker.Application.ThemeView.hpp> 
#include <Futurewalker.Application.FlexLayout.hpp>
#include <Futurewalker.Application.ContainerView.hpp>
#include <Futurewalker.Application.ApplicationTheme.hpp>
#include <Futurewalker.Application.AlignView.hpp>
#include <Futurewalker.Application.SizedView.hpp>
#include <Futurewalker.Application.DrawScope.hpp>
#include <Futurewalker.Application.BoxView.hpp>

#include <Futurewalker.Component.Lamp.Color.hpp>
#include <Futurewalker.Component.Lamp.TextView.hpp>
#include <Futurewalker.Component.Lamp.Style.hpp>

#include <Futurewalker.Graphics.Scene.hpp>

#include <Futurewalker.Base.Debug.hpp>

#include <chrono>

namespace Futurewalker::ComponentGallery
{
namespace
{
struct Swatch
{
    Swatch(String const& n, StaticAttributeRef<RGBAColor> const& c)
      : name(n)
      , color(c)
    {
    }
    String name;
    StaticAttributeRef<RGBAColor> color;
};

auto const primaryColors = std::vector<Swatch> {
    {u8"0", Lamp::Color::Primary0},
    {u8"1", Lamp::Color::Primary1},
    {u8"2", Lamp::Color::Primary2},
    {u8"3", Lamp::Color::Primary3},
    {u8"4", Lamp::Color::Primary4},
    {u8"5", Lamp::Color::Primary5},
    {u8"6", Lamp::Color::Primary6},
    {u8"7", Lamp::Color::Primary7},
    {u8"8", Lamp::Color::Primary8},
    {u8"9", Lamp::Color::Primary9},
    {u8"10", Lamp::Color::Primary10},
    {u8"11", Lamp::Color::Primary11},
    {u8"12", Lamp::Color::Primary12},
    {u8"13", Lamp::Color::Primary13},
    {u8"14", Lamp::Color::Primary14},
    {u8"15", Lamp::Color::Primary15},
    {u8"16", Lamp::Color::Primary16},
    {u8"17", Lamp::Color::Primary17},
    {u8"18", Lamp::Color::Primary18},
    {u8"19", Lamp::Color::Primary19},
    {u8"20", Lamp::Color::Primary20},
    {u8"25", Lamp::Color::Primary25},
    {u8"30", Lamp::Color::Primary30},
    {u8"40", Lamp::Color::Primary40},
    {u8"50", Lamp::Color::Primary50},
    {u8"60", Lamp::Color::Primary60},
    {u8"70", Lamp::Color::Primary70},
    {u8"80", Lamp::Color::Primary80},
    {u8"81", Lamp::Color::Primary81},
    {u8"82", Lamp::Color::Primary82},
    {u8"83", Lamp::Color::Primary83},
    {u8"84", Lamp::Color::Primary84},
    {u8"85", Lamp::Color::Primary85},
    {u8"86", Lamp::Color::Primary86},
    {u8"87", Lamp::Color::Primary87},
    {u8"88", Lamp::Color::Primary88},
    {u8"89", Lamp::Color::Primary89},
    {u8"90", Lamp::Color::Primary90},
    {u8"91", Lamp::Color::Primary91},
    {u8"92", Lamp::Color::Primary92},
    {u8"93", Lamp::Color::Primary93},
    {u8"94", Lamp::Color::Primary94},
    {u8"95", Lamp::Color::Primary95},
    {u8"96", Lamp::Color::Primary96},
    {u8"97", Lamp::Color::Primary97},
    {u8"98", Lamp::Color::Primary98},
    {u8"99", Lamp::Color::Primary99},
    {u8"100", Lamp::Color::Primary100},
};

auto const secondaryColors = std::vector<Swatch> {
    {u8"0", Lamp::Color::Secondary0},
    {u8"1", Lamp::Color::Secondary1},
    {u8"2", Lamp::Color::Secondary2},
    {u8"3", Lamp::Color::Secondary3},
    {u8"4", Lamp::Color::Secondary4},
    {u8"5", Lamp::Color::Secondary5},
    {u8"6", Lamp::Color::Secondary6},
    {u8"7", Lamp::Color::Secondary7},
    {u8"8", Lamp::Color::Secondary8},
    {u8"9", Lamp::Color::Secondary9},
    {u8"10", Lamp::Color::Secondary10},
    {u8"11", Lamp::Color::Secondary11},
    {u8"12", Lamp::Color::Secondary12},
    {u8"13", Lamp::Color::Secondary13},
    {u8"14", Lamp::Color::Secondary14},
    {u8"15", Lamp::Color::Secondary15},
    {u8"16", Lamp::Color::Secondary16},
    {u8"17", Lamp::Color::Secondary17},
    {u8"18", Lamp::Color::Secondary18},
    {u8"19", Lamp::Color::Secondary19},
    {u8"20", Lamp::Color::Secondary20},
    {u8"25", Lamp::Color::Secondary25},
    {u8"30", Lamp::Color::Secondary30},
    {u8"40", Lamp::Color::Secondary40},
    {u8"50", Lamp::Color::Secondary50},
    {u8"60", Lamp::Color::Secondary60},
    {u8"70", Lamp::Color::Secondary70},
    {u8"80", Lamp::Color::Secondary80},
    {u8"81", Lamp::Color::Secondary81},
    {u8"82", Lamp::Color::Secondary82},
    {u8"83", Lamp::Color::Secondary83},
    {u8"84", Lamp::Color::Secondary84},
    {u8"85", Lamp::Color::Secondary85},
    {u8"86", Lamp::Color::Secondary86},
    {u8"87", Lamp::Color::Secondary87},
    {u8"88", Lamp::Color::Secondary88},
    {u8"89", Lamp::Color::Secondary89},
    {u8"90", Lamp::Color::Secondary90},
    {u8"91", Lamp::Color::Secondary91},
    {u8"92", Lamp::Color::Secondary92},
    {u8"93", Lamp::Color::Secondary93},
    {u8"94", Lamp::Color::Secondary94},
    {u8"95", Lamp::Color::Secondary95},
    {u8"96", Lamp::Color::Secondary96},
    {u8"97", Lamp::Color::Secondary97},
    {u8"98", Lamp::Color::Secondary98},
    {u8"99", Lamp::Color::Secondary99},
    {u8"100", Lamp::Color::Secondary100},
};

auto const tertiaryColors = std::vector<Swatch> {
    {u8"0", Lamp::Color::Tertiary0},
    {u8"1", Lamp::Color::Tertiary1},
    {u8"2", Lamp::Color::Tertiary2},
    {u8"3", Lamp::Color::Tertiary3},
    {u8"4", Lamp::Color::Tertiary4},
    {u8"5", Lamp::Color::Tertiary5},
    {u8"6", Lamp::Color::Tertiary6},
    {u8"7", Lamp::Color::Tertiary7},
    {u8"8", Lamp::Color::Tertiary8},
    {u8"9", Lamp::Color::Tertiary9},
    {u8"10", Lamp::Color::Tertiary10},
    {u8"11", Lamp::Color::Tertiary11},
    {u8"12", Lamp::Color::Tertiary12},
    {u8"13", Lamp::Color::Tertiary13},
    {u8"14", Lamp::Color::Tertiary14},
    {u8"15", Lamp::Color::Tertiary15},
    {u8"16", Lamp::Color::Tertiary16},
    {u8"17", Lamp::Color::Tertiary17},
    {u8"18", Lamp::Color::Tertiary18},
    {u8"19", Lamp::Color::Tertiary19},
    {u8"20", Lamp::Color::Tertiary20},
    {u8"25", Lamp::Color::Tertiary25},
    {u8"30", Lamp::Color::Tertiary30},
    {u8"40", Lamp::Color::Tertiary40},
    {u8"50", Lamp::Color::Tertiary50},
    {u8"60", Lamp::Color::Tertiary60},
    {u8"70", Lamp::Color::Tertiary70},
    {u8"80", Lamp::Color::Tertiary80},
    {u8"81", Lamp::Color::Tertiary81},
    {u8"82", Lamp::Color::Tertiary82},
    {u8"83", Lamp::Color::Tertiary83},
    {u8"84", Lamp::Color::Tertiary84},
    {u8"85", Lamp::Color::Tertiary85},
    {u8"86", Lamp::Color::Tertiary86},
    {u8"87", Lamp::Color::Tertiary87},
    {u8"88", Lamp::Color::Tertiary88},
    {u8"89", Lamp::Color::Tertiary89},
    {u8"90", Lamp::Color::Tertiary90},
    {u8"91", Lamp::Color::Tertiary91},
    {u8"92", Lamp::Color::Tertiary92},
    {u8"93", Lamp::Color::Tertiary93},
    {u8"94", Lamp::Color::Tertiary94},
    {u8"95", Lamp::Color::Tertiary95},
    {u8"96", Lamp::Color::Tertiary96},
    {u8"97", Lamp::Color::Tertiary97},
    {u8"98", Lamp::Color::Tertiary98},
    {u8"99", Lamp::Color::Tertiary99},
    {u8"100", Lamp::Color::Tertiary100},
};
auto const errorColors = std::vector<Swatch> {
    {u8"0", Lamp::Color::Error0},
    {u8"5", Lamp::Color::Error5},
    {u8"10", Lamp::Color::Error10},
    {u8"15", Lamp::Color::Error15},
    {u8"20", Lamp::Color::Error20},
    {u8"25", Lamp::Color::Error25},
    {u8"30", Lamp::Color::Error30},
    {u8"40", Lamp::Color::Error40},
    {u8"50", Lamp::Color::Error50},
    {u8"60", Lamp::Color::Error60},
    {u8"70", Lamp::Color::Error70},
    {u8"80", Lamp::Color::Error80},
    {u8"90", Lamp::Color::Error90},
    {u8"95", Lamp::Color::Error95},
    {u8"98", Lamp::Color::Error98},
    {u8"99", Lamp::Color::Error99},
    {u8"100", Lamp::Color::Error100},
};

auto const neutralColors = std::vector<Swatch> {
    {u8"0", Lamp::Color::Neutral0},
    {u8"1", Lamp::Color::Neutral1},
    {u8"2", Lamp::Color::Neutral2},
    {u8"3", Lamp::Color::Neutral3},
    {u8"4", Lamp::Color::Neutral4},
    {u8"5", Lamp::Color::Neutral5},
    {u8"6", Lamp::Color::Neutral6},
    {u8"7", Lamp::Color::Neutral7},
    {u8"8", Lamp::Color::Neutral8},
    {u8"9", Lamp::Color::Neutral9},
    {u8"10", Lamp::Color::Neutral10},
    {u8"11", Lamp::Color::Neutral11},
    {u8"12", Lamp::Color::Neutral12},
    {u8"13", Lamp::Color::Neutral13},
    {u8"14", Lamp::Color::Neutral14},
    {u8"15", Lamp::Color::Neutral15},
    {u8"16", Lamp::Color::Neutral16},
    {u8"17", Lamp::Color::Neutral17},
    {u8"18", Lamp::Color::Neutral18},
    {u8"19", Lamp::Color::Neutral19},
    {u8"20", Lamp::Color::Neutral20},
    {u8"20", Lamp::Color::Neutral21},
    {u8"20", Lamp::Color::Neutral22},
    {u8"20", Lamp::Color::Neutral23},
    {u8"20", Lamp::Color::Neutral24},
    {u8"25", Lamp::Color::Neutral25},
    {u8"25", Lamp::Color::Neutral26},
    {u8"25", Lamp::Color::Neutral27},
    {u8"25", Lamp::Color::Neutral28},
    {u8"25", Lamp::Color::Neutral29},
    {u8"30", Lamp::Color::Neutral30},
    {u8"40", Lamp::Color::Neutral40},
    {u8"50", Lamp::Color::Neutral50},
    {u8"60", Lamp::Color::Neutral60},
    {u8"70", Lamp::Color::Neutral70},
    {u8"70", Lamp::Color::Neutral71},
    {u8"70", Lamp::Color::Neutral72},
    {u8"70", Lamp::Color::Neutral73},
    {u8"70", Lamp::Color::Neutral74},
    {u8"70", Lamp::Color::Neutral75},
    {u8"70", Lamp::Color::Neutral76},
    {u8"70", Lamp::Color::Neutral77},
    {u8"70", Lamp::Color::Neutral78},
    {u8"70", Lamp::Color::Neutral79},
    {u8"80", Lamp::Color::Neutral80},
    {u8"81", Lamp::Color::Neutral81},
    {u8"82", Lamp::Color::Neutral82},
    {u8"83", Lamp::Color::Neutral83},
    {u8"84", Lamp::Color::Neutral84},
    {u8"85", Lamp::Color::Neutral85},
    {u8"86", Lamp::Color::Neutral86},
    {u8"87", Lamp::Color::Neutral87},
    {u8"88", Lamp::Color::Neutral88},
    {u8"89", Lamp::Color::Neutral89},
    {u8"90", Lamp::Color::Neutral90},
    {u8"91", Lamp::Color::Neutral91},
    {u8"92", Lamp::Color::Neutral92},
    {u8"93", Lamp::Color::Neutral93},
    {u8"94", Lamp::Color::Neutral94},
    {u8"95", Lamp::Color::Neutral95},
    {u8"96", Lamp::Color::Neutral96},
    {u8"97", Lamp::Color::Neutral97},
    {u8"98", Lamp::Color::Neutral98},
    {u8"99", Lamp::Color::Neutral99},
    {u8"100", Lamp::Color::Neutral100},
};

auto const neutralVariantColors = std::vector<Swatch> {
    {u8"0", Lamp::Color::NeutralVariant0},
    {u8"1", Lamp::Color::NeutralVariant1},
    {u8"2", Lamp::Color::NeutralVariant2},
    {u8"3", Lamp::Color::NeutralVariant3},
    {u8"4", Lamp::Color::NeutralVariant4},
    {u8"5", Lamp::Color::NeutralVariant5},
    {u8"6", Lamp::Color::NeutralVariant6},
    {u8"7", Lamp::Color::NeutralVariant7},
    {u8"8", Lamp::Color::NeutralVariant8},
    {u8"9", Lamp::Color::NeutralVariant9},
    {u8"10", Lamp::Color::NeutralVariant10},
    {u8"11", Lamp::Color::NeutralVariant11},
    {u8"12", Lamp::Color::NeutralVariant12},
    {u8"13", Lamp::Color::NeutralVariant13},
    {u8"14", Lamp::Color::NeutralVariant14},
    {u8"15", Lamp::Color::NeutralVariant15},
    {u8"16", Lamp::Color::NeutralVariant16},
    {u8"17", Lamp::Color::NeutralVariant17},
    {u8"18", Lamp::Color::NeutralVariant18},
    {u8"19", Lamp::Color::NeutralVariant19},
    {u8"20", Lamp::Color::NeutralVariant20},
    {u8"20", Lamp::Color::NeutralVariant21},
    {u8"20", Lamp::Color::NeutralVariant22},
    {u8"20", Lamp::Color::NeutralVariant23},
    {u8"20", Lamp::Color::NeutralVariant24},
    {u8"25", Lamp::Color::NeutralVariant25},
    {u8"25", Lamp::Color::NeutralVariant26},
    {u8"25", Lamp::Color::NeutralVariant27},
    {u8"25", Lamp::Color::NeutralVariant28},
    {u8"25", Lamp::Color::NeutralVariant29},
    {u8"30", Lamp::Color::NeutralVariant30},
    {u8"40", Lamp::Color::NeutralVariant40},
    {u8"50", Lamp::Color::NeutralVariant50},
    {u8"60", Lamp::Color::NeutralVariant60},
    {u8"70", Lamp::Color::NeutralVariant70},
    {u8"70", Lamp::Color::NeutralVariant71},
    {u8"70", Lamp::Color::NeutralVariant72},
    {u8"70", Lamp::Color::NeutralVariant73},
    {u8"70", Lamp::Color::NeutralVariant74},
    {u8"70", Lamp::Color::NeutralVariant75},
    {u8"70", Lamp::Color::NeutralVariant76},
    {u8"70", Lamp::Color::NeutralVariant77},
    {u8"70", Lamp::Color::NeutralVariant78},
    {u8"70", Lamp::Color::NeutralVariant79},
    {u8"80", Lamp::Color::NeutralVariant80},
    {u8"81", Lamp::Color::NeutralVariant81},
    {u8"82", Lamp::Color::NeutralVariant82},
    {u8"83", Lamp::Color::NeutralVariant83},
    {u8"84", Lamp::Color::NeutralVariant84},
    {u8"85", Lamp::Color::NeutralVariant85},
    {u8"86", Lamp::Color::NeutralVariant86},
    {u8"87", Lamp::Color::NeutralVariant87},
    {u8"88", Lamp::Color::NeutralVariant88},
    {u8"89", Lamp::Color::NeutralVariant89},
    {u8"90", Lamp::Color::NeutralVariant90},
    {u8"91", Lamp::Color::NeutralVariant91},
    {u8"92", Lamp::Color::NeutralVariant92},
    {u8"93", Lamp::Color::NeutralVariant93},
    {u8"94", Lamp::Color::NeutralVariant94},
    {u8"95", Lamp::Color::NeutralVariant95},
    {u8"96", Lamp::Color::NeutralVariant96},
    {u8"97", Lamp::Color::NeutralVariant97},
    {u8"98", Lamp::Color::NeutralVariant98},
    {u8"99", Lamp::Color::NeutralVariant99},
    {u8"100", Lamp::Color::NeutralVariant100},
};

auto MakePaletteView(String const name, std::vector<Swatch> const& swatches) -> Shared<View>
{
    auto column = FlexLayout::Make();
    column->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Start);

    auto text = Lamp::TextView::MakeWithText(name);
    AttributeNode::SetReference<&Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeLabelMedium);
    column->AddChild(text);

    auto const flex = FlexLayout::Make();
    flex->SetDirection(FlexLayoutDirection::Row);
    flex->SetMainAxisSize(FlexLayoutMainAxisSize::Max);

    for (auto const& swatch : swatches)
    {
       // auto bgn = std::chrono::steady_clock::now();
        auto const label = Lamp::TextView::MakeWithText(swatch.name);
        //auto end = std::chrono::steady_clock::now();
        //FW_DEBUG_LOG_INFO("Swatch {} initialized in {} ms", swatch.name.ToStdString(), std::chrono::duration_cast<std::chrono::milliseconds>(end - bgn).count());
        auto const align = AlignView::MakeWithContent(label);
        auto const sized = SizedView::MakeWithContent(align);
        sized->SetHeight(Lamp::Style::Size400);
        auto const container = BoxView::MakeWithContent(sized);
        container->SetBackgroundColor(swatch.color);
        FlexLayout::SetChildGrowFactor(container, 1.0);
        FlexLayout::SetChildShrinkFactor(container, 1.0);
        flex->AddChild(container);
    }
    column->AddChild(flex);
    return column;
}
}

auto ColorSwatchView::Make() -> Shared<ColorSwatchView>
{
    return View::MakeDerived<ColorSwatchView>();
}

ColorSwatchView::ColorSwatchView(PassKey<View> key)
  : View(key)
{
}

auto ColorSwatchView::Initialize() -> void
{
    auto const column = FlexLayout::Make();
    column->SetDirection(FlexLayoutDirection::Column);
    column->SetCrossAxisSize(FlexLayoutCrossAxisSize::Max);

    auto bgn = std::chrono::steady_clock::now();
    column->AddChild(MakePaletteView(u8"Primary Colors", primaryColors));
    column->AddChild(MakePaletteView(u8"Secondary Colors", secondaryColors));
    column->AddChild(MakePaletteView(u8"Tertiary Colors", tertiaryColors));
    column->AddChild(MakePaletteView(u8"Error Colors", errorColors));
    column->AddChild(MakePaletteView(u8"Neutral Colors", neutralColors));
    column->AddChild(MakePaletteView(u8"Neutral Variant Colors", neutralVariantColors));
    auto end = std::chrono::steady_clock::now();

    FW_DEBUG_LOG_INFO("ColorSwatchView initialized in {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(end - bgn).count());

    bgn = std::chrono::steady_clock::now();
    AddChildBack(column);
    end = std::chrono::steady_clock::now();
    FW_DEBUG_LOG_INFO("ColorSwatchView added to view hierarchy in {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(end - bgn).count());
}
}

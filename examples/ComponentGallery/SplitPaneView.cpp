#include "SplitPaneView.hpp"
#include "ColorPageView.hpp"
#include "HomePageView.hpp"
#include "TypographyPageView.hpp"
#include "ButtonPageView.hpp"
#include "TextEditPageView.hpp"

#include <Futurewalker.Application.FlexLayout.hpp>
#include <Futurewalker.Application.ContainerView.hpp>
#include <Futurewalker.Application.PaddingView.hpp>
#include <Futurewalker.Application.SizedView.hpp>
#include <Futurewalker.Application.ThemeView.hpp>
#include <Futurewalker.Application.BoxView.hpp>
#include <Futurewalker.Application.ClipView.hpp>
#include "Futurewalker.Application.Icon.hpp" 

#include <Futurewalker.Component.Lamp.Style.hpp> 
#include <Futurewalker.Component.Lamp.TextButton.hpp>
#include <Futurewalker.Component.Lamp.TextButtonTheme.hpp>
#include <Futurewalker.Component.Lamp.DisclosureTextButton.hpp>
#include <Futurewalker.Component.Lamp.TextView.hpp>

#include <Futurewalker.Resource.ResourceManager.hpp>
#include <Futurewalker.Resource.Resource.hpp>

#include <Futurewalker.Attribute.StaticAttribute.hpp>

#include <Futurewalker.Graphics.SvgDocument.hpp>

#include "Resource/ComponentGallery.hpp"
#include "Resource/ComponentGallery.Icon.hpp"

namespace Futurewalker::ComponentGallery
{
namespace
{
auto const ViewParameterKeyContentView = UniqueId();

auto SetPageView(Shared<View> const& button, Shared<View> const& pageView) -> void
{
    PropertyStore::SetValue(*button, ViewParameterKeyContentView, pageView);
}

auto GetPageView(Shared<View> const& button) -> Shared<View>
{
    return PropertyStore::GetValue<Shared<View>>(*button, ViewParameterKeyContentView).GetValueOrDefault();
}
}

auto SplitPaneView::Make() -> Shared<SplitPaneView>
{
    return View::MakeDerived<SplitPaneView>();
}

SplitPaneView::SplitPaneView(PassKey<View> key)
  : View(key)
{
}

auto SplitPaneView::Initialize() -> void
{
    auto resource = ResourceManager::GetResource(M::ComponentGallery);

    auto loadIcon = [&](auto fileId) {
        auto file = resource->LoadFile(fileId);
        auto svg = Graphics::SvgDocument::LoadFromStream(file);
        return Icon::MakeFromSvgDocument(std::move(svg));
    };

    auto buttonColumn = FlexLayout::Make();
    buttonColumn->SetDirection(FlexLayoutDirection::Column);
    buttonColumn->SetMainAxisSize(FlexLayoutMainAxisSize::Max);
    buttonColumn->SetMainAxisAlignment(FlexLayoutMainAxisAlignment::Start);
    buttonColumn->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Stretch);
    buttonColumn->SetCrossAxisSize(FlexLayoutCrossAxisSize::Min);

    AttributeNode::SetReference<Lamp::TextButtonStyle::TextFontSize>(*buttonColumn, Lamp::Style::FontSizeLabelLarge);

    // Home
    auto homeView = HomePageView::Make();
    {
        auto homeButton = Lamp::TextButton::MakeWithTextAndIcon(u8"Home", loadIcon(R::ComponentGallery::Icon::House));
        FlexLayout::SetChildMargin(homeButton, EdgeInsets(0, 0, 0, 4));
        SetPageView(homeButton, homeView);
        SetButtonSelected(homeButton, true);
        buttonColumn->AddChild(homeButton);
    }
    // Foundation
    {
        auto disclosureButton = Lamp::DisclosureTextButton::MakeWithTextAndIcon(u8"Foundation", loadIcon(R::ComponentGallery::Icon::LibraryBig));
        auto column = FlexLayout::Make();
        column->SetDirection(FlexLayoutDirection::Column);
        column->SetMainAxisSize(FlexLayoutMainAxisSize::Min);
        column->SetCrossAxisSize(FlexLayoutCrossAxisSize::Max);
        column->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Stretch);
        {
            auto button = Lamp::TextButton::MakeWithTextAndIcon(u8"Color", Icon::MakeBlank());
            FlexLayout::SetChildMargin(button, EdgeInsets(0, 4, 0, 4));
            SetPageView(button, ColorPageView::Make());
            column->AddChild(button);
        }
        {
            auto button = Lamp::TextButton::MakeWithTextAndIcon(u8"Typography", Icon::MakeBlank());
            FlexLayout::SetChildMargin(button, EdgeInsets(0, 0, 0, 4));
            SetPageView(button, TypographyPageView::Make());
            column->AddChild(button);
        }
        disclosureButton->SetContent(column);
        buttonColumn->AddChild(disclosureButton);
    }
    // Components
    {
        auto disclosureButton = Lamp::DisclosureTextButton::MakeWithTextAndIcon(u8"Components", loadIcon(R::ComponentGallery::Icon::LayoutGrid));
        auto column = FlexLayout::Make();
        column->SetDirection(FlexLayoutDirection::Column);
        column->SetMainAxisSize(FlexLayoutMainAxisSize::Min);
        column->SetCrossAxisSize(FlexLayoutCrossAxisSize::Max);
        column->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Stretch);
        {
            auto button = Lamp::TextButton::MakeWithTextAndIcon(u8"Buttons", Icon::MakeBlank());
            FlexLayout::SetChildMargin(button, EdgeInsets(0, 4, 0, 4));
            SetPageView(button, ButtonPageView::Make());
            column->AddChild(button);
        }
        {
            auto button = Lamp::TextButton::MakeWithTextAndIcon(u8"Text fields", Icon::MakeBlank());
            FlexLayout::SetChildMargin(button, EdgeInsets(0, 0, 0, 4));
            SetPageView(button, TextEditPageView::Make());
            column->AddChild(button);
        }
        disclosureButton->SetContent(column);
        buttonColumn->AddChild(disclosureButton);
    }
    auto buttonColumnSize = SizedView::MakeWithContent(200, Dp::Infinity(), buttonColumn);
    auto buttonColumnTheme = ThemeView::MakeWithContent(Lamp::TextButtonTheme::Make(Lamp::TextButtonTheme::Type::Text), buttonColumnSize);
    AttributeNode::SetReference<Lamp::TextButtonStyle::CornerRadius>(*buttonColumnTheme, Lamp::Style::CornerRadiusMedium);
    auto buttonColumnBox = BoxView::MakeWithContent(buttonColumnTheme);
    buttonColumnBox->SetCornerRadius(Lamp::Style::CornerRadiusMedium);
    auto buttonColumnPadding = PaddingView::MakeWithPaddingAndContent(EdgeInsets(8, 8, 0, 8), buttonColumnBox);

    auto contentArea = BoxView::MakeWithContent(homeView);
    contentArea->SetBackgroundColor(Lamp::Style::ColorSurface);
    auto contentAreaClip = ClipView::MakeWithContent(contentArea);
    FW_LOCAL_STATIC_ATTRIBUTE_DEFAULT_FUNCTION(CornerRadius, AttributeCornerRadius, [](CornerRadius const& radius) { return CornerRadius(radius.topLeading, 0, radius.bottomLeading, 0); }, Lamp::Style::CornerRadiusMedium);
    contentAreaClip->SetCornerRadius(AttributeCornerRadius);
    auto contentAreaPadding = PaddingView::MakeWithPaddingAndContent(EdgeInsets(8, 8, 0, 0), contentAreaClip);

    auto leftPane = ContainerView::MakeWithContent(buttonColumnPadding);
    auto rightPane = ContainerView::MakeWithContent(contentAreaPadding);
    FlexLayout::SetChildGrowFactor(rightPane, 1.0);
    FlexLayout::SetChildShrinkFactor(rightPane, 1.0);
    FlexLayout::SetChildMainAxisFlexibility(rightPane, FlexLayoutMainAxisFlexibility::Expand);

    auto row = FlexLayout::Make();
    row->SetDirection(FlexLayoutDirection::Row);
    row->SetMainAxisSize(FlexLayoutMainAxisSize::Max);
    row->SetMainAxisAlignment(FlexLayoutMainAxisAlignment::Start);
    row->SetCrossAxisSize(FlexLayoutCrossAxisSize::Max);
    row->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Stretch);
    row->AddChild(leftPane);
    row->AddChild(rightPane);

    _buttonColumn = buttonColumn;
    _rightPaneContentArea = contentArea;

    AddChildFront(row);

    EventReceiver::Connect(*buttonColumn, *this, &SplitPaneView::ReceiveButtonEvent);
}

auto SplitPaneView::ReceiveButtonEvent(Event<>& event) -> Bool
{
    if (event.Is<Lamp::TextButtonEvent::Press>())
    {
        auto const sender = event.As<Lamp::TextButtonEvent>()->GetSender();
        if (auto const pageView = GetPageView(sender))
        {
            if (auto currentSelected = _selected.Lock())
            {
                SetButtonSelected(currentSelected, false);
            }

            _selected = sender;

            if (sender)
            {
                SetButtonSelected(sender, true);
                _rightPaneContentArea->SetContent(pageView);
            }
            else
            {
                _rightPaneContentArea->SetContent(nullptr);
            }
        }
        return true;
    }
    return false;
}

auto SplitPaneView::SetButtonSelected(Shared<View> const& button, Bool const active) -> void
{
    if (button)
    {
        auto const theme = Lamp::TextButtonTheme::Make(Lamp::TextButtonTheme::Type::Tonal);
        if (active)
        {
            theme->Apply(button->GetAttributeNode());
        }
        else
        {
            theme->Clear(button->GetAttributeNode());
        }
    }
}
}

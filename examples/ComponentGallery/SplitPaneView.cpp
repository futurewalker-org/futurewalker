#include "SplitPaneView.hpp"

#include <Futurewalker.Application.FlexLayout.hpp>
#include <Futurewalker.Application.ContainerView.hpp>
#include <Futurewalker.Application.PaddingView.hpp>
#include <Futurewalker.Application.SizedView.hpp>
#include <Futurewalker.Application.ThemeView.hpp>
#include <Futurewalker.Application.BoxView.hpp>
#include <Futurewalker.Application.ClipView.hpp>

#include <Futurewalker.Component.Lamp.Style.hpp> 
#include <Futurewalker.Component.Lamp.TextButton.hpp>
#include <Futurewalker.Component.Lamp.TextButtonTheme.hpp>


namespace Futurewalker::ComponentGallery
{
auto SplitPaneView::Make() -> Shared<SplitPaneView>
{
    return View::MakeDerived<SplitPaneView>();
}

SplitPaneView::SplitPaneView(PassKey<View> key)
  : View(key)
{
}

auto SplitPaneView::SetPages(std::vector<PageInfo> const& pages) -> void
{
    _pages = pages;
    _buttonColumn->RemoveChildren();
    _rightPaneContentArea->SetContent(nullptr);
    _index = -1;

    for (auto const& page : _pages)
    {
        auto button = Lamp::TextButton::MakeWithText(page.title);
        _buttonColumn->AddChild(button);
    }
    SetActivePage(0);
}

auto SplitPaneView::Initialize() -> void
{
    auto buttonColumn = FlexLayout::Make();
    buttonColumn->SetDirection(FlexLayoutDirection::Column);
    buttonColumn->SetMainAxisSize(FlexLayoutMainAxisSize::Max);
    buttonColumn->SetMainAxisAlignment(FlexLayoutMainAxisAlignment::Start);
    buttonColumn->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Stretch);
    buttonColumn->SetCrossAxisSize(FlexLayoutCrossAxisSize::Min);
    auto buttonColumnSize = SizedView::MakeWithContent(150, Dp::Infinity(), buttonColumn);
    auto buttonColumnTheme = ThemeView::MakeWithContent(Lamp::TextButtonTheme::Make(Lamp::TextButtonTheme::Type::Text), buttonColumnSize);
    AttributeNode::SetReference<&Lamp::TextButtonStyle::CornerRadius>(*buttonColumnTheme, Lamp::Style::CornerRadiusMedium);
    auto buttonColumnBox = BoxView::MakeWithContent(buttonColumnTheme);
    buttonColumnBox->SetBackgroundColor(Lamp::Style::ColorSurfaceContainerLow);
    buttonColumnBox->SetCornerRadius(Lamp::Style::CornerRadiusMedium);
    auto buttonColumnPadding = PaddingView::MakeWithPaddingAndContent(EdgeInsets(5, 5, 0, 5), buttonColumnBox);

    auto contentArea = BoxView::Make();
    contentArea->SetBackgroundColor(Lamp::Style::ColorSurfaceContainerLow);
    contentArea->SetCornerRadius(Lamp::Style::CornerRadiusMedium);
    auto contentAreaClip = ClipView::MakeWithContent(contentArea);
    contentAreaClip->SetCornerRadius(Lamp::Style::CornerRadiusMedium);
    auto contentAreaPadding = PaddingView::MakeWithPaddingAndContent(EdgeInsets::MakeUniform(5), contentAreaClip);

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

auto SplitPaneView::SetActivePage(SInt64 const index) -> void
{
    if (0 <= index && index < std::ssize(_pages))
    {
        if (_index != index)
        {
            auto const oldIndex = std::exchange(_index, index);
            auto const& page = _pages[static_cast<size_t>(index)];
            _rightPaneContentArea->SetContent(page.content);
            SetButtonActive(oldIndex, false);
            SetButtonActive(index, true);
        }
    }
}

auto SplitPaneView::ReceiveButtonEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<Lamp::TextButtonEvent::Press>())
    {
        auto const sender = event.As<Lamp::TextButtonEvent>()->GetSender();
        auto index = _buttonColumn->GetAddedChildIndex(sender);
        if (index)
        {
            auto viewEventParameter = Event<SplitPaneViewEvent>::Make();
            viewEventParameter->SetIndex(*index);
            auto viewEvent = Event<>(std::move(viewEventParameter));
            co_await SendEvent(viewEvent);
        }
        co_return true;
    }
    co_return false;
}

auto SplitPaneView::SetButtonActive(SInt64 const index, Bool const active) -> void
{
    if (0 <= index && index < std::ssize(_pages))
    {
        auto const button = _buttonColumn->GetAddedChildAt(index);
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

// SPDX-License-Identifier: MPL-2.0 OR CC0-1.0
#include "Gallery.Pages.IconButton.hpp"

#include <Futurewalker.Application.FlexLayout.hpp>
#include <Futurewalker.Application.PaddingView.hpp>
#include <Futurewalker.Application.BoxView.hpp>
#include <Futurewalker.Application.ClipView.hpp>
#include <Futurewalker.Application.ScrollView.hpp>
#include <Futurewalker.Application.AlignView.hpp>
#include <Futurewalker.Application.ConstrainedView.hpp>

#include <Futurewalker.Resource.ResourceManager.hpp>
#include <Futurewalker.Resource.Resource.hpp>

#include <Futurewalker.Graphics.SvgDocument.hpp>

#include <Futurewalker.Component.Lamp.Style.hpp>
#include <Futurewalker.Component.Lamp.TextView.hpp>
#include <Futurewalker.Component.Lamp.IconButton.hpp>
#include <Futurewalker.Component.Lamp.DropdownMenuButton.hpp>

#include <Futurewalker.Core.StringFunction.hpp>

#include "Resource/Gallery.hpp"
#include "Resource/Gallery.Icon.hpp"
#include "Resource/Gallery.String.IconButtonPage.hpp"

#include <array>
#include <initializer_list>
#include <utility>

namespace Futurewalker::Gallery
{
namespace
{
namespace StringId = R::Gallery::String::IconButtonPage;

auto MakeSectionContainer(Shared<View> const& content) -> Shared<BoxView>
{
    auto padding = PaddingView::MakeWithPaddingAndContent(Lamp::Style::PaddingMedium, content);
    auto clip = ClipView::MakeWithContent(padding);
    clip->SetCornerRadius(Lamp::Style::CornerRadiusSmall);
    auto container = BoxView::MakeWithContent(clip);
    container->SetBackgroundColor(Lamp::Style::ColorSurfaceContainer);
    container->SetBorderColor(Lamp::Style::ColorOutlineVariant);
    container->SetCornerRadius(Lamp::Style::CornerRadiusSmall);
    container->SetBorderWidth(1);
    return container;
}

auto MakeVariants(Shared<Resource> const& resource, std::array<Icon, 4> const& icons) -> Shared<View>
{
    auto innerColumn = FlexLayout::Make();
    innerColumn->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Start);

    auto makeRow = [&](UInt32 const labelId, Bool const enabled) {
        auto label = Lamp::TextView::MakeWithText(resource->LoadString(labelId));
        AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*label, Lamp::Style::FontSizeLabelMedium);
        innerColumn->AddChild(label);

        auto row = FlexLayout::Make();
        row->SetDirection(FlexLayoutDirection::Row);
        row->SetMainAxisSize(FlexLayoutMainAxisSize::Min);
        row->SetCrossAxisSize(FlexLayoutCrossAxisSize::Min);
        for (auto const& icon : icons)
        {
            auto button = Lamp::IconButton::MakeWithIcon(icon);
            auto buttonPadding = PaddingView::MakeWithPaddingAndContent(EdgeInsets::MakeUniform(5), button);
            buttonPadding->SetEnabled(enabled);
            row->AddChild(buttonPadding);
        }
        innerColumn->AddChild(row);
    };
    makeRow(StringId::EnabledButtons, true);
    makeRow(StringId::DisabledButtons, false);
    return MakeSectionContainer(innerColumn);
}

auto MakeLabeledDropdown(Shared<Resource> const& resource, UInt32 const labelId, std::initializer_list<UInt32> const& itemIds, SInt64 const currentIndex)
    -> std::pair<Shared<View>, Shared<Lamp::DropdownMenuButton>>
{
    auto column = FlexLayout::Make();
    column->SetDirection(FlexLayoutDirection::Column);
    column->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Start);
    {
        auto label = Lamp::TextView::MakeWithText(resource->LoadString(labelId));
        AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*label, Lamp::Style::FontSizeLabelMedium);
        column->AddChild(label);
    }
    auto dropdown = Lamp::DropdownMenuButton::Make();
    for (auto const itemId : itemIds)
    {
        dropdown->AddItem({resource->LoadString(itemId)});
    }
    dropdown->SetCurrentIndex(currentIndex);
    auto constrained = ConstrainedView::MakeWithContent(dropdown);
    constrained->SetWidthConstraints(AxisConstraints::MakeUnbounded(160));
    FlexLayout::SetChildMargin(constrained, EdgeInsets(0, 4, 0, 0));
    column->AddChild(constrained);
    return {column, dropdown};
}
}

auto IconButtonPage::Make() -> Shared<IconButtonPage>
{
    return View::MakeDerived<IconButtonPage>();
}

IconButtonPage::IconButtonPage(PassKey<View> key)
  : View(key)
{
}

auto IconButtonPage::Initialize() -> void
{
    _resource = ResourceManager::GetResource(M::Gallery);

    auto loadIcon = [&](auto fileId) {
        auto file = _resource->LoadFile(fileId);
        auto svg = Graphics::SvgDocument::LoadFromStream(file);
        return Icon::MakeFromSvgDocument(std::move(svg));
    };
    _icons = {loadIcon(R::Gallery::Icon::House), loadIcon(R::Gallery::Icon::LayoutGrid), loadIcon(R::Gallery::Icon::AppWindow), loadIcon(R::Gallery::Icon::SunMoon)};

    auto column = FlexLayout::Make();
    column->SetDirection(FlexLayoutDirection::Column);
    column->SetMainAxisSize(FlexLayoutMainAxisSize::Max);
    column->SetMainAxisAlignment(FlexLayoutMainAxisAlignment::Start);
    column->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Stretch);
    column->SetCrossAxisSize(FlexLayoutCrossAxisSize::Max);
    {
        auto text = Lamp::TextView::MakeWithText(_resource->LoadString(StringId::Title));
        AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeHeadlineMedium);
        column->AddChild(text);
    }
    {
        auto text = Lamp::TextView::MakeWithText(_resource->LoadString(StringId::Description));
        AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeBodyMedium);
        AttributeNode::SetReference<Lamp::TextViewStyle::Color>(*text, Lamp::Style::ColorOnSurfaceVariant);
        FlexLayout::SetChildMargin(text, EdgeInsets(0, 10, 0, 0));
        column->AddChild(text);
    }
    {
        auto text = Lamp::TextView::MakeWithText(_resource->LoadString(StringId::VariantsTitle));
        AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeTitleLarge);
        FlexLayout::SetChildMargin(text, EdgeInsets(0, 20, 0, 0));
        column->AddChild(text);
    }
    {
        auto variantsContainer = MakeVariants(_resource, _icons);
        FlexLayout::SetChildMargin(variantsContainer, EdgeInsets(0, 10, 0, 0));
        column->AddChild(variantsContainer);
    }
    {
        auto text = Lamp::TextView::MakeWithText(_resource->LoadString(StringId::PreviewTitle));
        AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeTitleLarge);
        FlexLayout::SetChildMargin(text, EdgeInsets(0, 20, 0, 0));
        column->AddChild(text);
    }
    {
        auto innerColumn = FlexLayout::Make();
        innerColumn->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Stretch);
        {
            auto row = FlexLayout::Make();
            row->SetDirection(FlexLayoutDirection::Row);
            row->SetMainAxisSize(FlexLayoutMainAxisSize::Min);
            row->SetCrossAxisSize(FlexLayoutCrossAxisSize::Min);
            row->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Start);
            {
                auto const control = MakeLabeledDropdown(_resource, StringId::IconLabel,
                                                         {StringId::IconHouse, StringId::IconLayoutGrid, StringId::IconAppWindow, StringId::IconSunMoon}, 0);
                _iconDropdown = control.second;
                row->AddChild(control.first);
            }
            {
                auto const control = MakeLabeledDropdown(_resource, StringId::SizeLabel, {StringId::SizeSmall, StringId::SizeMedium, StringId::SizeLarge}, 1);
                _sizeDropdown = control.second;
                FlexLayout::SetChildMargin(control.first, EdgeInsets(20, 0, 0, 0));
                row->AddChild(control.first);
            }
            {
                auto const control = MakeLabeledDropdown(_resource, StringId::StateLabel, {StringId::StateEnabled, StringId::StateDisabled}, 0);
                _stateDropdown = control.second;
                FlexLayout::SetChildMargin(control.first, EdgeInsets(20, 0, 0, 0));
                row->AddChild(control.first);
            }
            innerColumn->AddChild(row);
        }
        {
            _previewAlign = AlignView::Make();
            auto constrained = ConstrainedView::MakeWithContent(_previewAlign);
            constrained->SetHeightConstraints(AxisConstraints::MakeUnbounded(120));
            FlexLayout::SetChildMargin(constrained, EdgeInsets(0, 10, 0, 0));
            innerColumn->AddChild(constrained);
        }
        {
            _pressCountText = Lamp::TextView::MakeWithText(StringFunction::Format(_resource->LoadString(StringId::PressCount), _pressCount));
            AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*_pressCountText, Lamp::Style::FontSizeBodyMedium);
            AttributeNode::SetReference<Lamp::TextViewStyle::Color>(*_pressCountText, Lamp::Style::ColorOnSurfaceVariant);
            FlexLayout::SetChildMargin(_pressCountText, EdgeInsets(0, 10, 0, 0));
            innerColumn->AddChild(_pressCountText);
        }
        auto previewContainer = MakeSectionContainer(innerColumn);
        FlexLayout::SetChildMargin(previewContainer, EdgeInsets(0, 10, 0, 0));
        column->AddChild(previewContainer);
    }
    auto columnPadding = PaddingView::MakeWithPaddingAndContent(Lamp::Style::PaddingLarge, column);
    auto scroll = ScrollView::MakeWithContent(columnPadding);
    scroll->SetDirection(ScrollViewDirection::Vertical);
    AddChildFront(scroll);

    UpdatePreview();

    EventReceiver::Connect(*this, *this, &IconButtonPage::ReceiveEvent);
}

auto IconButtonPage::ReceiveEvent(Event<>& event) -> Bool
{
    if (event.Is<ViewEvent::Notify>())
    {
        auto const& notify = event.As<ViewEvent::Notify>();
        auto const sender = notify->GetSender();
        if (event.Is<Lamp::DropdownMenuButtonEvent::CurrentItemChanged>())
        {
            auto const& changed = event.As<Lamp::DropdownMenuButtonEvent::CurrentItemChanged>();
            if (sender == _iconDropdown)
            {
                _iconIndex = changed->GetCurrentIndex();
                UpdatePreview();
                return true;
            }
            if (sender == _sizeDropdown)
            {
                _sizeIndex = changed->GetCurrentIndex();
                UpdatePreview();
                return true;
            }
            if (sender == _stateDropdown)
            {
                _previewEnabled = changed->GetCurrentIndex() == 0;
                UpdatePreview();
                return true;
            }
        }
        if (event.Is<Lamp::IconButtonEvent::Press>() && sender == _previewButton)
        {
            ++_pressCount;
            _pressCountText->SetText(StringFunction::Format(_resource->LoadString(StringId::PressCount), _pressCount));
            return true;
        }
    }
    return false;
}

auto IconButtonPage::UpdatePreview() -> void
{
    auto button = Lamp::IconButton::MakeWithIcon(_icons[static_cast<std::size_t>(_iconIndex)]);
    if (_sizeIndex == 0)
    {
        AttributeNode::SetReference<Lamp::IconButtonStyle::IconSize>(*button, Lamp::Style::IconSizeSmall);
    }
    else if (_sizeIndex == 2)
    {
        AttributeNode::SetReference<Lamp::IconButtonStyle::IconSize>(*button, Lamp::Style::IconSizeLarge);
    }
    else
    {
        AttributeNode::SetReference<Lamp::IconButtonStyle::IconSize>(*button, Lamp::Style::IconSizeMedium);
    }
    button->SetEnabled(_previewEnabled);
    _previewButton = button;
    _previewAlign->SetContent(button);
}
}

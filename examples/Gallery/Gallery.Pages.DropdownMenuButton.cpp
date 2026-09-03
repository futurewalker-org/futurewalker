// SPDX-License-Identifier: MPL-2.0 OR CC0-1.0
#include "Gallery.Pages.DropdownMenuButton.hpp"

#include <Futurewalker.Application.FlexLayout.hpp>
#include <Futurewalker.Application.PaddingView.hpp>
#include <Futurewalker.Application.BoxView.hpp>
#include <Futurewalker.Application.ClipView.hpp>
#include <Futurewalker.Application.ScrollView.hpp>
#include <Futurewalker.Application.AlignView.hpp>
#include <Futurewalker.Application.ConstrainedView.hpp>

#include <Futurewalker.Resource.ResourceManager.hpp>
#include <Futurewalker.Resource.Resource.hpp>

#include <Futurewalker.Component.Lamp.Style.hpp>
#include <Futurewalker.Component.Lamp.TextView.hpp>
#include <Futurewalker.Component.Lamp.DropdownMenuButton.hpp>

#include <Futurewalker.Core.StringFunction.hpp>

#include "Resource/Gallery.hpp"
#include "Resource/Gallery.String.DropdownMenuButtonPage.hpp"

#include <array>
#include <initializer_list>
#include <utility>

namespace Futurewalker::Gallery
{
namespace
{
namespace StringId = R::Gallery::String::DropdownMenuButtonPage;

auto const ItemIds = std::array {
    StringId::ItemApple,
    StringId::ItemBanana,
    StringId::ItemCherry,
    StringId::ItemOrange,
};

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

auto MakeDropdown(Shared<Resource> const& resource, SInt64 const currentIndex) -> Shared<Lamp::DropdownMenuButton>
{
    auto dropdown = Lamp::DropdownMenuButton::Make();
    for (auto const itemId : ItemIds)
    {
        dropdown->AddItem({resource->LoadString(itemId)});
    }
    dropdown->SetPlaceholderText(resource->LoadString(StringId::Placeholder));
    dropdown->SetCurrentIndex(currentIndex);
    return dropdown;
}

auto MakeVariants(Shared<Resource> const& resource) -> Shared<View>
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
        for (auto const currentIndex : {-1, 0})
        {
            auto dropdown = MakeDropdown(resource, currentIndex);
            auto constrained = ConstrainedView::MakeWithContent(dropdown);
            constrained->SetWidthConstraints(AxisConstraints::MakeUnbounded(180));
            auto dropdownPadding = PaddingView::MakeWithPaddingAndContent(EdgeInsets::MakeUniform(5), constrained);
            dropdownPadding->SetEnabled(enabled);
            row->AddChild(dropdownPadding);
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

auto DropdownMenuButtonPage::Make() -> Shared<DropdownMenuButtonPage>
{
    return View::MakeDerived<DropdownMenuButtonPage>();
}

DropdownMenuButtonPage::DropdownMenuButtonPage(PassKey<View> key)
  : View(key)
{
}

auto DropdownMenuButtonPage::Initialize() -> void
{
    _resource = ResourceManager::GetResource(M::Gallery);

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
        auto variantsContainer = MakeVariants(_resource);
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
                auto const control = MakeLabeledDropdown(_resource, StringId::StateLabel, {StringId::StateEnabled, StringId::StateDisabled}, 0);
                _stateDropdown = control.second;
                row->AddChild(control.first);
            }
            {
                auto const control = MakeLabeledDropdown(_resource, StringId::InitialSelectionLabel, {StringId::InitialSelectionPlaceholder, StringId::InitialSelectionFirst}, 0);
                _selectionDropdown = control.second;
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
            _selectionText = Lamp::TextView::MakeWithText(StringFunction::Format(_resource->LoadString(StringId::SelectedItem), _resource->LoadString(StringId::SelectedNone)));
            AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*_selectionText, Lamp::Style::FontSizeBodyMedium);
            AttributeNode::SetReference<Lamp::TextViewStyle::Color>(*_selectionText, Lamp::Style::ColorOnSurfaceVariant);
            FlexLayout::SetChildMargin(_selectionText, EdgeInsets(0, 10, 0, 0));
            innerColumn->AddChild(_selectionText);
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

    EventReceiver::Connect(*this, *this, &DropdownMenuButtonPage::ReceiveEvent);
}

auto DropdownMenuButtonPage::ReceiveEvent(Event<>& event) -> Bool
{
    if (event.Is<ViewEvent::Notify>())
    {
        auto const& notify = event.As<ViewEvent::Notify>();
        auto const sender = notify->GetSender();
        if (event.Is<Lamp::DropdownMenuButtonEvent::CurrentItemChanged>())
        {
            auto const& changed = event.As<Lamp::DropdownMenuButtonEvent::CurrentItemChanged>();
            if (sender == _stateDropdown)
            {
                _previewEnabled = changed->GetCurrentIndex() == 0;
                UpdatePreview();
                return true;
            }
            if (sender == _selectionDropdown)
            {
                _startWithPlaceholder = changed->GetCurrentIndex() == 0;
                UpdatePreview();
                return true;
            }
            if (sender == _previewDropdown)
            {
                auto const index = changed->GetCurrentIndex();
                auto name = _resource->LoadString(StringId::SelectedNone);
                if (0 <= index && index < static_cast<SInt64>(ItemIds.size()))
                {
                    name = _resource->LoadString(ItemIds[static_cast<std::size_t>(index)]);
                }
                _selectionText->SetText(StringFunction::Format(_resource->LoadString(StringId::SelectedItem), name));
                return true;
            }
        }
    }
    return false;
}

auto DropdownMenuButtonPage::UpdatePreview() -> void
{
    auto dropdown = MakeDropdown(_resource, _startWithPlaceholder ? -1 : 0);
    dropdown->SetEnabled(_previewEnabled);
    auto constrained = ConstrainedView::MakeWithContent(dropdown);
    constrained->SetWidthConstraints(AxisConstraints::MakeUnbounded(200));
    _previewDropdown = dropdown;
    _previewAlign->SetContent(constrained);
}
}

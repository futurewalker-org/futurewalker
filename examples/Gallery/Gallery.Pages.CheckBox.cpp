// SPDX-License-Identifier: MPL-2.0 OR CC0-1.0
#include "Gallery.Pages.CheckBox.hpp"

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
#include <Futurewalker.Component.Lamp.CheckBox.hpp>
#include <Futurewalker.Component.Lamp.DropdownMenuButton.hpp>

#include <Futurewalker.Core.StringFunction.hpp>

#include "Resource/Gallery.hpp"
#include "Resource/Gallery.String.CheckBoxPage.hpp"

#include <array>
#include <initializer_list>
#include <utility>

namespace Futurewalker::Gallery
{
namespace
{
namespace StringId = R::Gallery::String::CheckBoxPage;

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

auto LoadStateName(Shared<Resource> const& resource, Lamp::CheckBoxState const state) -> String
{
    switch (state)
    {
    case Lamp::CheckBoxState::Unchecked:
        return resource->LoadString(StringId::StateUnchecked);
    case Lamp::CheckBoxState::Checked:
        return resource->LoadString(StringId::StateChecked);
    case Lamp::CheckBoxState::Indeterminate:
        return resource->LoadString(StringId::StateIndeterminate);
    }
    return {};
}

auto MakeVariants(Shared<Resource> const& resource) -> Shared<View>
{
    auto const states = std::array {
        std::pair {Lamp::CheckBoxState::Unchecked, StringId::StateUnchecked},
        std::pair {Lamp::CheckBoxState::Checked, StringId::StateChecked},
        std::pair {Lamp::CheckBoxState::Indeterminate, StringId::StateIndeterminate},
    };

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
        row->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Center);
        for (auto const& [state, nameId] : states)
        {
            auto item = FlexLayout::Make();
            item->SetDirection(FlexLayoutDirection::Row);
            item->SetMainAxisSize(FlexLayoutMainAxisSize::Min);
            item->SetCrossAxisSize(FlexLayoutCrossAxisSize::Min);
            item->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Center);
            auto checkBox = Lamp::CheckBox::MakeWithState(state);
            item->AddChild(checkBox);
            auto name = Lamp::TextView::MakeWithText(resource->LoadString(nameId));
            AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*name, Lamp::Style::FontSizeBodyMedium);
            FlexLayout::SetChildMargin(name, EdgeInsets(8, 0, 0, 0));
            item->AddChild(name);
            auto itemPadding = PaddingView::MakeWithPaddingAndContent(EdgeInsets::MakeUniform(5), item);
            itemPadding->SetEnabled(enabled);
            row->AddChild(itemPadding);
        }
        innerColumn->AddChild(row);
    };
    makeRow(StringId::EnabledCheckBoxes, true);
    makeRow(StringId::DisabledCheckBoxes, false);
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

auto CheckBoxPage::Make() -> Shared<CheckBoxPage>
{
    return View::MakeDerived<CheckBoxPage>();
}

CheckBoxPage::CheckBoxPage(PassKey<View> key)
  : View(key)
{
}

auto CheckBoxPage::Initialize() -> void
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
            _stateText = Lamp::TextView::MakeWithText(u8"");
            AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*_stateText, Lamp::Style::FontSizeBodyMedium);
            AttributeNode::SetReference<Lamp::TextViewStyle::Color>(*_stateText, Lamp::Style::ColorOnSurfaceVariant);
            FlexLayout::SetChildMargin(_stateText, EdgeInsets(0, 10, 0, 0));
            innerColumn->AddChild(_stateText);
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

    EventReceiver::Connect(*this, *this, &CheckBoxPage::ReceiveEvent);
}

auto CheckBoxPage::ReceiveEvent(Event<>& event) -> Bool
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
        }
        if (event.Is<Lamp::CheckBoxEvent::Pressed>() && sender == _previewCheckBox)
        {
            auto const state = _previewCheckBox->GetState();
            if (state == Lamp::CheckBoxState::Unchecked)
            {
                _previewCheckBox->SetState(Lamp::CheckBoxState::Checked);
            }
            else if (state == Lamp::CheckBoxState::Checked)
            {
                _previewCheckBox->SetState(Lamp::CheckBoxState::Indeterminate);
            }
            else
            {
                _previewCheckBox->SetState(Lamp::CheckBoxState::Unchecked);
            }
            UpdateStateText();
            return true;
        }
    }
    return false;
}

auto CheckBoxPage::UpdatePreview() -> void
{
    if (!_previewCheckBox)
    {
        auto labelText = _resource->LoadString(StringId::CheckBoxLabelText);
        _previewCheckBox = Lamp::CheckBox::MakeWithStateAndLabel(Lamp::CheckBoxState::Unchecked, labelText);
        _previewAlign->SetContent(_previewCheckBox);
    }
    _previewCheckBox->SetEnabled(_previewEnabled);
    UpdateStateText();
}

auto CheckBoxPage::UpdateStateText() -> void
{
    auto const stateName = LoadStateName(_resource, _previewCheckBox->GetState());
    _stateText->SetText(StringFunction::Format(_resource->LoadString(StringId::CurrentState), stateName));
}
}

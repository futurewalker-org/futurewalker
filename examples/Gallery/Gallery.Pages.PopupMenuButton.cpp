// SPDX-License-Identifier: MPL-2.0 OR CC0-1.0
#include "Gallery.Pages.PopupMenuButton.hpp"

#include <Futurewalker.Application.FlexLayout.hpp>
#include <Futurewalker.Application.PaddingView.hpp>
#include <Futurewalker.Application.BoxView.hpp>
#include <Futurewalker.Application.ClipView.hpp>
#include <Futurewalker.Application.ScrollView.hpp>
#include <Futurewalker.Application.AlignView.hpp>
#include <Futurewalker.Application.ConstrainedView.hpp>
#include <Futurewalker.Application.Menu.hpp>
#include <Futurewalker.Application.MenuItem.hpp>

#include <Futurewalker.Resource.ResourceManager.hpp>
#include <Futurewalker.Resource.Resource.hpp>

#include <Futurewalker.Component.Lamp.Style.hpp>
#include <Futurewalker.Component.Lamp.TextView.hpp>
#include <Futurewalker.Component.Lamp.PopupMenuButton.hpp>
#include <Futurewalker.Component.Lamp.DropdownMenuButton.hpp>

#include <Futurewalker.Action.Action.hpp>
#include <Futurewalker.Action.CommandNode.hpp>
#include <Futurewalker.Action.CommandId.hpp>

#include <Futurewalker.Core.StringFunction.hpp>

#include "Resource/Gallery.hpp"
#include "Resource/Gallery.String.PopupMenuButtonPage.hpp"

#include <initializer_list>
#include <utility>

namespace Futurewalker::Gallery
{
namespace
{
namespace StringId = R::Gallery::String::PopupMenuButtonPage;

UniqueCommandId const CommandOpen;
UniqueCommandId const CommandSave;
UniqueCommandId const CommandDelete;

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

auto MakeMenu(Shared<Resource> const& resource) -> Menu
{
    auto menu = Menu();
    {
        auto item = MenuItem(MenuItemType::Item);
        item.SetTitle(resource->LoadString(StringId::MenuItemOpen));
        item.SetCommandId(CommandOpen);
        menu.AddItem(item);
    }
    {
        auto item = MenuItem(MenuItemType::Item);
        item.SetTitle(resource->LoadString(StringId::MenuItemSave));
        item.SetCommandId(CommandSave);
        item.SetAccessKey(u8"Ctrl+D");
        menu.AddItem(item);
    }
    {
        auto item = MenuItem(MenuItemType::Item);
        item.SetTitle(resource->LoadString(StringId::MenuItemDelete));
        item.SetSubtitle(resource->LoadString(StringId::MenuItemDeleteSubtitle));
        item.SetCommandId(CommandDelete);
        menu.AddItem(item);
    }
    {
        auto item4 = MenuItem(MenuItemType::Separator);
        menu.AddItem(item4);
    }
    {
        auto item3 = MenuItem(MenuItemType::SubMenu);
        item3.SetTitle(resource->LoadString(StringId::MenuItemSubMenu));
        auto subMenu = Menu();
        {
            auto subMenuItem1 = MenuItem(MenuItemType::Item);
            subMenuItem1.SetId(3U);
            subMenuItem1.SetTitle(resource->LoadString(StringId::MenuItemSubMenuItem));
            subMenu.AddItem(subMenuItem1);
            item3.SetSubMenu(subMenu);
        }
        menu.AddItem(item3);
    }
    return menu;
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
        auto button = Lamp::PopupMenuButton::MakeWithContent(Lamp::TextView::MakeWithText(resource->LoadString(StringId::ButtonText)));
        button->SetMenu(MakeMenu(resource));
        auto buttonPadding = PaddingView::MakeWithPaddingAndContent(EdgeInsets::MakeUniform(5), button);
        buttonPadding->SetEnabled(enabled);
        row->AddChild(buttonPadding);
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

auto PopupMenuButtonPage::Make() -> Shared<PopupMenuButtonPage>
{
    return View::MakeDerived<PopupMenuButtonPage>();
}

PopupMenuButtonPage::PopupMenuButtonPage(PassKey<View> key)
  : View(key)
{
}

auto PopupMenuButtonPage::Initialize() -> void
{
    _resource = ResourceManager::GetResource(M::Gallery);

    auto makeAction = [this](UInt32 const labelId, Bool const enabled) {
        return Action::Make([this, labelId, enabled](Event<>& event) -> Bool {
            if (event.Is<ActionEvent::State>())
            {
                auto parameter = event.As<ActionEvent::State>();
                parameter->SetEnabled(enabled);
                event = parameter;
                return true;
            }
            if (event.Is<ActionEvent::Execute>())
            {
                _lastActionText->SetText(StringFunction::Format(_resource->LoadString(StringId::LastAction), _resource->LoadString(labelId)));
                return true;
            }
            return false;
        });
    };

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
                auto const control = MakeLabeledDropdown(_resource, StringId::AnchorEdgeLabel,
                                                         {StringId::AnchorEdgeLeading, StringId::AnchorEdgeTop, StringId::AnchorEdgeTrailing, StringId::AnchorEdgeBottom}, 3);
                _edgeDropdown = control.second;
                row->AddChild(control.first);
            }
            {
                auto const control = MakeLabeledDropdown(_resource, StringId::AnchorAlignmentLabel,
                                                         {StringId::AnchorAlignmentStart, StringId::AnchorAlignmentCenter, StringId::AnchorAlignmentEnd}, 0);
                _alignmentDropdown = control.second;
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
            _lastActionText = Lamp::TextView::MakeWithText(StringFunction::Format(_resource->LoadString(StringId::LastAction), _resource->LoadString(StringId::NoAction)));
            AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*_lastActionText, Lamp::Style::FontSizeBodyMedium);
            AttributeNode::SetReference<Lamp::TextViewStyle::Color>(*_lastActionText, Lamp::Style::ColorOnSurfaceVariant);
            FlexLayout::SetChildMargin(_lastActionText, EdgeInsets(0, 10, 0, 0));
            innerColumn->AddChild(_lastActionText);
        }
        auto previewContainer = MakeSectionContainer(innerColumn);
        FlexLayout::SetChildMargin(previewContainer, EdgeInsets(0, 10, 0, 0));
        column->AddChild(previewContainer);
    }
    auto columnPadding = PaddingView::MakeWithPaddingAndContent(Lamp::Style::PaddingLarge, column);
    auto scroll = ScrollView::MakeWithContent(columnPadding);
    scroll->SetDirection(ScrollViewDirection::Vertical);
    AddChildFront(scroll);

    CommandNode::AddAction(*this, CommandOpen, makeAction(StringId::MenuItemOpen, true));
    CommandNode::AddAction(*this, CommandSave, makeAction(StringId::MenuItemSave, true));
    CommandNode::AddAction(*this, CommandDelete, makeAction(StringId::MenuItemDelete, false));

    UpdatePreview();

    EventReceiver::Connect(*this, *this, &PopupMenuButtonPage::ReceiveEvent);
}

auto PopupMenuButtonPage::ReceiveEvent(Event<>& event) -> Bool
{
    if (event.Is<ViewEvent::Notify>())
    {
        auto const& notify = event.As<ViewEvent::Notify>();
        auto const sender = notify->GetSender();
        if (event.Is<Lamp::DropdownMenuButtonEvent::CurrentItemChanged>())
        {
            auto const& changed = event.As<Lamp::DropdownMenuButtonEvent::CurrentItemChanged>();
            if (sender == _edgeDropdown)
            {
                _anchorEdge = static_cast<PopupAnchorEdge>(changed->GetCurrentIndex());
                UpdatePreview();
                return true;
            }
            if (sender == _alignmentDropdown)
            {
                _anchorAlignment = static_cast<PopupAnchorAlignment>(changed->GetCurrentIndex());
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
    }
    return false;
}

auto PopupMenuButtonPage::UpdatePreview() -> void
{
    auto button = Lamp::PopupMenuButton::MakeWithContent(Lamp::TextView::MakeWithText(_resource->LoadString(StringId::ButtonText)));
    button->SetMenu(MakeMenu(_resource));
    button->SetAnchorEdge(_anchorEdge);
    button->SetAnchorAlignment(_anchorAlignment);
    button->SetEnabled(_previewEnabled);
    _previewButton = button;
    _previewAlign->SetContent(button);
}
}

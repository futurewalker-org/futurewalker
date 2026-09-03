// SPDX-License-Identifier: MPL-2.0 OR CC0-1.0
#include "Gallery.Pages.TextButton.hpp"

#include <Futurewalker.Application.FlexLayout.hpp>
#include <Futurewalker.Application.PaddingView.hpp>
#include <Futurewalker.Application.BoxView.hpp>
#include <Futurewalker.Application.ClipView.hpp>
#include <Futurewalker.Application.ScrollView.hpp>
#include <Futurewalker.Application.ThemeView.hpp>
#include <Futurewalker.Application.AlignView.hpp>
#include <Futurewalker.Application.ConstrainedView.hpp>

#include <Futurewalker.Resource.ResourceManager.hpp>
#include <Futurewalker.Resource.Resource.hpp>

#include <Futurewalker.Graphics.SvgDocument.hpp>

#include <Futurewalker.Component.Lamp.Style.hpp>
#include <Futurewalker.Component.Lamp.TextView.hpp>
#include <Futurewalker.Component.Lamp.TextButton.hpp>
#include <Futurewalker.Component.Lamp.DropdownMenuButton.hpp>

#include <Futurewalker.Core.StringFunction.hpp>

#include "Resource/Gallery.hpp"
#include "Resource/Gallery.Icon.hpp"
#include "Resource/Gallery.String.TextButtonPage.hpp"

#include <array>
#include <initializer_list>
#include <utility>

namespace Futurewalker::Gallery
{
namespace
{
namespace StringId = R::Gallery::String::TextButtonPage;

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

auto MakeVariants(Shared<Resource> const& resource) -> Shared<View>
{
    auto const themes = std::array {
        std::pair {Lamp::TextButtonTheme::Type::Filled, StringId::ThemeFilled},
        std::pair {Lamp::TextButtonTheme::Type::Tonal, StringId::ThemeTonal},
        std::pair {Lamp::TextButtonTheme::Type::Outlined, StringId::ThemeOutlined},
        std::pair {Lamp::TextButtonTheme::Type::Text, StringId::ThemeText},
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
        for (auto const& [type, nameId] : themes)
        {
            auto const text = StringFunction::Format(u8"{} {}", resource->LoadString(nameId), resource->LoadString(StringId::PreviewButtonText));
            auto theme = Lamp::TextButtonTheme::Make(type);
            auto button = Lamp::TextButton::MakeWithText(text);
            auto buttonTheme = ThemeView::MakeWithContent(theme, button);
            auto buttonPadding = PaddingView::MakeWithPaddingAndContent(EdgeInsets::MakeUniform(5), buttonTheme);
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

auto TextButtonPage::Make() -> Shared<TextButtonPage>
{
    return View::MakeDerived<TextButtonPage>();
}

TextButtonPage::TextButtonPage(PassKey<View> key)
  : View(key)
{
}

auto TextButtonPage::Initialize() -> void
{
    _resource = ResourceManager::GetResource(M::Gallery);
    {
        auto file = _resource->LoadFile(R::Gallery::Icon::House);
        auto svg = Graphics::SvgDocument::LoadFromStream(file);
        _previewIcon = Icon::MakeFromSvgDocument(std::move(svg));
    }

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
                auto const control = MakeLabeledDropdown(_resource, StringId::ThemeLabel, {StringId::ThemeFilled, StringId::ThemeTonal, StringId::ThemeOutlined, StringId::ThemeText}, 0);
                _themeDropdown = control.second;
                row->AddChild(control.first);
            }
            {
                auto const control = MakeLabeledDropdown(_resource, StringId::StateLabel, {StringId::StateEnabled, StringId::StateDisabled}, 0);
                _stateDropdown = control.second;
                FlexLayout::SetChildMargin(control.first, EdgeInsets(20, 0, 0, 0));
                row->AddChild(control.first);
            }
            {
                auto const control = MakeLabeledDropdown(_resource, StringId::IconLabel, {StringId::IconWithoutIcon, StringId::IconWithIcon}, 0);
                _iconDropdown = control.second;
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

    EventReceiver::Connect(*this, *this, &TextButtonPage::ReceiveEvent);
}

auto TextButtonPage::ReceiveEvent(Event<>& event) -> Bool
{
    if (event.Is<ViewEvent::Notify>())
    {
        auto const& notify = event.As<ViewEvent::Notify>();
        auto const sender = notify->GetSender();
        if (event.Is<Lamp::DropdownMenuButtonEvent::CurrentItemChanged>())
        {
            auto const& changed = event.As<Lamp::DropdownMenuButtonEvent::CurrentItemChanged>();
            if (sender == _themeDropdown)
            {
                _themeType = static_cast<Lamp::TextButtonTheme::Type>(changed->GetCurrentIndex());
                UpdatePreview();
                return true;
            }
            if (sender == _stateDropdown)
            {
                _previewEnabled = changed->GetCurrentIndex() == 0;
                UpdatePreview();
                return true;
            }
            if (sender == _iconDropdown)
            {
                _previewIconVisible = changed->GetCurrentIndex() == 1;
                UpdatePreview();
                return true;
            }
        }
        if (event.Is<Lamp::TextButtonEvent::Press>() && sender == _previewButton)
        {
            ++_pressCount;
            _pressCountText->SetText(StringFunction::Format(_resource->LoadString(StringId::PressCount), _pressCount));
            return true;
        }
    }
    return false;
}

auto TextButtonPage::UpdatePreview() -> void
{
    auto const text = _resource->LoadString(StringId::PreviewButtonText);
    auto button = Lamp::TextButton::MakeWithText(text);
    if (_previewIconVisible)
    {
        button = Lamp::TextButton::MakeWithTextAndIcon(text, _previewIcon);
    }
    auto theme = Lamp::TextButtonTheme::Make(_themeType);
    auto themeView = ThemeView::MakeWithContent(theme, button);
    themeView->SetEnabled(_previewEnabled);
    _previewButton = button;
    _previewAlign->SetContent(themeView);
}
}

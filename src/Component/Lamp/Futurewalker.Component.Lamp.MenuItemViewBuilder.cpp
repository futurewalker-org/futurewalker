// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Component.Lamp.MenuItemViewBuilder.hpp"
#include "Futurewalker.Component.Lamp.MenuItemView.hpp"
#include "Futurewalker.Component.Lamp.TextView.hpp"
#include "Futurewalker.Component.Lamp.IconView.hpp"
#include "Futurewalker.Component.Lamp.Style.hpp"

#include "Futurewalker.Application.MenuItem.hpp"
#include "Futurewalker.Application.KeyFunction.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace
{
auto GetAccessKeyString(String const& accessKey, ModifierKeyFlags const modifiers) -> String
{
    auto parts = std::vector<String>();
    auto modifierStrings = KeyFunction::GetModifierKeyStrings(modifiers);
    parts.insert(parts.end(), modifierStrings.begin(), modifierStrings.end());
    parts.push_back(accessKey);
    return StringFunction::Join(u8"+", parts);
}
}

///
/// @brief Make standard menu item view builder for menu view.
///
auto MenuItemViewBuilder::MakeForMenuView() -> MenuItemViewBuilder
{
    return MenuItemViewBuilder([](MenuItemViewBuilder const&, MenuItem const& item) -> Shared<View> {
        auto const type = item.GetType();
        if (type == MenuItemType::Item)
        {
            auto leadingIcon = IconView::MakeWithIcon(item.GetLeadingIcon());
            AttributeNode::SetReference<&IconViewStyle::Size>(*leadingIcon, Style::IconSizeSmall);
            auto text = TextView::MakeWithText(item.GetTitle());
            auto itemView = MenuItemView::Make();
            itemView->SetLeadingView(leadingIcon);
            itemView->SetMiddleView(text);

            if (!item.GetTrailingIcon().IsEmpty())
            {
                auto trailingIcon = IconView::MakeWithIcon(item.GetTrailingIcon());
                AttributeNode::SetReference<&IconViewStyle::Size>(*trailingIcon, Style::IconSizeSmall);
                itemView->SetTrailingView(trailingIcon);
            }

            if (!item.GetAccessKey().IsEmpty())
            {
                auto accessKeyString = GetAccessKeyString(item.GetAccessKey(), item.GetAccessKeyModifiers());
                auto accessKeyText = TextView::MakeWithText(accessKeyString);
                AttributeNode::SetReference<&TextViewStyle::FontSize>(*accessKeyText, Style::FontSizeLabelSmall);
                accessKeyText->SetEnabled(false);
                itemView->SetTrailingView(accessKeyText);
            }
            return itemView;
        }
        else if (type == MenuItemType::SubMenu)
        {
            auto leadingIcon = IconView::MakeWithIcon(item.GetLeadingIcon());
            auto trailingIcon = IconView::MakeWithIcon(Icon::MakeFromFont(Graphics::FontFamily(u8"Segoe Fluent Icons"), 0xE974));
            AttributeNode::SetReference<&IconViewStyle::Size>(*leadingIcon, Style::IconSizeSmall);
            AttributeNode::SetReference<&IconViewStyle::Size>(*trailingIcon, Style::IconSizeExtraSmall);
            auto text = TextView::MakeWithText(item.GetTitle());
            auto itemView = MenuItemView::Make();
            itemView->SetLeadingView(leadingIcon);
            itemView->SetMiddleView(text);
            itemView->SetTrailingView(trailingIcon);
            return itemView;
        }
        else if (type == MenuItemType::Separator)
        {
            // TODO;
        }
        else
        {
            FW_DEBUG_ASSERT(false);
        }
        return {};
    });
}

auto MenuItemViewBuilder::Make(MenuItemViewBuilderFunction const& function) -> MenuItemViewBuilder
{
    return MenuItemViewBuilder(function);
}

///
/// @brief Builds item view for the given menu item.
///
/// @param item Menu item.
///
/// @return Menu item view.
///
/// @note Returned view only contains layout for the menu item. Interaction handling (e.g., button) should be provided by the caller.
///
auto MenuItemViewBuilder::Build(MenuItem const& item) const -> Shared<View>
{
    if (_function)
    {
        return _function(*this, item);
    }
    return {};
}

MenuItemViewBuilder::MenuItemViewBuilder(MenuItemViewBuilderFunction const& function)
  : _function(function)
{
}
}

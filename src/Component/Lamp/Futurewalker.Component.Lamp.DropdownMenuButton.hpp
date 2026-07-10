// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.DropdownMenuButtonType.hpp"
#include "Futurewalker.Component.Lamp.DropdownMenuButtonEvent.hpp"
#include "Futurewalker.Component.Lamp.MenuButtonViewType.hpp"
#include "Futurewalker.Component.Lamp.TextViewType.hpp"

#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.PopupType.hpp"
#include "Futurewalker.Application.FlexLayoutType.hpp"
#include "Futurewalker.Application.PaddingViewType.hpp"

#include "Futurewalker.Attribute.AttributeAccessor.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Button that shows dropdown menu when activated.
///
class DropdownMenuButton : public View
{
public:
    struct ItemInfo
    {
        String name; // Display name of the item.
    };

    static auto Make() -> Shared<DropdownMenuButton>;
    static auto MakeWithItems(std::span<const ItemInfo> items) -> Shared<DropdownMenuButton>;

    auto AddItem(ItemInfo const& item) -> void;
    auto AddItems(std::span<const ItemInfo> items) -> void;
    auto InsertItem(SInt64 const index, ItemInfo const& item) -> void;
    auto RemoveItem(SInt64 const index) -> void;

    auto GetItemCount() const -> SInt64;
    auto GetItem(SInt64 const index) const -> Optional<ItemInfo>;

    auto GetItems() const -> std::vector<ItemInfo>;
    auto SetItems(std::span<const ItemInfo> items) -> void;

    auto GetCurrentIndex() const -> SInt64;
    auto SetCurrentIndex(SInt64 const index) -> void;

    auto SetPlaceholderText(AttributeArg<String> const& text) -> void;

    DropdownMenuButton(PassKey<View> key);

protected:
    auto Initialize() -> void override;
    auto ReceiveEvent(Event<>& event) -> Bool;
    auto ReceiveAttributeEvent(Event<>& event) -> Bool;
    auto CreatePopup() -> void;
    auto DestroyPopup() -> void;
    auto UpdatePopup() -> void;
    auto UpdateItems() -> void;
    auto UpdateButtonText() -> void;

private:
    SInt64 _currentIndex = -1;
    std::vector<ItemInfo> _items;
    Shared<TextView> _buttonText;
    Shared<MenuButtonView> _button;
    Shared<Popup> _popup;
    Shared<FlexLayout> _itemColumn;
    AttributeAccessor<String> _placeholderText;
};
}
}

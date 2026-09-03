// SPDX-License-Identifier: MPL-2.0 OR CC0-1.0
#pragma once

#include <Futurewalker.Application.View.hpp>
#include <Futurewalker.Application.AlignViewType.hpp>
#include <Futurewalker.Application.Icon.hpp>

#include <Futurewalker.Component.Lamp.IconButtonType.hpp>
#include <Futurewalker.Component.Lamp.TextViewType.hpp>
#include <Futurewalker.Component.Lamp.DropdownMenuButtonType.hpp>

#include <Futurewalker.Resource.ResourceType.hpp>

#include <array>

namespace Futurewalker::Gallery
{
class IconButtonPage : public View
{
public:
    static auto Make() -> Shared<IconButtonPage>;

    IconButtonPage(PassKey<View> key);

private:
    auto Initialize() -> void override;
    auto ReceiveEvent(Event<>& event) -> Bool;
    auto UpdatePreview() -> void;

private:
    Shared<Resource> _resource;
    Shared<AlignView> _previewAlign;
    Shared<Lamp::IconButton> _previewButton;
    Shared<Lamp::TextView> _pressCountText;
    Shared<Lamp::DropdownMenuButton> _iconDropdown;
    Shared<Lamp::DropdownMenuButton> _stateDropdown;
    Shared<Lamp::DropdownMenuButton> _sizeDropdown;
    std::array<Icon, 4> _icons;
    SInt64 _iconIndex = 0;
    SInt64 _sizeIndex = 1;
    Bool _previewEnabled = true;
    SInt64 _pressCount = 0;
};
}

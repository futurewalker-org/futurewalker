// SPDX-License-Identifier: MPL-2.0 OR CC0-1.0
#pragma once

#include <Futurewalker.Application.View.hpp>
#include <Futurewalker.Application.AlignViewType.hpp>

#include <Futurewalker.Component.Lamp.TextViewType.hpp>
#include <Futurewalker.Component.Lamp.DropdownMenuButtonType.hpp>

#include <Futurewalker.Resource.ResourceType.hpp>

namespace Futurewalker::Gallery
{
class DropdownMenuButtonPage : public View
{
public:
    static auto Make() -> Shared<DropdownMenuButtonPage>;

    DropdownMenuButtonPage(PassKey<View> key);

private:
    auto Initialize() -> void override;
    auto ReceiveEvent(Event<>& event) -> Bool;
    auto UpdatePreview() -> void;

private:
    Shared<Resource> _resource;
    Shared<AlignView> _previewAlign;
    Shared<Lamp::DropdownMenuButton> _previewDropdown;
    Shared<Lamp::TextView> _selectionText;
    Shared<Lamp::DropdownMenuButton> _stateDropdown;
    Shared<Lamp::DropdownMenuButton> _selectionDropdown;
    Bool _previewEnabled = true;
    Bool _startWithPlaceholder = true;
};
}

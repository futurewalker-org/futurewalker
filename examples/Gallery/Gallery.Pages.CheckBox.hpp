// SPDX-License-Identifier: MPL-2.0 OR CC0-1.0
#pragma once

#include <Futurewalker.Application.View.hpp>
#include <Futurewalker.Application.AlignViewType.hpp>

#include <Futurewalker.Component.Lamp.CheckBoxType.hpp>
#include <Futurewalker.Component.Lamp.TextViewType.hpp>
#include <Futurewalker.Component.Lamp.DropdownMenuButtonType.hpp>

#include <Futurewalker.Resource.ResourceType.hpp>

namespace Futurewalker::Gallery
{
class CheckBoxPage : public View
{
public:
    static auto Make() -> Shared<CheckBoxPage>;

    CheckBoxPage(PassKey<View> key);

private:
    auto Initialize() -> void override;
    auto ReceiveEvent(Event<>& event) -> Bool;
    auto UpdatePreview() -> void;
    auto UpdateStateText() -> void;

private:
    Shared<Resource> _resource;
    Shared<AlignView> _previewAlign;
    Shared<Lamp::CheckBox> _previewCheckBox;
    Shared<Lamp::TextView> _stateText;
    Shared<Lamp::DropdownMenuButton> _stateDropdown;
    Bool _previewEnabled = true;
};
}

// SPDX-License-Identifier: MPL-2.0 OR CC0-1.0
#pragma once

#include <Futurewalker.Application.View.hpp>
#include <Futurewalker.Application.AlignViewType.hpp>
#include <Futurewalker.Application.PopupType.hpp>

#include <Futurewalker.Component.Lamp.PopupMenuButtonType.hpp>
#include <Futurewalker.Component.Lamp.PopupMenuType.hpp>
#include <Futurewalker.Component.Lamp.TextViewType.hpp>
#include <Futurewalker.Component.Lamp.DropdownMenuButtonType.hpp>

#include <Futurewalker.Resource.ResourceType.hpp>

namespace Futurewalker::Gallery
{
class PopupMenuButtonPage : public View
{
public:
    static auto Make() -> Shared<PopupMenuButtonPage>;

    PopupMenuButtonPage(PassKey<View> key);

private:
    auto Initialize() -> void override;
    auto ReceiveEvent(Event<>& event) -> Bool;
    auto UpdatePreview() -> void;

private:
    Shared<Resource> _resource;
    Shared<AlignView> _previewAlign;
    Shared<Lamp::PopupMenuButton> _previewButton;
    Shared<Lamp::TextView> _lastActionText;
    Shared<Lamp::DropdownMenuButton> _edgeDropdown;
    Shared<Lamp::DropdownMenuButton> _alignmentDropdown;
    Shared<Lamp::DropdownMenuButton> _stateDropdown;
    PopupAnchorEdge _anchorEdge = PopupAnchorEdge::Bottom;
    PopupAnchorAlignment _anchorAlignment = PopupAnchorAlignment::Start;
    Bool _previewEnabled = true;
};
}

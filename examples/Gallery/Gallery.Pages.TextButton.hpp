// SPDX-License-Identifier: MPL-2.0 OR CC0-1.0
#pragma once

#include <Futurewalker.Application.View.hpp>
#include <Futurewalker.Application.AlignViewType.hpp>
#include <Futurewalker.Application.Icon.hpp>

#include <Futurewalker.Component.Lamp.TextButtonType.hpp>
#include <Futurewalker.Component.Lamp.TextButtonTheme.hpp>
#include <Futurewalker.Component.Lamp.TextViewType.hpp>
#include <Futurewalker.Component.Lamp.DropdownMenuButtonType.hpp>

#include <Futurewalker.Resource.ResourceType.hpp>

namespace Futurewalker::Gallery
{
class TextButtonPage : public View
{
public:
    static auto Make() -> Shared<TextButtonPage>;

    TextButtonPage(PassKey<View> key);

private:
    auto Initialize() -> void override;
    auto ReceiveEvent(Event<>& event) -> Bool;
    auto UpdatePreview() -> void;

private:
    Shared<Resource> _resource;
    Shared<AlignView> _previewAlign;
    Shared<Lamp::TextButton> _previewButton;
    Shared<Lamp::TextView> _pressCountText;
    Shared<Lamp::DropdownMenuButton> _themeDropdown;
    Shared<Lamp::DropdownMenuButton> _stateDropdown;
    Shared<Lamp::DropdownMenuButton> _iconDropdown;
    Icon _previewIcon;
    Lamp::TextButtonTheme::Type _themeType = Lamp::TextButtonTheme::Type::Filled;
    Bool _previewEnabled = true;
    Bool _previewIconVisible = false;
    SInt64 _pressCount = 0;
};
}

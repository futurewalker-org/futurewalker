// SPDX-License-Identifier: MPL-2.0

#include "ComponentGalleryApplication.hpp"

#include <Futurewalker.Component.Lamp.TextView.hpp>
#include <Futurewalker.Component.Lamp.Theme.hpp>
#include <Futurewalker.Component.Lamp.Style.hpp>
#include <Futurewalker.Component.Lamp.PopupMenuButton.hpp>
#include <Futurewalker.Component.Lamp.MenuBar.hpp>
#include <Futurewalker.Component.Lamp.TextButton.hpp>

#include <Futurewalker.Application.ApplicationEvent.hpp>
#include <Futurewalker.Application.ApplicationTheme.hpp>
#include <Futurewalker.Application.Window.hpp>
#include <Futurewalker.Application.WindowFrame.hpp>
#include <Futurewalker.Application.AlignView.hpp>
#include <Futurewalker.Application.FlexLayout.hpp>
#include <Futurewalker.Application.ContainerView.hpp>
#include <Futurewalker.Application.PaddingView.hpp>
#include <Futurewalker.Application.SizedView.hpp>
#include <Futurewalker.Application.Menu.hpp>
#include <Futurewalker.Application.MenuItem.hpp>
#include <Futurewalker.Application.Key.hpp>
#include <Futurewalker.Application.ActionView.hpp>
#include <Futurewalker.Application.ThemeView.hpp>

#include <Futurewalker.Action.Action.hpp>
#include <Futurewalker.Action.CommandDispatcher.hpp>

#include <Futurewalker.Event.EventReceiver.hpp>
#include <Futurewalker.Event.EventWaiter.hpp>

namespace Futurewalker::ComponentGallery
{
namespace
{
auto const CommandFileNew = UniqueCommandId();
auto const CommandFileOpen = UniqueCommandId();
auto const CommandFileSave = UniqueCommandId();
auto const CommandEditCut = UniqueCommandId();
auto const CommandEditCopy = UniqueCommandId();
auto const CommandEditPaste = UniqueCommandId();
}

auto ComponentGalleryApplication::Make() -> Shared<ComponentGalleryApplication>
{
    return Application::MakeDerived<ComponentGalleryApplication>();
}

ComponentGalleryApplication::ComponentGalleryApplication(PassKey<Application> key)
  : Application(key, ApplicationOptions {.identifier = u8"com.example.futurewalker.componentgallery"})
{
}

auto ComponentGalleryApplication::Initialize() -> void
{
    _commandDispatcher = CommandDispatcher::Make();
    _commandDispatcher->Attach();
    _commandDispatcher->AddAction(CommandFileNew, Action::Make(*this, &ComponentGalleryApplication::ReceiveCommandEvent));
    _commandDispatcher->AddAction(CommandEditPaste, Action::Make(*this, &ComponentGalleryApplication::ReceiveCommandEvent));
    EventReceiver::Connect(*this, *this, &ComponentGalleryApplication::ReceiveEvent);
}

auto ComponentGalleryApplication::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<ApplicationEvent::Started>())
    {
        ApplicationTheme::AddTheme(ThemeBrightness::Light, Shared<Lamp::Theme>::Make(ThemeBrightness::Light));
        ApplicationTheme::AddTheme(ThemeBrightness::Dark, Shared<Lamp::Theme>::Make(ThemeBrightness::Dark));
        ApplicationTheme::SetBrightness(ApplicationThemeBrightness::Dark);

        auto buttonColumn = FlexLayout::Make();
        buttonColumn->SetDirection(FlexLayoutDirection::Column);
        buttonColumn->SetMainAxisSize(FlexLayoutMainAxisSize::Max);
        buttonColumn->SetMainAxisAlignment(FlexLayoutMainAxisAlignment::Start);
        buttonColumn->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Stretch);
        buttonColumn->SetCrossAxisSize(FlexLayoutCrossAxisSize::Min);
        auto buttonColumnSize = SizedView::MakeWithContent(200, Dp::Infinity(), buttonColumn);

        auto menu = Menu();
        {
            auto menuItem = MenuItem(MenuItemType::Item);
            auto const icon = Icon::MakeFromFont(Graphics::FontFamily(u8"Segoe Fluent Icons"), 0xEB52);
            menuItem.SetTitle(u8"Menu Item 1");
            menuItem.SetLeadingIcon(icon);
            menu.AddItem(menuItem);
        }
        {
            auto menuItem = MenuItem(MenuItemType::Item);
            auto const icon = Icon::MakeFromFont(Graphics::FontFamily(u8"Segoe Fluent Icons"), 0xE713);
            menuItem.SetTitle(u8"Menu Item 2");
            menuItem.SetLeadingIcon(icon);
            menu.AddItem(menuItem);
        }
        {
            auto menuItem = MenuItem(MenuItemType::Item);
            menuItem.SetAccessKey(u8"A");
            menuItem.SetAccessKeyModifiers(ModifierKeyFlags::Control | ModifierKeyFlags::Shift);
            menuItem.SetTitle(u8"Menu Item 3");
            menu.AddItem(menuItem);
        }
        static auto commandId = UniqueCommandId();
        {
            auto menuItem = MenuItem(MenuItemType::Item);
            menuItem.SetTitle(u8"Menu Item 4");
            menuItem.SetTrailingIcon(Icon::MakeFromFont(Graphics::FontFamily(u8"Segoe Fluent Icons"), 0xE76C));
            menuItem.SetCommandId(commandId);
            menu.AddItem(menuItem);
        }

        {
            auto menuItem = MenuItem(MenuItemType::SubMenu);
            menuItem.SetTitle(u8"Submenu Test");
            {
                auto submenu = Menu();
                {
                    auto subMenuItem = MenuItem(MenuItemType::Item);
                    subMenuItem.SetTitle(u8"Submenu Item 1");
                    submenu.AddItem(subMenuItem);
                }
                {
                    auto subMenuItem = MenuItem(MenuItemType::Item);
                    subMenuItem.SetTitle(u8"Submenu Item 2");
                    submenu.AddItem(subMenuItem);
                }
                menuItem.SetSubMenu(submenu);
            }
            menu.AddItem(menuItem);
        }

        auto text = Lamp::TextView::MakeWithText(u8"Hello, World!");
        auto button = Lamp::PopupMenuButton::MakeWithContent(text);
        button->SetMenu(menu);

        auto actionView = ActionView::MakeWithContent(button);
        actionView->AddAction(commandId, Action::Make([](Event<>& event) -> Async<Bool> {
            if (event.Is<ActionEvent::State>())
            {
                auto param = event.As<ActionEvent::State>();
                param->SetEnabled(true);
                event = param;
                co_return true;
            }
            else if (event.Is<ActionEvent::Execute>())
            {
                FW_DEBUG_LOG_INFO("PopupMenuButton: Menu Item for command1 selected.");
                co_return true;
            }
            co_return false;
        }));
        auto column = FlexLayout::Make();
        column->SetDirection(FlexLayoutDirection::Column);
        column->SetMainAxisSize(FlexLayoutMainAxisSize::Max);
        column->SetCrossAxisSize(FlexLayoutCrossAxisSize::Max);
        column->SetMainAxisAlignment(FlexLayoutMainAxisAlignment::Start);
        column->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Center);
        column->AddChild(actionView);
        {
            {
                auto row = FlexLayout::Make();
                row->SetDirection(FlexLayoutDirection::Row);
                row->SetMainAxisSize(FlexLayoutMainAxisSize::Min);
                row->SetCrossAxisSize(FlexLayoutCrossAxisSize::Min);
                auto makeTextButton = [](auto type, auto text, auto enabled) {
                    auto theme = Shared<Lamp::TextButtonTheme>::Make(type);
                    auto button1 = Lamp::TextButton::MakeWithText(text);
                    auto buttonTheme = ThemeView::MakeWithContent(theme, button1);
                    auto buttonPadding = PaddingView::MakeWithPaddingAndContent(EdgeInsets::MakeUniform(5), buttonTheme);
                    buttonPadding->SetEnabled(enabled);
                    return buttonPadding;
                };
                row->AddChild(makeTextButton(Lamp::TextButtonTheme::Type::Filled, u8"Filled Button", true));
                row->AddChild(makeTextButton(Lamp::TextButtonTheme::Type::Tonal, u8"Tonal Button", true));
                row->AddChild(makeTextButton(Lamp::TextButtonTheme::Type::Outlined, u8"Outlined Button", true));
                row->AddChild(makeTextButton(Lamp::TextButtonTheme::Type::Text, u8"Text Button", true));
                column->AddChild(row);
            }
            {
                auto row = FlexLayout::Make();
                row->SetDirection(FlexLayoutDirection::Row);
                row->SetMainAxisSize(FlexLayoutMainAxisSize::Min);
                row->SetCrossAxisSize(FlexLayoutCrossAxisSize::Min);
                auto makeTextButton = [](auto type, auto text, auto enabled) {
                    auto theme = Shared<Lamp::TextButtonTheme>::Make(type);
                    auto button1 = Lamp::TextButton::MakeWithText(text);
                    auto buttonTheme = ThemeView::MakeWithContent(theme, button1);
                    auto buttonPadding = PaddingView::MakeWithPaddingAndContent(EdgeInsets::MakeUniform(5), buttonTheme);
                    buttonPadding->SetEnabled(enabled);
                    return buttonPadding;
                };
                row->AddChild(makeTextButton(Lamp::TextButtonTheme::Type::Filled, u8"Filled Button", false));
                row->AddChild(makeTextButton(Lamp::TextButtonTheme::Type::Tonal, u8"Tonal Button", false));
                row->AddChild(makeTextButton(Lamp::TextButtonTheme::Type::Outlined, u8"Outlined Button", false));
                row->AddChild(makeTextButton(Lamp::TextButtonTheme::Type::Text, u8"Text Button", false));
                column->AddChild(row);
            }
        }
        auto contentArea = ContainerView::MakeWithContent(column);
        contentArea->SetBackgroundColor(Lamp::Style::ColorSurfaceContainer);
        contentArea->SetCornerRadius(Lamp::Style::CornerRadiusMedium);
        auto contentAreaPadding = PaddingView::MakeWithPaddingAndContent(EdgeInsets::MakeUniform(5), contentArea);

        auto leftPane = ContainerView::MakeWithContent(buttonColumnSize);
        auto rightPane = ContainerView::MakeWithContent(contentAreaPadding);
        FlexLayout::SetChildGrowFactor(rightPane, 1.0);
        FlexLayout::SetChildShrinkFactor(rightPane, 1.0);
        FlexLayout::SetChildMainAxisFlexibility(rightPane, FlexLayoutMainAxisFlexibility::Expand);

        auto row = FlexLayout::Make();
        row->SetDirection(FlexLayoutDirection::Row);
        row->SetMainAxisSize(FlexLayoutMainAxisSize::Max);
        row->SetMainAxisAlignment(FlexLayoutMainAxisAlignment::Start);
        row->SetCrossAxisSize(FlexLayoutCrossAxisSize::Max);
        row->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Stretch);
        row->AddChild(leftPane);
        row->AddChild(rightPane);

        auto frame = WindowFrame::Make();
        frame->SetContent(row);

        auto menuBar = Lamp::MenuBar::Make();
        {
            menu = Menu();
            {
                auto item = MenuItem(MenuItemType::SubMenu);
                item.SetTitle(u8"File");
                {
                    auto subMebu = Menu();
                    {
                        auto subMenuItem = MenuItem(MenuItemType::Item);
                        subMenuItem.SetTitle(u8"New");
                        subMenuItem.SetCommandId(CommandFileNew);
                        subMebu.AddItem(subMenuItem);
                    }
                    {
                        auto subMenuItem = MenuItem(MenuItemType::Item);
                        subMenuItem.SetTitle(u8"Save");
                        subMenuItem.SetCommandId(CommandFileSave);
                        subMebu.AddItem(subMenuItem);
                    }
                    {
                        auto subMenuItem = MenuItem(MenuItemType::Item);
                        subMenuItem.SetTitle(u8"Open");
                        subMenuItem.SetTrailingIcon(Icon::MakeFromFont(Graphics::FontFamily(u8"Segoe Fluent Icons"), 0xE76C));
                        subMenuItem.SetCommandId(CommandFileOpen);
                        subMebu.AddItem(subMenuItem);
                    }
                    item.SetSubMenu(subMebu);
                }
                menu.AddItem(item);
            }
            {
                auto item = MenuItem(MenuItemType::SubMenu);
                item.SetTitle(u8"Edit");
                {
                    auto subMebu = Menu();
                    {
                        auto subMenuItem = MenuItem(MenuItemType::Item);
                        subMenuItem.SetTitle(u8"Cut");
                        subMenuItem.SetCommandId(CommandEditCut);
                        subMebu.AddItem(subMenuItem);
                    }
                    {
                        auto subMenuItem = MenuItem(MenuItemType::Item);
                        subMenuItem.SetTitle(u8"Copy");
                        subMenuItem.SetCommandId(CommandEditCopy);
                        subMebu.AddItem(subMenuItem);
                    }
                    {
                        auto subMenuItem = MenuItem(MenuItemType::SubMenu);
                        subMenuItem.SetTitle(u8"Paste");
                        {
                            auto m = Menu();
                            auto i = MenuItem(MenuItemType::Item);
                            i.SetTitle(u8"Paste as Plain Text");
                            i.SetCommandId(CommandEditPaste);
                            m.AddItem(i);
                            i = MenuItem(MenuItemType::Item);
                            subMenuItem.SetSubMenu(m);
                        }
                        subMebu.AddItem(subMenuItem);
                    }
                    item.SetSubMenu(subMebu);
                }
                menu.AddItem(item);
            }
        }
        menuBar->SetMenu(menu);
        frame->SetTitleContent(menuBar);

        auto window = Window::Make({
            .backgroundStyle = WindowBackgroundStyle::Solid,
        });
        window->SetBackgroundColor(Lamp::Style::ColorSurface);
        window->SetContent(frame);
        window->SetFrameRect({{100, 100}, {500, 500}});
        window->SetVisible(true);

        co_await EventWaiter(*window).Wait<WindowEvent::Closed>();
        co_await Exit();
    }
    co_return false;
}

auto ComponentGalleryApplication::ReceiveCommandEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<ActionEvent::Execute>())
    {
        FW_DEBUG_LOG_INFO("ComponentGalleryApplication: Command Execute requested.");
        co_return true;
    }
    else if (event.Is<ActionEvent::State>())
    {
        FW_DEBUG_LOG_INFO("ComponentGalleryApplication: Command State requested.");
        auto parameter = event.As<ActionEvent::State>();
        parameter->SetEnabled(true);
        event = parameter;
        co_return true;
    }
    co_return false;
}
}

// SPDX-License-Identifier: MPL-2.0 OR CC0-1.0
#include "Gallery.Application.hpp"
#include "Gallery.Views.RootLayout.hpp"

#include <Futurewalker.Component.Lamp.TextView.hpp>
#include <Futurewalker.Component.Lamp.Theme.hpp>
#include <Futurewalker.Component.Lamp.Style.hpp>
#include <Futurewalker.Component.Lamp.MenuBar.hpp>

#include <Futurewalker.Application.ApplicationEvent.hpp>
#include <Futurewalker.Application.ApplicationTheme.hpp>
#include <Futurewalker.Application.Window.hpp>
#include <Futurewalker.Application.WindowFrame.hpp>
#include <Futurewalker.Application.AlignView.hpp>
#include <Futurewalker.Application.Menu.hpp>
#include <Futurewalker.Application.MenuItem.hpp>
#include <Futurewalker.Application.Screen.hpp>

#include <Futurewalker.Resource.Resource.hpp>
#include <Futurewalker.Resource.ResourceManager.hpp>

#include <Futurewalker.Async.ThisThread.hpp>

#include <Futurewalker.Action.Action.hpp>
#include <Futurewalker.Action.CommandDispatcher.hpp>

#include <Futurewalker.Async.AsyncFunction.hpp>

#include <Futurewalker.Attribute.AttributeSlotCache.hpp>

#include <Futurewalker.Event.EventReceiver.hpp>
#include <Futurewalker.Event.EventWaiter.hpp>

namespace Futurewalker::Gallery
{
namespace
{
auto const CommandApplicationHelpAbout = UniqueCommandId();
}

auto Application::Make() -> Shared<Application>
{
    return Application::MakeDerived<Application>();
}

Application::Application(PassKey<Futurewalker::Application> key)
  : Futurewalker::Application(key, ApplicationOptions {.identifier = u8"com.example.futurewalker.gallery"})
{
}

auto Application::Initialize() -> void
{
    _commandDispatcher = CommandDispatcher::Make();
    _commandDispatcher->Attach();
    _commandDispatcher->AddAction(CommandApplicationHelpAbout, Action::Make(*this, &Application::ReceiveCommandEvent));

    EventReceiver::Connect(*this, *this, &Application::ReceiveEvent);
}

auto Application::ReceiveEvent(Event<>& event) -> Bool
{
    if (event.Is<ApplicationEvent::Started>())
    {
        ApplicationTheme::AddTheme(ThemeBrightness::Light, Shared<Lamp::Theme>::Make(ThemeBrightness::Light));
        ApplicationTheme::AddTheme(ThemeBrightness::Dark, Shared<Lamp::Theme>::Make(ThemeBrightness::Dark));
        ApplicationTheme::SetBrightness(ApplicationThemeBrightness::System);

        ResourceManager::SetPreferredLocales(Locale::GetPreferredUILocales());

        AsyncFunction::SpawnFn([=, this]() -> Async<void> {
            auto frame = WindowFrame::Make();
            auto view = RootLayout::Make();
            frame->SetContent(view);

            auto menuBar = Lamp::MenuBar::MakeWithMenu(BuildMainMenu());
            frame->SetTitleContent(menuBar);

            _window = Window::Make({
                .backgroundStyle = WindowBackgroundStyle::Solid,
            });
            _window->SetBackgroundColor(Lamp::Style::ColorSurfaceContainer);
            _window->SetContent(frame);
            _window->SetSizeConstraints(BoxConstraints::MakeUnbounded({500, 500}));
            _window->SetFrameRect({0, 0, 1500, 1000});
            _window->SetVisible(true);

            co_await EventWaiter(*_window).Wait<WindowEvent::Closed>();
            RequestQuit();
        }).Detach();
    }
    return false;
}

auto Application::ReceiveCommandEvent(Event<>& event) -> Bool
{
    if (event.Is<ActionEvent>())
    {
        auto const commandId = event.As<ActionEvent>()->GetCommandId();
        if (commandId == CommandApplicationHelpAbout)
        {
            if (event.Is<ActionEvent::State>())
            {
                auto parameter = event.As<ActionEvent::State>();
                parameter->SetEnabled(true);
                event = parameter;
                return true;
            }
            else if (event.Is<ActionEvent::Execute>())
            {
                if (_window)
                {
                    AsyncFunction::SpawnFn([=, this] -> Async<void> {
                        auto const windowRect = _window->GetFrameRect();
                        auto dialogWindow = Window::Make({
                            .behavior = WindowBehavior::Dialog,
                            .backgroundStyle = WindowBackgroundStyle::Solid,
                            .owner = _window,
                            .resizable = false,
                            .minimizable = false,
                        });

                        auto text = Lamp::TextView::MakeWithText(u8"Futurewalker version 0.0.0");
                        AttributeNode::SetReference<Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeTitleMedium);
                        auto align = AlignView::MakeWithContent(text);
                        dialogWindow->SetContent(align);

                        auto const width = 400;
                        auto const height = 300;
                        dialogWindow->SetFrameRect(Rect<Vp>::Make({windowRect.x0 + (windowRect.GetWidth() - width) / 2, windowRect.y0 + (windowRect.GetHeight() - height) / 2}, {width, height}));
                        dialogWindow->SetBackgroundColor(Lamp::Style::ColorSurface);
                        dialogWindow->SetVisible(true);

                        co_await EventWaiter(*dialogWindow).Wait<WindowEvent::Closed>();
                    }).Detach();
                }
                return true;
            }
        }
    }
    return false;
}

auto Application::BuildMainMenu() const -> Menu
{
    auto menu = Menu();
    {
        auto item = MenuItem(MenuItemType::SubMenu);
        item.SetTitle(u8"Help");
        {
            auto subMenu = Menu();
            {
                auto subMenuItem = MenuItem(MenuItemType::Item);
                subMenuItem.SetTitle(u8"About");
                subMenuItem.SetCommandId(CommandApplicationHelpAbout);
                subMenu.AddItem(subMenuItem);
            }
            item.SetSubMenu(subMenu);
        }
        menu.AddItem(item);
    }
    return menu;
}
}

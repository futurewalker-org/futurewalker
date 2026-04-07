#include "ComponentGalleryApplication.hpp"
#include "SplitPaneController.hpp"  

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

#include <Futurewalker.Action.Action.hpp>
#include <Futurewalker.Action.CommandDispatcher.hpp>

#include <Futurewalker.Attribute.AttributeSlotCache.hpp>

#include <Futurewalker.Event.EventReceiver.hpp>
#include <Futurewalker.Event.EventWaiter.hpp>

namespace Futurewalker::ComponentGallery
{
namespace
{
auto const CommandApplicationHelpAbout = UniqueCommandId();
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
    _commandDispatcher->AddAction(CommandApplicationHelpAbout, Action::Make(*this, &ComponentGalleryApplication::ReceiveCommandEvent));

    EventReceiver::Connect(*this, *this, &ComponentGalleryApplication::ReceiveEvent);
}

auto ComponentGalleryApplication::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<ApplicationEvent::Started>())
    {
        ApplicationTheme::AddTheme(ThemeBrightness::Light, Shared<Lamp::Theme>::Make(ThemeBrightness::Light));
        ApplicationTheme::AddTheme(ThemeBrightness::Dark, Shared<Lamp::Theme>::Make(ThemeBrightness::Dark));
        ApplicationTheme::SetBrightness(ApplicationThemeBrightness::Dark);

        auto frame = WindowFrame::Make();
        auto controller = SplitPaneController::Make();
        frame->SetContent(controller->GetView());

        auto menuBar = Lamp::MenuBar::MakeWithMenu(BuildMainMenu());
        frame->SetTitleContent(menuBar);

        _window = Window::Make({
            .backgroundStyle = WindowBackgroundStyle::Solid,
        });
        _window->SetBackgroundColor(Lamp::Style::ColorSurface);
        _window->SetContent(frame);
        _window->SetSizeConstraints(BoxConstraints::MakeUnbounded({500, 500}));
        _window->SetFrameRect({0, 0, 1500, 1000});
        _window->SetVisible(true);

        co_await EventWaiter(*_window).Wait<WindowEvent::Closed>();
        co_await Exit();
    }
    co_return false;
}

auto ComponentGalleryApplication::ReceiveCommandEvent(Event<>& event) -> Async<Bool>
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
                co_return true;
            }
            else if (event.Is<ActionEvent::Execute>())
            {
                if (_window)
                {
                    auto const windowRect = _window->GetFrameRect();
                    auto dialogWindow = Window::Make({
                        .behavior = WindowBehavior::Dialog,
                        .backgroundStyle = WindowBackgroundStyle::Solid,
                        .owner = _window,
                        .resizable = false,
                        .minimizable = false,
                    });

                    auto text = Lamp::TextView::MakeWithText(u8"Futurewalker version 0.0.0");
                    AttributeNode::SetReference<&Lamp::TextViewStyle::FontSize>(*text, Lamp::Style::FontSizeTitleMedium);
                    auto align = AlignView::MakeWithContent(text);
                    dialogWindow->SetContent(align);

                    auto const width = 400;
                    auto const height = 300;
                    dialogWindow->SetFrameRect(Rect<Vp>::Make({windowRect.x0 + (windowRect.GetWidth() - width) / 2, windowRect.y0 + (windowRect.GetHeight() - height) / 2}, {width, height}));
                    dialogWindow->SetBackgroundColor(Lamp::Style::ColorSurface);
                    dialogWindow->SetVisible(true);
                    co_await EventWaiter(*dialogWindow).Wait<WindowEvent::Closed>();
                }
                co_return true;
            }
        }
    }
    co_return false;
}

auto ComponentGalleryApplication::BuildMainMenu() const -> Menu
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

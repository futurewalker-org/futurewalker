// SPDX-License-Identifier: MPL-2.0

#include "ThemeViewerApplication.hpp"
#include "ColorSchemeView.hpp"

#include <Futurewalker.Component.Lamp.TextView.hpp>
#include <Futurewalker.Component.Lamp.Theme.hpp>
#include <Futurewalker.Component.Lamp.Style.hpp>

#include <Futurewalker.Application.ApplicationEvent.hpp>
#include <Futurewalker.Application.ApplicationTheme.hpp>
#include <Futurewalker.Application.Window.hpp>
#include <Futurewalker.Application.WindowFrame.hpp>
#include <Futurewalker.Application.FlexLayout.hpp>
#include <Futurewalker.Application.ScrollView.hpp>
#include <Futurewalker.Application.ThemeView.hpp>

#include <Futurewalker.Event.EventReceiver.hpp>
#include <Futurewalker.Event.EventWaiter.hpp>

namespace Futurewalker::ThemeViewer
{
auto ThemeViewerApplication::Make() -> Shared<ThemeViewerApplication>
{
    return Application::MakeDerived<ThemeViewerApplication>();
}

ThemeViewerApplication::ThemeViewerApplication(PassKey<Application> key)
  : Application(key, ApplicationOptions {.identifier = u8"com.example.futurewalker.themeviewer"})
{
}

auto ThemeViewerApplication::Initialize() -> void
{
    EventReceiver::Connect(*this, *this, &ThemeViewerApplication::ReceiveEvent);
}

auto ThemeViewerApplication::ReceiveEvent(Event<>& event) -> Async<Bool>
{
    if (event.Is<ApplicationEvent::Started>())
    {
        ApplicationTheme::AddTheme(ThemeBrightness::Light, Shared<Lamp::Theme>::Make(ThemeBrightness::Light));
        ApplicationTheme::AddTheme(ThemeBrightness::Dark, Shared<Lamp::Theme>::Make(ThemeBrightness::Dark));

        auto column = FlexLayout::Make();
        column->SetDirection(FlexLayoutDirection::Column);
        column->SetMainAxisSize(FlexLayoutMainAxisSize::Max);
        column->SetMainAxisAlignment(FlexLayoutMainAxisAlignment::Start);
        column->SetCrossAxisSize(FlexLayoutCrossAxisSize::Max);
        column->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Center);
        {
            auto const brightness = ThemeBrightness::Light;
            auto schemeView = ColorSchemeView::Make(brightness);
            auto themeView = ThemeView::MakeWithContent(schemeView);
            themeView->SetTheme(Shared<Lamp::Theme>::Make(brightness));
            FlexLayout::SetChildMargin(themeView, EdgeInsets(0, 0, 0, 10));
            column->AddChild(themeView);
        }
        {
            auto const brightness = ThemeBrightness::Dark;
            auto schemeView = ColorSchemeView::Make(brightness);
            auto themeView = ThemeView::MakeWithContent(schemeView);
            themeView->SetTheme(Shared<Lamp::Theme>::Make(brightness));
            FlexLayout::SetChildMargin(themeView, EdgeInsets(0, 0, 0, 10));
            column->AddChild(themeView);
        }

        auto scroll = ScrollView::Make();
        scroll->SetDirection(ScrollViewDirection::Vertical);
        scroll->SetContent(column);

        auto frame = WindowFrame::Make();
        frame->SetContent(scroll);

        auto window = Window::Make({
            .backgroundStyle = WindowBackgroundStyle::Solid,
        });
        window->SetTitle(u8"Theme Viewer");
        window->SetBackgroundColor(Lamp::Style::ColorSurface);
        window->SetFrameRect({0, 0, 1000, 1000});
        window->SetContent(frame);
        window->SetVisible(true);

        co_await EventWaiter(*window).Wait<WindowEvent::Closed>();
        co_await Exit();
    }
    co_return false;
}
}

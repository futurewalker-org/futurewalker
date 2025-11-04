// SPDX-License-Identifier: MPL-2.0

#include <Futurewalker.Component.Lamp.TextView.hpp>
#include <Futurewalker.Component.Lamp.Theme.hpp>
#include <Futurewalker.Component.Lamp.Style.hpp>

#include <Futurewalker.Application.Application.hpp>
#include <Futurewalker.Application.ApplicationEvent.hpp>
#include <Futurewalker.Application.ApplicationTheme.hpp>
#include <Futurewalker.Application.Window.hpp>
#include <Futurewalker.Application.AlignView.hpp>

#include <Futurewalker.Event.EventReceiver.hpp>
#include <Futurewalker.Event.EventWaiter.hpp>

#include <Futurewalker.Base.Environment.hpp>

#include <Futurewalker.Main.Main.hpp>

namespace Futurewalker
{
class HelloWorldApplication : public Application
{
public:
    static auto Make() -> Shared<HelloWorldApplication>
    {
        return Application::MakeDerived<HelloWorldApplication>();
    }

    HelloWorldApplication(PassKey<Application> key)
      : Application(key, ApplicationOptions {.identifier = u8"com.example.futurewalker.helloworld"})
    {
    }

protected:
    auto Initialize() -> void override
    {
        EventReceiver::Connect(*this, *this, &HelloWorldApplication::ReceiveEvent);
    }

    auto ReceiveEvent(Event<>& event) -> Async<Bool>
    {
        if (event.Is<ApplicationEvent::Started>())
        {
            ApplicationTheme::PushTheme(ThemeBrightness::Light, Shared<Lamp::Theme>::Make(ThemeBrightness::Light));
            ApplicationTheme::PushTheme(ThemeBrightness::Dark, Shared<Lamp::Theme>::Make(ThemeBrightness::Dark));

            auto const hello = String(u8"Hello, World!");

            auto window = Window::Make({});
            window->SetTitle(hello);
            window->SetSize({500, 500});
            auto text = Lamp::TextView::MakeWithText(hello);
            AttributeNode::SetValue<&Lamp::TextViewStyle::FontSize>(*text, 42);
            AttributeNode::SetValue<&Lamp::TextViewStyle::FontWeight>(*text, Graphics::FontWeight::Bold());
            auto align = AlignView::MakeWithContent(text);
            window->SetContent(align);
            window->SetVisible(true);

            co_await EventWaiter(*window).Wait<WindowEvent::Closed>();
            co_await Exit();
        }
        co_return false;
    }
};

auto Main() -> Async<ExitCode>
{
    try
    {
        auto env = Environment();
        auto app = HelloWorldApplication::Make();
        co_await app->Run();
    }
    catch (...)
    {
        co_return ExitCode::Failure;
    }
    co_return ExitCode::Success;
}
}

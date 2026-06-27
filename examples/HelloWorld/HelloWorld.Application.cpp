// SPDX-License-Identifier: MPL-2.0

#include "HelloWorld.Application.hpp"

#include "Resource/HelloWorld.hpp"

#include <Futurewalker.Component.Lamp.TextView.hpp>
#include <Futurewalker.Component.Lamp.Theme.hpp>
#include <Futurewalker.Component.Lamp.Style.hpp>

#include <Futurewalker.Application.ApplicationEvent.hpp>
#include <Futurewalker.Application.ApplicationTheme.hpp>
#include <Futurewalker.Application.Window.hpp>
#include <Futurewalker.Application.AlignView.hpp>

#include <Futurewalker.Resource.Resource.hpp>
#include <Futurewalker.Resource.ResourceManager.hpp>

#include <Futurewalker.Event.EventReceiver.hpp>
#include <Futurewalker.Event.EventWaiter.hpp>

namespace Futurewalker::HelloWorld
{
auto Application::Make() -> Shared<Application>
{
    return Futurewalker::Application::MakeDerived<Application>();
}

Application::Application(PassKey<Futurewalker::Application> key)
  : Futurewalker::Application(key, ApplicationOptions {.identifier = u8"com.example.futurewalker.helloworld"})
{
}

auto Application::Initialize() -> void
{
    // Add event handler for application events.
    EventReceiver::Connect(*this, *this, &Application::ReceiveEvent);
}

auto Application::ReceiveEvent(Event<>& event) -> Bool
{
    // After Application::Run() is called and framework is ready to run application specific code, ApplicationEvent::Started is sent.
    // You can perform initialization of your application and display first window to user here.
    if (event.Is<ApplicationEvent::Started>())
    {
        // Set up themes used in this application.
        // This will initialize attributes used in the theme at root attribute node.
        // Futurewalker's core libraries are style(theme)-agnostic, so you need to set up themes by yourself for each component library you use.
        ApplicationTheme::AddTheme(ThemeBrightness::Light, Shared<Lamp::Theme>::Make(ThemeBrightness::Light));
        ApplicationTheme::AddTheme(ThemeBrightness::Dark, Shared<Lamp::Theme>::Make(ThemeBrightness::Dark));

        // Set preferred locales to ResourceManager.
        // ResourceManager negotiates with resource definitions and pick best matching locales available.
        // Here we use system's preferred UI locales. Instead, you could use custom locale selection mechanism.
        ResourceManager::SetPreferredLocales(Locale::GetPreferredUILocales());

        // Load resource module of this application.
        // Resource object is a handle to a resource module with specific UUID.
        // You can reuse this object to avoid unnecessary UUID lookups.
        auto const resource = ResourceManager::GetResource(M::HelloWorld);

        // Loads localized string of "Hello, World!".
        // Actual values are defined in .xml files in Resource folder.
        auto const hello = resource->GetString(R::HelloWorld::Hello);

        AsyncFunction::SpawnFn([=, this] -> Async<void> {
            // Create main window with background color.
            auto window = Window::Make({});
            window->SetTitle(hello);
            window->SetFrameRect({0, 0, 500, 500});
            window->SetBackgroundColor(Lamp::Style::ColorSurface);

            // Create styled text view.
            // If you assign attribute value to a view, it will override the attribute inherited from parent.
            // In this case, we override theme's default value of font size and weight with specified values for the text view.
            // Template argument of SetValue takes static address of attribute info.
            // NOTE: We could use static reference instead of pointer to remove '&' in template parameter, but MSVC has a bug that prevent passing static reference.
            auto text = Lamp::TextView::MakeWithText(hello);
            AttributeNode::SetValue<&Lamp::TextViewStyle::FontSize>(*text, 42);
            AttributeNode::SetValue<&Lamp::TextViewStyle::FontWeight>(*text, Graphics::FontWeight::Bold());

            // Create layout view that centers the text view.
            // Layouts and controls are both derived from View. You can combine them to create your desired UI layout.
            auto align = AlignView::MakeWithContent(text);

            // Set layout view as content of window.
            window->SetContent(align);

            // Show window to user.
            window->SetVisible(true);

            // Wait until window is closed (asynchronous).
            co_await EventWaiter(*window).Wait<WindowEvent::Closed>();
            // Exit application.
            co_await Exit();
        }).Detach();
    }
    return false;
}
}

// SPDX-License-Identifier: MPL-2.0

#include <Futurewalker.Application.Application.hpp>
#include <Futurewalker.Application.ApplicationEvent.hpp>
#include <Futurewalker.Application.ApplicationTheme.hpp>
#include <Futurewalker.Application.Window.hpp>

#include <Futurewalker.Event.EventReceiver.hpp>
#include <Futurewalker.Event.EventWaiter.hpp>

#include <Futurewalker.Base.Environment.hpp>

#include <Futurewalker.Main.Main.hpp>

namespace Futurewalker
{
class App : public Application
{
public:
    static auto Make() -> Shared<App>
    {
        return Application::MakeDerived<App>();
    }

    App(PassKey<Application> key)
      : Application(key, ApplicationOptions {.identifier = u8"com.example.futurewalker.application"})
    {
    }

protected:
    auto Initialize() -> void override
    {
        EventReceiver::Connect(*this, *this, &App::ReceiveEvent);
    }

    auto ReceiveEvent(Event<>& event) -> Async<Bool>
    {
        if (event.Is<ApplicationEvent::Started>())
        {
            auto window = Window::Make({});
            window->SetFrameRect({{100, 100}, {500, 500}});
            window->SetVisible(true);

            FW_DEBUG_ASSERT(window->GetFrameRect().GetPosition() == Point<Vp>(100, 100));
            FW_DEBUG_ASSERT(window->GetFrameRect().GetSize() == Size<Vp>(500, 500));

            window->SetFrameRect({{150, 150}, {600, 600}});
            FW_DEBUG_ASSERT(window->GetFrameRect().GetPosition() == Point<Vp>(150, 150));
            FW_DEBUG_ASSERT(window->GetFrameRect().GetSize() == Size<Vp>(600, 600));

            window->SetVisible(false);
            FW_DEBUG_ASSERT(window->GetFrameRect().GetPosition() == Point<Vp>(150, 150));
            FW_DEBUG_ASSERT(window->GetFrameRect().GetSize() == Size<Vp>(600, 600));
            window->SetVisible(true);

            window->Minimize();
            FW_DEBUG_ASSERT(window->GetFrameRect().GetPosition() == Point<Vp>(150, 150));
            FW_DEBUG_ASSERT(window->GetFrameRect().GetSize() == Size<Vp>(600, 600));
            FW_DEBUG_ASSERT(window->IsVisible());

            window->SetFrameRect({{200, 200}, {700, 700}});
            FW_DEBUG_ASSERT(window->GetFrameRect().GetPosition() == Point<Vp>(200, 200));
            FW_DEBUG_ASSERT(window->GetFrameRect().GetSize() == Size<Vp>(700, 700));

            window->SetVisible(false);
            FW_DEBUG_ASSERT(window->GetFrameRect().GetPosition() == Point<Vp>(200, 200));
            FW_DEBUG_ASSERT(window->GetFrameRect().GetSize() == Size<Vp>(700, 700));
            FW_DEBUG_ASSERT(!window->IsVisible());

            window->SetVisible(true);
            FW_DEBUG_ASSERT(window->GetFrameRect().GetPosition() == Point<Vp>(200, 200));
            FW_DEBUG_ASSERT(window->GetFrameRect().GetSize() == Size<Vp>(700, 700));
            FW_DEBUG_ASSERT(window->IsVisible());

            window->Restore();
            FW_DEBUG_ASSERT(window->GetFrameRect().GetPosition() == Point<Vp>(200, 200));
            FW_DEBUG_ASSERT(window->GetFrameRect().GetSize() == Size<Vp>(700, 700));
            FW_DEBUG_ASSERT(window->IsVisible());

            window->Minimize();
            window->SetVisible(false);
            FW_DEBUG_ASSERT(!window->IsVisible());

            window->SetFrameRect({{200, 200}, {700, 700}});
            FW_DEBUG_ASSERT(window->GetFrameRect().GetPosition() == Point<Vp>(200, 200));
            FW_DEBUG_ASSERT(window->GetFrameRect().GetSize() == Size<Vp>(700, 700));
            FW_DEBUG_ASSERT(!window->IsVisible());

            window->Restore();
            FW_DEBUG_ASSERT(window->GetFrameRect().GetPosition() == Point<Vp>(200, 200));
            FW_DEBUG_ASSERT(window->GetFrameRect().GetSize() == Size<Vp>(700, 700));
            FW_DEBUG_ASSERT(!window->IsVisible());

            window->SetVisible(true);
            FW_DEBUG_ASSERT(window->GetFrameRect().GetPosition() == Point<Vp>(200, 200));
            FW_DEBUG_ASSERT(window->GetFrameRect().GetSize() == Size<Vp>(700, 700));

            window->Maximize();
            FW_DEBUG_ASSERT(window->GetFrameRect().GetPosition() != Point<Vp>(200, 200));
            FW_DEBUG_ASSERT(window->GetFrameRect().GetSize() != Size<Vp>(700, 700));

            window->SetFrameRect({{300, 300}, {800, 800}});
            FW_DEBUG_ASSERT(window->GetFrameRect().GetPosition() == Point<Vp>(300, 300));
            FW_DEBUG_ASSERT(window->GetFrameRect().GetSize() == Size<Vp>(800, 800));

            window->Restore();
            FW_DEBUG_ASSERT(window->GetFrameRect().GetPosition() == Point<Vp>(300, 300));
            FW_DEBUG_ASSERT(window->GetFrameRect().GetSize() == Size<Vp>(800, 800));
            window->Maximize();
            window->SetVisible(false);
            window->SetFrameRect({{400, 400}, {1000, 1000}});
            window->SetVisible(true);
            FW_DEBUG_ASSERT(window->GetFrameRect().GetPosition() == Point<Vp>(400, 400));
            FW_DEBUG_ASSERT(window->GetFrameRect().GetSize() == Size<Vp>(1000, 1000));

            window->Restore();
            FW_DEBUG_ASSERT(window->GetFrameRect().GetPosition() == Point<Vp>(400, 400));
            FW_DEBUG_ASSERT(window->GetFrameRect().GetSize() == Size<Vp>(1000, 1000));

            window->Maximize();
            window->SetVisible(false);
            window->SetRestoredFrameRect({{0, 0}, {500, 500}});
            window->Restore();
            window->SetVisible(true);
            window->Minimize();
            window->SetVisible(false);
            window->SetRestoredFrameRect({{500, 500}, {600, 600}});
            window->Restore();
            window->SetVisible(true);

            co_await EventWaiter(*window).Wait<WindowEvent::Closed>();
            co_await Exit();

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
        auto app = App::Make();
        co_await app->Run();
    }
    catch (...)
    {
        co_return ExitCode::Failure;
    }
    co_return ExitCode::Success;
}
}

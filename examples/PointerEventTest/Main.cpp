// SPDX-License-Identifier: MPL-2.0

#include <Futurewalker.Component.Lamp.TextView.hpp>
#include <Futurewalker.Component.Lamp.Theme.hpp>

#include <Futurewalker.Application.Application.hpp>
#include <Futurewalker.Application.ApplicationEvent.hpp>
#include <Futurewalker.Application.ApplicationTheme.hpp>
#include <Futurewalker.Application.Window.hpp>
#include <Futurewalker.Application.PointerEvent.hpp>
#include <Futurewalker.Application.FlexLayout.hpp>
#include <Futurewalker.Application.WindowFrame.hpp>

#include <Futurewalker.Event.EventReceiver.hpp>
#include <Futurewalker.Event.EventWaiter.hpp>

#include <Futurewalker.Base.Environment.hpp>
#include <Futurewalker.Base.Debug.hpp>

#include <Futurewalker.Main.Main.hpp>

namespace Futurewalker
{

class TestView : public View
{
public:
    static auto Make() -> Shared<TestView>
    {
        return View::MakeDerived<TestView>();
    }

    auto Initialize() -> void override
    {
        SetPointerTrackingFlags(ViewPointerTrackingFlags::All);
        EventReceiver::Connect(*this, *this, &TestView::ReceiveEvent);
    }

    TestView(PassKey<View> key)
      : View(key)
    {
    }

    auto ReceiveEvent(Event<>& event) -> Async<Bool>
    {
        if (event.Is<PointerEvent::Motion>())
        {
            if (_layout)
            {
                _layout->RemoveFromParent();
                _layout = {};
            }
            _layout = FlexLayout::Make();
            _layout->SetDirection(FlexLayoutDirection::Column);
            _layout->SetMainAxisAlignment(FlexLayoutMainAxisAlignment::Center);
            _layout->SetCrossAxisAlignment(FlexLayoutCrossAxisAlignment::Center);
            auto const& eventParameter = *event;
            _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"type: {}", String::MakeFromStdString(typeid(eventParameter).name()))));
            _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"id: {}", (UInt64)event.As<PointerEvent>()->GetPointerId())));
            _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"type: {}", std::to_underlying(event.As<PointerEvent>()->GetPointerType()))));
            _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"timestamp: {}", event.As<PointerEvent>()->GetTimestamp().GetIntervalSinceEpoch().GetIntNanoseconds())));
            _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"primary: {}", event.As<PointerEvent>()->IsPrimaryPointer() ? u8"true" : u8"false")));
            _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"position: ({}, {})", event.As<PointerEvent>()->GetPosition().x, event.As<PointerEvent>()->GetPosition().y)));

            if (event.Is<PointerEvent::Motion::Down>())
            {
                _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"tiltX: {}", event.As<PointerEvent::Motion::Down>()->GetTiltX())));
                _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"tiltY: {}", event.As<PointerEvent::Motion::Down>()->GetTiltY())));
                _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"azimuth: {}", event.As<PointerEvent::Motion::Down>()->GetAzimuth())));
                _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"altitude: {}", event.As<PointerEvent::Motion::Down>()->GetAltitude())));
                _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"pressure: {}", event.As<PointerEvent::Motion::Down>()->GetPressure())));
                _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"tangential_pressure: {}", event.As<PointerEvent::Motion::Down>()->GetTangentialPressure())));
                _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"button: {}", std::to_underlying(event.As<PointerEvent::Motion::Down>()->GetButton()))));

                if ((event.As<PointerEvent::Motion::Down>()->GetButtons() & PointerButtonFlags::Button1) != PointerButtonFlags::None)
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"button: Button1"));
                }
                if ((event.As<PointerEvent::Motion::Down>()->GetButtons() & PointerButtonFlags::Button2) != PointerButtonFlags::None)
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"button: Button2"));
                }
                if ((event.As<PointerEvent::Motion::Down>()->GetButtons() & PointerButtonFlags::Button3) != PointerButtonFlags::None)
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"button: Button3"));
                }
                if ((event.As<PointerEvent::Motion::Down>()->GetButtons() & PointerButtonFlags::ButtonX1) != PointerButtonFlags::None)
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"button: ButtonX1"));
                }
                if ((event.As<PointerEvent::Motion::Down>()->GetButtons() & PointerButtonFlags::ButtonX2) != PointerButtonFlags::None)
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"button: ButtonX2"));
                }

                if ((event.As<PointerEvent::Motion::Down>()->GetModifiers() & ModifierKeyFlags::Alt) != ModifierKeyFlags::None) 
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"modifier: Alt"));
                }
                if ((event.As<PointerEvent::Motion::Down>()->GetModifiers() & ModifierKeyFlags::Control) != ModifierKeyFlags::None) 
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"modifier: Control"));
                }
                if ((event.As<PointerEvent::Motion::Down>()->GetModifiers() & ModifierKeyFlags::Shift) != ModifierKeyFlags::None) 
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"modifier: Shift"));
                }
            }
            else if (event.Is<PointerEvent::Motion::Up>())
            {
                _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"tiltX: {}", event.As<PointerEvent::Motion::Up>()->GetTiltX())));
                _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"tiltY: {}", event.As<PointerEvent::Motion::Up>()->GetTiltY())));
                _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"azimuth: {}", event.As<PointerEvent::Motion::Up>()->GetAzimuth())));
                _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"altitude: {}", event.As<PointerEvent::Motion::Up>()->GetAltitude())));
                _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"pressure: {}", event.As<PointerEvent::Motion::Up>()->GetPressure())));
                _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"tangential_pressure: {}", event.As<PointerEvent::Motion::Up>()->GetTangentialPressure())));
                _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"button: {}", std::to_underlying(event.As<PointerEvent::Motion::Up>()->GetButton()))));

                if ((event.As<PointerEvent::Motion::Up>()->GetButtons() & PointerButtonFlags::Button1) != PointerButtonFlags::None)
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"button: Button1"));
                }
                if ((event.As<PointerEvent::Motion::Up>()->GetButtons() & PointerButtonFlags::Button2) != PointerButtonFlags::None)
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"button: Button2"));
                }
                if ((event.As<PointerEvent::Motion::Up>()->GetButtons() & PointerButtonFlags::Button3) != PointerButtonFlags::None)
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"button: Button3"));
                }
                if ((event.As<PointerEvent::Motion::Up>()->GetButtons() & PointerButtonFlags::ButtonX1) != PointerButtonFlags::None)
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"button: ButtonX1"));
                }
                if ((event.As<PointerEvent::Motion::Up>()->GetButtons() & PointerButtonFlags::ButtonX2) != PointerButtonFlags::None)
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"button: ButtonX2"));
                }
                
                if ((event.As<PointerEvent::Motion::Up>()->GetModifiers() & ModifierKeyFlags::Alt) != ModifierKeyFlags::None) 
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"modifier: Alt"));
                }
                if ((event.As<PointerEvent::Motion::Up>()->GetModifiers() & ModifierKeyFlags::Control) != ModifierKeyFlags::None) 
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"modifier: Control"));
                }
                if ((event.As<PointerEvent::Motion::Up>()->GetModifiers() & ModifierKeyFlags::Shift) != ModifierKeyFlags::None) 
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"modifier: Shift"));
                }
            }
            else if (event.Is<PointerEvent::Motion::Move>())
            {
                _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"tiltX: {}", event.As<PointerEvent::Motion::Move>()->GetTiltX())));
                _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"tiltY: {}", event.As<PointerEvent::Motion::Move>()->GetTiltY())));
                _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"azimuth: {}", event.As<PointerEvent::Motion::Move>()->GetAzimuth())));
                _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"altitude: {}", event.As<PointerEvent::Motion::Move>()->GetAltitude())));
                _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"pressure: {}", event.As<PointerEvent::Motion::Move>()->GetPressure())));
                _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"tangential_pressure: {}", event.As<PointerEvent::Motion::Move>()->GetTangentialPressure())));
                _layout->AddChild(Lamp::TextView::MakeWithText(StringFunction::Format(u8"button: {}", std::to_underlying(event.As<PointerEvent::Motion::Move>()->GetButton()))));

                if ((event.As<PointerEvent::Motion::Move>()->GetButtons() & PointerButtonFlags::Button1) != PointerButtonFlags::None)
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"button: Button1"));
                }
                if ((event.As<PointerEvent::Motion::Move>()->GetButtons() & PointerButtonFlags::Button2) != PointerButtonFlags::None)
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"button: Button2"));
                }
                if ((event.As<PointerEvent::Motion::Move>()->GetButtons() & PointerButtonFlags::Button3) != PointerButtonFlags::None)
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"button: Button3"));
                }
                if ((event.As<PointerEvent::Motion::Move>()->GetButtons() & PointerButtonFlags::ButtonX1) != PointerButtonFlags::None)
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"button: ButtonX1"));
                }
                if ((event.As<PointerEvent::Motion::Move>()->GetButtons() & PointerButtonFlags::ButtonX2) != PointerButtonFlags::None)
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"button: ButtonX2"));
                }

                if ((event.As<PointerEvent::Motion::Move>()->GetModifiers() & ModifierKeyFlags::Alt) != ModifierKeyFlags::None) 
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"modifier: Alt"));
                }
                if ((event.As<PointerEvent::Motion::Move>()->GetModifiers() & ModifierKeyFlags::Control) != ModifierKeyFlags::None) 
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"modifier: Control"));
                }
                if ((event.As<PointerEvent::Motion::Move>()->GetModifiers() & ModifierKeyFlags::Shift) != ModifierKeyFlags::None) 
                {
                    _layout->AddChild(Lamp::TextView::MakeWithText(u8"modifier: Shift"));
                }
            }

            AddChildBack(_layout);
            co_return true;
        }
        co_return false;
    }

    Shared<FlexLayout> _layout;
};

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
            ApplicationTheme::AddTheme(ThemeBrightness::Light, Shared<Lamp::Theme>::Make(ThemeBrightness::Light));
            ApplicationTheme::AddTheme(ThemeBrightness::Dark, Shared<Lamp::Theme>::Make(ThemeBrightness::Dark));
            auto window = Window::Make({});
            auto frame = WindowFrame::Make();
            frame->SetContent(TestView::Make());
            window->SetContent(frame);
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

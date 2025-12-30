Futurewalker is a (not yet) cross-platform, retained-mode GUI application framework written in C++23.  

It is primarily designed for developing graphics tools.

**Futurewalker is currently in early development phase, thus not production ready by any means.**   

If you have interest in the project, please try it out and provide feedback!

### Supported platforms 

| OS | Support Status |
| ---- | ---- |
| Windows 10 | ❔ Undecided |
| Windows 11 | 🚧 In Development |
| macOS | 📝 Planned for 2026~ |
| iOS | 📝 Planned for 2027~ |
| Android | 📝 Planned for 2027~ |
| Linux | ❔ Undecided |
| Web | ❔ Undecided |

### Building

> NOTE: This is for building tests and examples in this repository.   
> There's no proper way to develop applications yet. 

1. Install Conan package manager

```
pip install conan
```

2. Install Bazel build tool

```
winget install Bazel
```

3. Setup local package for Skia

```
git clone https://github.com/mocabe/conan-skia.git
conan remote add conan_skia ./conan-skia
```

4. Build the project

```
git clone https://github.com/mocabe/futurewalker.git
cd futurewalker
conan build . -pr:h=./conan/vs2022_x86_64_Debug.profile --build=missing
```

## Hello, World!

```cpp
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
```

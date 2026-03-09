#pragma once

#include <Futurewalker.Application.Application.hpp>
#include <Futurewalker.Application.Menu.hpp>
#include <Futurewalker.Application.WindowType.hpp>

#include <Futurewalker.Action.CommandDispatcherType.hpp>

namespace Futurewalker::ComponentGallery
{
class ComponentGalleryApplication : public Application
{
public:
    static auto Make() -> Shared<ComponentGalleryApplication>;

    ComponentGalleryApplication(PassKey<Application> key);

protected:
    auto Initialize() -> void override;
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;
    auto ReceiveCommandEvent(Event<>& event) -> Async<Bool>;
    auto BuildMainMenu() const -> Menu;

private:
    Unique<CommandDispatcher> _commandDispatcher;
    Shared<Window> _window;
};
}

// SPDX-License-Identifier: MPL-2.0 OR CC0-1.0
#pragma once

#include <Futurewalker.Application.Application.hpp>
#include <Futurewalker.Application.Menu.hpp>
#include <Futurewalker.Application.WindowType.hpp>

#include <Futurewalker.Action.CommandDispatcherType.hpp>

namespace Futurewalker::Gallery
{
class Application : public Futurewalker::Application
{
public:
    static auto Make() -> Shared<Application>;

    Application(PassKey<Futurewalker::Application> key);

protected:
    auto Initialize() -> void override;
    auto ReceiveEvent(Event<>& event) -> Bool;
    auto ReceiveCommandEvent(Event<>& event) -> Bool;
    auto BuildMainMenu() const -> Menu;

private:
    Unique<CommandDispatcher> _commandDispatcher;
    Shared<Window> _window;
};
}

// SPDX-License-Identifier: MPL-2.0
#pragma once

#include <Futurewalker.Application.Application.hpp>

namespace Futurewalker::HelloWorld
{
class Application final : public Futurewalker::Application
{
public:
    static auto Make() -> Shared<Application>;

    Application(PassKey<Futurewalker::Application> key);

protected:
    auto Initialize() -> void override;
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;
};
}

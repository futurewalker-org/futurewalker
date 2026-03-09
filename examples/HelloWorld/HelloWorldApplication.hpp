// SPDX-License-Identifier: MPL-2.0
#pragma once

#include <Futurewalker.Application.Application.hpp>

namespace Futurewalker::HelloWorld
{
class HelloWorldApplication : public Application
{
public:
    static auto Make() -> Shared<HelloWorldApplication>;

    HelloWorldApplication(PassKey<Application> key);

protected:
    auto Initialize() -> void override;
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;
};
}

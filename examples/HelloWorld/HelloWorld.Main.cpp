// SPDX-License-Identifier: MPL-2.0

#include "HelloWorld.Application.hpp"

#include <Futurewalker.Base.Environment.hpp>

#include <Futurewalker.Main.Main.hpp>

namespace Futurewalker
{
auto Main() -> Async<ExitCode>
{
    try
    {
        auto env = Environment();
        auto app = HelloWorld::Application::Make();
        co_await Application::Run(std::move(app));
    }
    catch (...)
    {
        co_return ExitCode::Failure;
    }
    co_return ExitCode::Success;
}
}

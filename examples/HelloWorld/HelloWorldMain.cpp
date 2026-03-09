// SPDX-License-Identifier: MPL-2.0

#include "HelloWorldApplication.hpp"

#include <Futurewalker.Base.Environment.hpp>

#include <Futurewalker.Main.Main.hpp>

namespace Futurewalker
{
auto Main() -> Async<ExitCode>
{
    try
    {
        auto env = Environment();
        auto app = HelloWorld::HelloWorldApplication::Make();
        co_await app->Run();
    }
    catch (...)
    {
        co_return ExitCode::Failure;
    }
    co_return ExitCode::Success;
}
}

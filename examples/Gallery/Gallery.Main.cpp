// SPDX-License-Identifier: MPL-2.0 OR CC0-1.0
#include "Gallery.Application.hpp"

#include <Futurewalker.Base.Environment.hpp>

#include <Futurewalker.Main.Main.hpp>

#include <Futurewalker.Base.Debug.hpp>

namespace Futurewalker
{
auto Main() -> Async<ExitCode>
{
    try
    {
        auto env = Environment();
        auto app = Gallery::Application::Make();
        co_await app->Run();
    }
    catch (...)
    {
        co_return ExitCode::Failure;
    }
    co_return ExitCode::Success;
}
}

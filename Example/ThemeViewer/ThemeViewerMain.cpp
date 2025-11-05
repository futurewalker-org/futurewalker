// SPDX-License-Identifier: MPL-2.0

#include "ThemeViewerApplication.hpp"

#include <Futurewalker.Base.Environment.hpp>
#include <Futurewalker.Base.Debug.hpp>

#include <Futurewalker.Main.Main.hpp>

namespace Futurewalker
{
auto Main() -> Async<ExitCode>
{
    try
    {
        auto env = Environment();
        auto app = ThemeViewer::ThemeViewerApplication::Make();
        co_await app->Run();
    }
    catch (...)
    {
        co_return ExitCode::Failure;
    }
    co_return ExitCode::Success;
}
}

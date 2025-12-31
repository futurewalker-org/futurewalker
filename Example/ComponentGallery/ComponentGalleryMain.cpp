// SPDX-License-Identifier: MPL-2.0

#include "ComponentGalleryApplication.hpp"

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
        auto app = ComponentGallery::ComponentGalleryApplication::Make();
        co_await app->Run();
    }
    catch (...)
    {
        co_return ExitCode::Failure;
    }
    co_return ExitCode::Success;
}
}

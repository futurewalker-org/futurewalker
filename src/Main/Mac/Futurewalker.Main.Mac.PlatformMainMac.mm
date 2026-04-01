// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Main.Mac.PlatformMainMac.hpp"
#include "Futurewalker.Main.Mac.PlatformExitCodeMac.hpp"
#include "Futurewalker.Main.Main.hpp"
#include "Futurewalker.Application.Mac.PlatformInputMethodContextMac.hpp"

#include "Futurewalker.Base.Mac.PlatformEnvironmentContextMac.hpp"
#include "Futurewalker.Base.Mac.PlatformCommandLineMac.hpp"
#include "Futurewalker.Base.Mac.PlatformSystemInfoMac.hpp"
#include "Futurewalker.Base.Mac.PlatformDebugMac.hpp"
#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Graphics.Mac.PlatformSkiaFontManagerMac.hpp"

#include "Futurewalker.Async.Async.hpp"
#include "Futurewalker.Async.AsyncFunction.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Register platform singletons.
///
auto PlatformMainMac::Register() -> void
{
    Locator::Register<PlatformEnvironmentContextMac>();
    Locator::Register<PlatformCommandLineMac>();
    Locator::Register<PlatformSystemInfoMac>();
    Locator::Register<PlatformDebugMac>();
    Locator::Register<PlatformInputMethodContextMac>();
    Locator::Register<Graphics::PlatformSkiaFontManagerMac>();
}

///
/// @brief Call Main().
///
auto PlatformMainMac::Main() -> ExitCode
{
    @autoreleasepool
    {
        // On macOS, we just synchronously call Main() without creating event loop.
        // This is because Application::Run() will create its own event loop there.
        auto const main = []() -> Async<ExitCode> { co_return co_await FW_NS::Main(); };
        if (auto const exitCode = AsyncFunction::SpawnFn(main).SyncWait())
        {
            return *exitCode;
        }
        return ExitCode::Failure;
    }
}
}

///
/// @brief
///
/// @param argc
/// @param argv
///
/// @return
///
auto main(int argc, char* argv[]) -> int
{
    (void)argc;
    (void)argv;

    using namespace FW_NS;

    PlatformMainMac::Register();

    return PlatformExitCodeMac::GetPlatformExitCode(PlatformMainMac::Main());
}

// SPDX-License-Identifier: MIT

#include "Futurewalker.Main.Win.Prelude.hpp"
#include "Futurewalker.Main.Win.PlatformExitCodeWin.hpp"
#include "Futurewalker.Main.Main.hpp"

#include "Futurewalker.Base.Win.PlatformEnvironmentContextWin.hpp"
#include "Futurewalker.Base.Win.PlatformInstanceHandleWin.hpp"
#include "Futurewalker.Base.Win.PlatformCommandLineWin.hpp"
#include "Futurewalker.Base.Win.PlatformSystemInfoWin.hpp"
#include "Futurewalker.Base.Win.PlatformDebugWin.hpp"
#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Application.Win.PlatformApplicationContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformEventLoopContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformWindowContextWin.hpp"
#include "Futurewalker.Application.Win.PlatformMainThreadWin.hpp"
#include "Futurewalker.Application.Win.PlatformDCompositionDeviceWin.hpp"
#include "Futurewalker.Application.Win.PlatformViewLayercontextWin.hpp"
#include "Futurewalker.Application.Win.PlatformDrawableViewLayerContextWin.hpp"

#include "Futurewalker.Graphics.Win.PlatformD3D11DeviceWin.hpp"
#include "Futurewalker.Graphics.Win.PlatformD3D12DeviceWin.hpp"

#include "Futurewalker.Async.AsyncFunction.hpp"

///
/// @brief Main entry point of GUI based windows application.
///
/// @param hInst A handle to the current instance of the application.
/// @param hInstPrev A handle to the previous instance of the application. This parameter is always NULL.
/// @param cmdline The command line for the application.
/// @param cmdshow Controls how the window is to be shown.
///
/// @return
///
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    (void)hPrevInstance; // Always NULL.
    (void)lpCmdLine;     // Ignored. PlatformCommandLineWin handles arguments by itself.
    (void)nCmdShow;      // Ignored.

    using namespace FW_NS;

    Locator::Register<PlatformEnvironmentContextWin>();
    Locator::Register<PlatformInstanceHandleWin>(hInstance);
    Locator::Register<PlatformCommandLineWin>();
    Locator::Register<PlatformSystemInfoWin>();
    Locator::Register<PlatformDebugWin>();
    Locator::Register<PlatformMainThreadWin>();
    Locator::Register<PlatformApplicationContextWin>();
    Locator::Register<PlatformEventLoopContextWin>();
    Locator::Register<PlatformWindowContextWin>();
    Locator::Register<PlatformD3D11DeviceWin>();
    Locator::Register<PlatformD3D12DeviceWin>();
    Locator::Register<PlatformDCompositionDeviceWin>();
    Locator::Register<PlatformViewLayerContextWin>();
    Locator::Register<PlatformDrawableViewLayerContextWin>();

    auto main = []() -> Async<ExitCode> { co_return co_await Main(); };

    // On Windows, we just synchronously call Main() without creating event loop.
    // This is because Application::Run() will create its own event loop there.
    const auto exitCode = AsyncFunction::SpawnFn(main).Wait();

    return PlatformExitCodeWin::GetPlatformExitCode(exitCode ? *exitCode : ExitCode::Failure);
}

// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Main.Prelude.hpp"
#include "Futurewalker.Main.ExitCode.hpp"

#include "Futurewalker.Async.Async.hpp"

#include "Futurewalker.Core.Primitive.hpp"

namespace FW_NS
{
///
/// @brief Main entry point of application.
///
/// This function should be a coroutine instead of regular function.
/// This is because, on some platforms (Android, Web), applications do not have control over event loop.
///
/// In this function you should await Application::Run() and return result when it finished.
///
auto Main() -> Async<ExitCode>;
}

// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Main.Win.PlatformExitCodeWin.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Convert exit code to platform exit code.
///
/// @param exitCode Exit code
///
auto PlatformExitCodeWin::GetPlatformExitCode(const ExitCode& exitCode) -> int
{
    switch (exitCode)
    {
        case ExitCode::Success:
            return 0;
        case ExitCode::Failure:
            return 1;
    }
    return 1;
}
}

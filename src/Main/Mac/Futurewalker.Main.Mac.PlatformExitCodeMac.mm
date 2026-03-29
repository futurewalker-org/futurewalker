// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Main.Mac.PlatformExitCodeMac.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Convert exit code to platform exit code.
///
/// @param exitCode Exit code
///
auto PlatformExitCodeMac::GetPlatformExitCode(const ExitCode& exitCode) -> int
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

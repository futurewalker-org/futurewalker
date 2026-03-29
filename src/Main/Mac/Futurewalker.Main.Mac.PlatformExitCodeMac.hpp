// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Main.Mac.Prelude.hpp"

#include "Futurewalker.Main.ExitCode.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
struct PlatformExitCodeMac : NonConstructible
{
    static auto GetPlatformExitCode(const ExitCode& exitCode) -> int;
};
}
}

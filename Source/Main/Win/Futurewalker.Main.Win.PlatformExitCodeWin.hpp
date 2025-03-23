// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Main.Win.Prelude.hpp"

#include "Futurewalker.Main.ExitCode.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PlatformExitCodeWin : NonConstructible
{
public:
    static auto GetPlatformExitCode(const ExitCode& exitCode) -> int;
};
}
}

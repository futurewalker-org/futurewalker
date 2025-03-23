// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Base.CommandLineType.hpp"
#include "Futurewalker.Base.PlatformCommandLineType.hpp"
#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Command line utility functions.
///
class CommandLine : NonConstructible
{
public:
    static auto GetArguments() -> std::vector<String>;
};
}
}

// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Base.PlatformEnvironmentContextType.hpp"
#include "Futurewalker.Base.PlatformDebug.hpp"
#include "Futurewalker.Base.PlatformSystemInfo.hpp"
#include "Futurewalker.Base.PlatformCommandLine.hpp"

#include "Futurewalker.Core.Memory.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Platform environment context.
///
class PlatformEnvironmentContext
{
public:
    PlatformEnvironmentContext(Shared<PlatformDebug> debug, Shared<PlatformSystemInfo> systemInfo, Shared<PlatformCommandLine> commandLine);

    virtual ~PlatformEnvironmentContext() = 0;

private:
    Shared<PlatformDebug> _debug;
    Shared<PlatformSystemInfo> _systemInfo;
    Shared<PlatformCommandLine> _commandLine;
};
}
}

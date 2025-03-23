// SPDX-License-Identifier: MIT

#include "Futurewalker.Base.PlatformEnvironmentContext.hpp"
#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
PlatformEnvironmentContext::PlatformEnvironmentContext(Shared<PlatformDebug> debug, Shared<PlatformSystemInfo> systemInfo, Shared<PlatformCommandLine> commandLine)
  : _debug {debug}
  , _systemInfo {systemInfo}
  , _commandLine {commandLine}
{
}

PlatformEnvironmentContext::~PlatformEnvironmentContext() = default;
}

// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.PlatformEnvironmentContext.hpp"
#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
PlatformEnvironmentContext::PlatformEnvironmentContext(
  Shared<PlatformDebug> debug,
  Shared<PlatformSystemInfo> systemInfo,
  Shared<PlatformCommandLine> commandLine,
  Shared<PlatformFilesystem> filesystem)
  : _debug {debug}
  , _systemInfo {systemInfo}
  , _commandLine {commandLine}
  , _filesystem {filesystem}
{
}

PlatformEnvironmentContext::~PlatformEnvironmentContext() = default;
}

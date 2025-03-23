// SPDX-License-Identifier: MIT

#include "Futurewalker.Base.Win.PlatformEnvironmentContextWin.hpp"
#include "Futurewalker.Base.Win.PlatformInstanceHandleWin.hpp"

#include "Futurewalker.Base.PlatformEnvironmentContext.hpp"
#include "Futurewalker.Base.Locator.hpp"

#include <windows.h>

namespace FW_DETAIL_NS
{
///
/// @brief Constructor.
///
PlatformEnvironmentContextWin::PlatformEnvironmentContextWin(
  Shared<PlatformDebug> debug,
  Shared<PlatformSystemInfo> systemInfo,
  Shared<PlatformCommandLine> commandLine,
  Shared<PlatformInstanceHandleWin> instanceHandle)
  : PlatformEnvironmentContext(debug, systemInfo, commandLine)
  , _instanceHandle {instanceHandle}
{
}

///
/// @brief Resolve dependency.
///
auto Locator::Resolver<PlatformEnvironmentContextWin>::Resolve() -> Shared<PlatformEnvironmentContextWin>
{
    auto debug = Locator::Resolve<PlatformDebug>();
    auto systemInfo = Locator::Resolve<PlatformSystemInfo>();
    auto commandLine = Locator::Resolve<PlatformCommandLine>();
    auto instanceHandle = Locator::ResolveWithDefault<PlatformInstanceHandleWin>();
    return Shared<PlatformEnvironmentContextWin>::Make(debug, systemInfo, commandLine, instanceHandle);
}
}

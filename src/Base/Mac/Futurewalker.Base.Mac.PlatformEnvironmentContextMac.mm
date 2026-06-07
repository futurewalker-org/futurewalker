// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Base.Mac.PlatformEnvironmentContextMac.hpp"

#include "Futurewalker.Base.PlatformEnvironmentContext.hpp"
#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
PlatformEnvironmentContextMac::PlatformEnvironmentContextMac(
  Shared<PlatformDebug> debug,
  Shared<PlatformSystemInfo> systemInfo,
  Shared<PlatformCommandLine> commandLine,
  Shared<PlatformFilesystem> filesystem,
  Shared<PlatformLocaleContext> localeContext)
  : PlatformEnvironmentContext(debug, systemInfo, commandLine, filesystem, localeContext)
{
}

auto Locator::Resolver<PlatformEnvironmentContextMac>::Resolve() -> Shared<PlatformEnvironmentContextMac>
{
    auto debug = Locator::Resolve<PlatformDebug>();
    auto systemInfo = Locator::Resolve<PlatformSystemInfo>();
    auto commandLine = Locator::Resolve<PlatformCommandLine>();
    auto filesystem = Locator::Resolve<PlatformFilesystem>();
    auto localeContext = Locator::Resolve<PlatformLocaleContext>();
    return Shared<PlatformEnvironmentContextMac>::Make(debug, systemInfo, commandLine, filesystem, localeContext);
}
}

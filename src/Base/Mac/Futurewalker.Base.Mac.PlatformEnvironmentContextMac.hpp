// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Base.PlatformEnvironmentContext.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.Memory.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief macOS implementation of PlatformEnvironmentContext.
///
class PlatformEnvironmentContextMac final : public PlatformEnvironmentContext
{
public:
    PlatformEnvironmentContextMac(Shared<PlatformDebug> debug, Shared<PlatformSystemInfo> systemInfo, Shared<PlatformCommandLine> commandLine);
};

template <>
struct Locator::Resolver<PlatformEnvironmentContextMac>
{
    using Interface = PlatformEnvironmentContext;
    static auto Resolve() -> Shared<PlatformEnvironmentContextMac>;
};
}
}

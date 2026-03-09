// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Base.PlatformEnvironmentContext.hpp"

#include "Futurewalker.Base.Win.PlatformInstanceHandleWin.hpp"

#include "Futurewalker.Base.Locator.hpp"

#include "Futurewalker.Core.Memory.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Win32 implementation of PlatformEnvironmentContext.
///
class PlatformEnvironmentContextWin final : public PlatformEnvironmentContext
{
public:
    PlatformEnvironmentContextWin(Shared<PlatformDebug> debug, Shared<PlatformSystemInfo> systemInfo, Shared<PlatformCommandLine> commandLine, Shared<PlatformInstanceHandleWin> instanceHandle);

private:
    Shared<PlatformInstanceHandleWin> _instanceHandle;
};

template <>
struct Locator::Resolver<PlatformEnvironmentContextWin>
{
    using Interface = PlatformEnvironmentContext;
    static auto Resolve() -> Shared<PlatformEnvironmentContextWin>;
};
}
}

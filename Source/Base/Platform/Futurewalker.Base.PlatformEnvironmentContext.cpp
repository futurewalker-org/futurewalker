// SPDX-License-Identifier: MIT

#include "Futurewalker.Base.PlatformEnvironmentContext.hpp"
#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
PlatformEnvironmentContext::PlatformEnvironmentContext(Shared<PlatformDebug> debug)
  : _debug {debug}
{
}

PlatformEnvironmentContext::~PlatformEnvironmentContext() = default;
}

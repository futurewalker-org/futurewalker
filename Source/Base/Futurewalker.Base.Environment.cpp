// SPDX-License-Identifier: MIT

#include "Futurewalker.Base.Environment.hpp"
#include "Futurewalker.Base.PlatformEnvironmentContext.hpp"
#include "Futurewalker.Base.UniqueIdentifierManager.hpp"
#include "Futurewalker.Base.Debug.hpp"
#include "Futurewalker.Base.Locator.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Constructor.
///
Environment::Environment()
{
    _platformContext = Locator::Resolve<PlatformEnvironmentContext>();
    _identifierManager = Locator::ResolveWithDefault<UniqueIdentifierManager>();
}
}

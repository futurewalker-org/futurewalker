// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Base.PlatformEnvironmentContextType.hpp"
#include "Futurewalker.Base.PlatformDebug.hpp"

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
    PlatformEnvironmentContext(Shared<PlatformDebug> debug);

    virtual ~PlatformEnvironmentContext() = 0;

private:
    Shared<PlatformDebug> _debug;
};
}
}

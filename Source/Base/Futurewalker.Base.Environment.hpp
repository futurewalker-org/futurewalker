// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Base.EnvironmentType.hpp"
#include "Futurewalker.Base.PlatformDebug.hpp"
#include "Futurewalker.Base.PlatformEnvironmentContextType.hpp"
#include "Futurewalker.Base.UniqueIdentifierManagerType.hpp"

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.String.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Environment class.
///
class Environment : NonCopyable
{
public:
    Environment();

private:
    Shared<UniqueIdentifierManager> _identifierManager;
    Shared<PlatformEnvironmentContext> _platformContext;
};
}
}

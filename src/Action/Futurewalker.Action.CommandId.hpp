// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Action.Prelude.hpp"

#include "Futurewalker.Base.UniqueIdentifier.hpp"

#include "Futurewalker.Core.Identifier.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class Command;
using CommandId = Identifier<UInt64, Command>;
using UniqueCommandId = UniqueIdentifier<Command>;
}
}

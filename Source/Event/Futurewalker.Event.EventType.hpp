// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Event.Prelude.hpp"

#include "Futurewalker.Async.Async.hpp"

#include "Futurewalker.Core.Function.hpp"
#include "Futurewalker.Core.Primitive.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class Event;
using EventFunction = Function<Async<Bool>(Event& event)>;
}
}

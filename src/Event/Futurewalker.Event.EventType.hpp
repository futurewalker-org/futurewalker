// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Event.EventParameterType.hpp"

#include "Futurewalker.Async.Async.hpp"

#include "Futurewalker.Core.Function.hpp"
#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Concepts.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
template <Concepts::DerivedFrom<EventParameter> Parameter = EventParameter>
class Event;

using EventFunction = Function<Async<Bool>(Event<>& event)>;
}
}

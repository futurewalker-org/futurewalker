// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Event.EventType.hpp"
#include "Futurewalker.Signal.Signal.hpp"
#include "Futurewalker.Signal.SignalCombiner.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Signal combiner used for event signals.
///
template <class T>
using EventSignalCombiner = AsyncSignalCombinerAnyOf<T>;

///
/// @brief A signal type used for event connections.
///
using EventSignal = Signal<Lazy<Bool>(Event&), EventSignalCombiner>;
}
}

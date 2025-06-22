// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PointerParameterType.hpp"
#include "Futurewalker.Application.PointerEvent.hpp" 
#include "Futurewalker.Application.PointerScopeType.hpp"
#include "Futurewalker.Application.ViewType.hpp"

#include "Futurewalker.Event.Event.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.MonotonicTime.hpp"
#include "Futurewalker.Core.PassKey.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Pointer parameter.
///
class PointerParameter
{
public:
    auto GetTargetView(PassKey<View>) const -> Shared<View>;
    auto SetTargetView(Shared<View> const& targetView) -> void;

    auto GetPhaseFlags(PassKey<View>) const -> PointerPhaseFlags;
    auto SetPhaseFlags(PointerPhaseFlags const flags) -> void;

    auto GetPointerEvent() const -> Event<PointerEvent> const&;
    auto SetPointerEvent(Event<PointerEvent> const& pointerEvent) -> void;

private:
    Weak<View> _targetView;
    Event<PointerEvent> _pointerEvent;
    PointerPhaseFlags _phaseFlags = PointerPhaseFlags::None;
};
}
}

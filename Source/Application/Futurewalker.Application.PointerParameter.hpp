// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.PointerParameterType.hpp"
#include "Futurewalker.Application.PointerEvent.hpp" 
#include "Futurewalker.Application.PointerScopeType.hpp"
#include "Futurewalker.Application.ViewType.hpp"

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
    explicit PointerParameter(PointerEvent const& pointerEvent);

    auto GetTargetView(PassKey<View>) const -> Shared<View>;
    auto SetTargetView(Shared<View> const& targetView) -> void;

    auto GetPhaseFlags(PassKey<View>) const -> PointerEventPhaseFlags;
    auto SetPhaseFlags(PointerEventPhaseFlags const flags) -> void;

    auto GetPointerEvent() const -> PointerEvent const&;
    auto SetPointerEvent(PointerEvent const& pointerEvent) -> void;

private:
    Weak<View> _targetView;
    PointerEvent _pointerEvent;
    PointerEventPhaseFlags _phaseFlags = PointerEventPhaseFlags::None;
};
}
}

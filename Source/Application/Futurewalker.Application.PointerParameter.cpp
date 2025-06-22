// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PointerParameter.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
auto PointerParameter::GetTargetView(PassKey<View>) const -> Shared<View>
{
    return _targetView.Lock();
}

///
/// @brief
///
/// @param targetView
///
auto PointerParameter::SetTargetView(Shared<View> const& targetView) -> void
{
    _targetView = targetView;
}

///
/// @brief
///
auto PointerParameter::GetPhaseFlags(PassKey<View>) const -> PointerPhaseFlags
{
    return _phaseFlags;
}

///
/// @brief
///
auto PointerParameter::SetPhaseFlags(PointerPhaseFlags const flags) -> void
{
    _phaseFlags = flags;
}

///
/// @brief
///
auto PointerParameter::GetPointerEvent() const -> Event<PointerEvent> const&
{
    return _pointerEvent;
}

///
/// @brief
///
auto PointerParameter::SetPointerEvent(Event<PointerEvent> const& pointerEvent) -> void
{
    _pointerEvent = pointerEvent;
}
}

// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.Win.Prelude.hpp"
#include "Futurewalker.Application.PlatformPointerEvent.hpp"

#include "Futurewalker.Core.NonConstructible.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
struct PlatformPointerEventFunctionWin : public NonConstructible
{
    static auto ConvertMouseEventParamToPointerButtonFlags(WPARAM const wParam) -> Flags<PointerButtonFlag>;
    static auto ConvertMouseEventParamToModifierKeyFlags(WPARAM const wParam) -> Flags<ModifierKeyFlag>;

    static auto ConvertPointerButtonChangeTypeToPointerButton(UINT32 const pointerChange) -> PointerButton; 
    static auto ConvertPointerFlagsToPointerButtonFlags(UINT32 const pointerFlags) -> Flags<PointerButtonFlag>;

    static auto SetPointerEventParamsForMouse(PlatformPointerEvent& parameter, HWND hWnd, UINT const msg, WPARAM const wParam, LPARAM const lParam) -> void;
    static auto SetPointerMotionEventParamsForMouse(PlatformPointerEvent::Motion& parameter, UINT const msg, WPARAM const wParam, LPARAM const lParam) -> void;
    static auto SetPointerScrollEventParamsForMouse(PlatformPointerEvent::Action::Scroll& parameter, UINT const msg, WPARAM const wParam, LPARAM const lParam) -> void;

    static auto SetPointerEventParamsForPointer(PlatformPointerEvent& parameter, UINT32 const pointerId, HWND const hwnd) -> void;
    static auto SetPointerMotionEventParamsForPointer(PlatformPointerEvent::Motion& parameter, UINT32 const pointerId) -> void;
};
}
}

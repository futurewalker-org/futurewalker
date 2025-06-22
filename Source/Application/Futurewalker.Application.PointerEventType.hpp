// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Event.hpp"

#include "Futurewalker.Core.Identifier.hpp"
#include "Futurewalker.Core.EnumClassFlag.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PointerEvent;
using PointerId = Identifier<UInt64, PointerEvent>;

enum class PointerType
{
    Other = 0,
    Mouse,
    Pen,
    Touch,
    TouchPad,
};

enum class PointerGesturePhase
{
    None = 0,
    Begin,
    Update,
    End,
    Cancel,
};

enum class PointerWheelPrecision
{
    Coarse,
    Precise,
};

enum class PointerButton
{
    Unknown = 0,
    Button1, // Primary button.
    Button2, // Secondary button.
    Button3, // Tertiary button.
    ButtonX1,
    ButtonX2,
    Barrel,
    Eraser,
};

enum class PointerButtonFlags : uint32_t
{
    None = 0,
    Button1 = 1 << 1,  // Mouse left button or pen tip.
    Button2 = 1 << 2,  // Mouse right button.
    Button3 = 1 << 3,  // Mouse middle button.
    ButtonX1 = 1 << 4, // Mouse extended button 1.
    ButtonX2 = 1 << 5, // Mouse extended button 2.
    Barrel = 1 << 6,   // Pen barrel button.
    Eraser = 1 << 7,   // Pen eraser button.
};
FW_ENABLE_ENUM_CLASS_BITWISE_OPERATORS(PointerButtonFlags);

enum class PointerPhaseFlags : uint32_t
{
    None = 0,
    Capture = 1 << 0, //
    Bubble = 1 << 1,  //
    Target = 1 << 2,  //
};
FW_ENABLE_ENUM_CLASS_BITWISE_OPERATORS(PointerPhaseFlags);
}
}

// SPDX-License-Identifier: MIT
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

enum class PointerEventType
{
    Down,
    Up,
    Enter,
    Leave,
    Over,
    Out,
    Move,
    Cancel,
};

enum class PointerType
{
    Other = 0,
    Mouse,
    Pen,
    Touch,
    TouchPad,
};

enum class PointerButtonFlags : uint32_t
{
    None = 0,
    Button1  = 1 << 0, // Mouse left button or pen tip.
    Button2  = 1 << 1, // Mouse right button.
    Button3  = 1 << 2, // Mouse middle button.
    ButtonX1 = 1 << 3, // Mouse extended button 1.
    ButtonX2 = 1 << 4, // Mouse extended button 2.
    Barrel   = 1 << 5, // Pen barrel button.
    Eraser   = 1 << 6, // Pen eraser button.
};
FW_ENABLE_ENUM_CLASS_BITWISE_OPERATORS(PointerButtonFlags);


enum class PointerEventPhaseFlags : uint32_t
{
    None = 0,
    Capture = 1 << 0, //
    Bubble  = 1 << 1, // 
    Target  = 1 << 2, //
};
FW_ENABLE_ENUM_CLASS_BITWISE_OPERATORS(PointerEventPhaseFlags);
}
}

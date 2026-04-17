// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Event.hpp"

#include "Futurewalker.Core.Identifier.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
class PointerEvent;
using PointerId = Identifier<UInt64, PointerEvent>;

///
/// @brief Type of pointer device.
///
enum class PointerType
{
    Other = 0,
    Mouse,
    Pen,
    Touch,
    TouchPad,
};

///
/// @brief Phase of pointer event propagation.
///
enum class PointerPhaseFlag
{
    None = 0,
    Capture = 1 << 0, ///< Event being sent during propagation from root to target.
    Bubble = 1 << 1,  //!< Event being sent during propagation from target to root.
    Target = 1 << 2,  //!< Event being sent to target.
};

///
/// @brief Phase of pointer actions.
///
enum class PointerActionPhase
{
    None = 0, ///< Action does not have a phase, e.g. discrete actions like mouse wheel scroll.
    Begin,    ///< Action started.
    Update,   ///< Action updated.
    End,      ///< Action ended.
    Cancel,   ///< Action canceled.
};

///
/// @brief Precision of pointer scroll.
///
enum class PointerScrollPrecision
{
    Coarse,  ///< Coarse precision, e.g. mouse wheel.
    Precise, ///< Precise precision, e.g. high precision touchpad or trackpad.
};

///
/// @brief Pointer button.
///
enum class PointerButton
{
    Unknown = 0,
    Button1,  // Primary button.
    Button2,  // Secondary button.
    Button3,  // Tertiary button.
    ButtonX1, // Mouse extended button 1.
    ButtonX2, // Mouse extended button 2.
    Barrel,   // Pen barrel button.
    Eraser,   // Pen eraser button.
};

///
/// @brief Flags for pointer buttons.
///
enum class PointerButtonFlag : uint32_t
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
}
}

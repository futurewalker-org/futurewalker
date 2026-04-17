// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PointerEventType.hpp"
#include "Futurewalker.Application.KeyEventType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Event.Event.hpp"

#include "Futurewalker.Core.MonotonicTime.hpp"
#include "Futurewalker.Core.Flags.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Pointer event.
///
class PointerEvent : public EventParameter
{
public:
    static void Copy(PointerEvent& dst, PointerEvent const& src);

    auto GetPointerId() const -> PointerId;
    auto SetPointerId(PointerId const id) -> void;

    auto GetPointerType() const -> PointerType;
    auto SetPointerType(PointerType const type) -> void;

    auto GetTimestamp() const -> MonotonicTime;
    auto SetTimestamp(MonotonicTime const timestamp) -> void;

    auto IsPrimaryPointer() const -> Bool;
    auto SetPrimaryPointer(Bool const isPrimaryPointer) -> void;

    auto GetPosition() const -> Point<Dp>;
    auto SetPosition(Point<Dp> const& position) -> void;

public:
    class Motion;
    class Action;
    class Forecast;

private:
    PointerId _id = 0U;
    PointerType _type = PointerType::Other;
    MonotonicTime _timestamp;
    Bool _isPrimaryPointer = false;
    Point<Dp> _position;
};

///
/// @brief Base class for pointer motion events.
///
/// @note Motion events notify about pointer state changes, such as button presses, movement, etc.
///
class PointerEvent::Motion : public PointerEvent
{
    using PointerEvent::Copy;

public:
    class Down;
    class Up;
    class Move;
    class Cancel;
    class Enter;
    class Leave;
    class Out;
    class Over;

protected:
    /// Common state for motion events.
    struct MotionState
    {
        Point<Dp> position;
        Degree azimuth = 0;
        Degree altitude = 0;
        Degree twist = 0;
        Degree tiltX = 0;
        Degree tiltY = 0;
        Float64 pressure = 0;
        Float64 tangentialPressure = 0;
        PointerButton button = PointerButton::Unknown;
        Flags<PointerButtonFlag> buttonState = PointerButtonFlag::None;
        Flags<ModifierKeyFlag> modifierState = ModifierKeyFlag::None;
    };
};

///
/// @brief Pointer button down event.
///
class PointerEvent::Motion::Down final : public PointerEvent::Motion
{
public:
    auto GetTiltX() const -> Degree;
    auto SetTiltX(Degree const tiltX) -> void;

    auto GetTiltY() const -> Degree;
    auto SetTiltY(Degree const tiltY) -> void;

    auto GetAzimuth() const -> Degree;
    auto SetAzimuth(Degree const azimuth) -> void;

    auto GetAltitude() const -> Degree;
    auto SetAltitude(Degree const altitude) -> void;

    auto GetTwist() const -> Degree;
    auto SetTwist(Degree const twist) -> void;

    auto GetPressure() const -> Float64;
    auto SetPressure(Float64 const pressure) -> void;

    auto GetTangentialPressure() const -> Float64;
    auto SetTangentialPressure(Float64 const pressure) -> void;

    auto GetButton() const -> PointerButton;
    auto SetButton(PointerButton const button) -> void;

    auto GetButtons() const -> Flags<PointerButtonFlag>;
    auto SetButtons(Flags<PointerButtonFlag> const buttons) -> void;

    auto GetModifiers() const -> Flags<ModifierKeyFlag>;
    auto SetModifiers(Flags<ModifierKeyFlag> const modifiers) -> void;

private:
    MotionState _state;
};

///
/// @brief Pointer button up event.
///
class PointerEvent::Motion::Up final : public PointerEvent::Motion
{
public:
    auto GetTiltX() const -> Degree;
    auto SetTiltX(Degree const tiltX) -> void;

    auto GetTiltY() const -> Degree;
    auto SetTiltY(Degree const tiltY) -> void;

    auto GetAzimuth() const -> Degree;
    auto SetAzimuth(Degree const azimuth) -> void;

    auto GetAltitude() const -> Degree;
    auto SetAltitude(Degree const altitude) -> void;

    auto GetTwist() const -> Degree;
    auto SetTwist(Degree const twist) -> void;

    auto GetPressure() const -> Float64;
    auto SetPressure(Float64 const pressure) -> void;

    auto GetTangentialPressure() const -> Float64;
    auto SetTangentialPressure(Float64 const pressure) -> void;

    auto GetButton() const -> PointerButton;
    auto SetButton(PointerButton const button) -> void;

    auto GetButtons() const -> Flags<PointerButtonFlag>;
    auto SetButtons(Flags<PointerButtonFlag> const buttons) -> void;

    auto GetModifiers() const -> Flags<ModifierKeyFlag>;
    auto SetModifiers(Flags<ModifierKeyFlag> const modifiers) -> void;

private:
    MotionState _state;
};

///
/// @brief Pointer movement event.
///
class PointerEvent::Motion::Move final : public PointerEvent::Motion
{
public:
    auto GetTiltX() const -> Degree;
    auto SetTiltX(Degree const tiltX) -> void;

    auto GetTiltY() const -> Degree;
    auto SetTiltY(Degree const tiltY) -> void;

    auto GetAzimuth() const -> Degree;
    auto SetAzimuth(Degree const azimuth) -> void;

    auto GetAltitude() const -> Degree;
    auto SetAltitude(Degree const altitude) -> void;

    auto GetTwist() const -> Degree;
    auto SetTwist(Degree const twist) -> void;

    auto GetPressure() const -> Float64;
    auto SetPressure(Float64 const pressure) -> void;

    auto GetTangentialPressure() const -> Float64;
    auto SetTangentialPressure(Float64 const pressure) -> void;

    auto GetButton() const -> PointerButton;
    auto SetButton(PointerButton const button) -> void;

    auto GetButtons() const -> Flags<PointerButtonFlag>;
    auto SetButtons(Flags<PointerButtonFlag> const buttons) -> void;

    auto GetModifiers() const -> Flags<ModifierKeyFlag>;
    auto SetModifiers(Flags<ModifierKeyFlag> const modifiers) -> void;

private:
    MotionState _state;
};

///
/// @brief Pointer cancel event.
///
/// @note This event is sent when the pointer input sequence is canceled.
///
class PointerEvent::Motion::Cancel final : public PointerEvent::Motion
{
};

///
/// @brief Pointer enter event.
///
/// @note This event is sent when the pointer enters a region of interest.
///
class PointerEvent::Motion::Enter final : public PointerEvent::Motion
{
};

///
/// @brief Pointer leave event.
///
/// @note This event is sent when the pointer leaves a region of interest.
///
class PointerEvent::Motion::Leave final : public PointerEvent::Motion
{
};

///
/// @brief Pointer over event.
///
/// @note This event is sent when the pointer hovers directly over a region of interest.
///
class PointerEvent::Motion::Over final : public PointerEvent::Motion
{
};

///
/// @brief Pointer out event.
///
/// @note This event is sent when the pointer no longer hovers directly over a region of interest.
///
class PointerEvent::Motion::Out final : public PointerEvent::Motion
{
};

///
/// @brief Base class for pointer action events.
///
/// @note Action events notify about pointer actions, such as wheel scrolling, magnification, rotation, etc.
///
class PointerEvent::Action : public PointerEvent
{
    using PointerEvent::Copy;

public:
    class Magnify;
    class Rotate;
    class Swipe;
    class Scroll;
};

///
/// @brief Pointer magnification action event.
///
/// @note This event is sent for pointer magnification actions, such as pinch-to-zoom gestures.
///
class PointerEvent::Action::Magnify final : public PointerEvent::Action
{
public:
    auto GetPhase() const -> PointerActionPhase;
    auto SetPhase(PointerActionPhase const phase) -> void;

    auto GetScale() const -> Float64;
    auto SetScale(Float64 const scale) -> void;

private:
    PointerActionPhase _phase = PointerActionPhase::None;
    Float64 _scale = 1.0;
};

///
/// @brief Pointer rotation action event.
///
/// @note This event is sent for pointer rotation actions, such as rotating a touchpad or trackpad.
///
class PointerEvent::Action::Rotate final : public PointerEvent::Action
{
public:
    auto GetPhase() const -> PointerActionPhase;
    auto SetPhase(PointerActionPhase const phase) -> void;

    auto GetRotation() const -> Degree;
    auto SetRotation(Degree const rotation) -> void;

private:
    PointerActionPhase _phase = PointerActionPhase::None;
    Degree _rotation = 0.0;
};

///
/// @brief Pointer swipe action event.
///
/// @note This event is sent for pointer swipe actions, such as three-finger swipes on a touchpad.
///
class PointerEvent::Action::Swipe final : public PointerEvent::Action
{
public:
    auto GetPhase() const -> PointerActionPhase;
    auto SetPhase(PointerActionPhase const phase) -> void;

    auto GetDeltaX() const -> Dp;
    auto SetDeltaX(Dp const deltaX) -> void;

    auto GetDeltaY() const -> Dp;
    auto SetDeltaY(Dp const deltaY) -> void;

    auto IsInertial() const -> Bool;
    auto SetInertial(Bool const inertial) -> void;

private:
    PointerActionPhase _phase = PointerActionPhase::None;
    Dp _deltaX = 0.0;
    Dp _deltaY = 0.0;
    Bool _inertial = false;
};

///
/// @brief Pointer scroll action event.
///
/// @note This event is sent for pointer scroll actions, such as mouse wheel scrolling or touchpad scrolling.
///
class PointerEvent::Action::Scroll final : public PointerEvent::Action
{
public:
    auto GetPhase() const -> PointerActionPhase;
    auto SetPhase(PointerActionPhase const phase) -> void;

    auto GetDeltaX() const -> Dp;
    auto SetDeltaX(Dp const deltaX) -> void;

    auto GetDeltaY() const -> Dp;
    auto SetDeltaY(Dp const deltaY) -> void;

    auto GetPrecision() const -> PointerScrollPrecision;
    auto SetPrecision(PointerScrollPrecision const precision) -> void;

    auto GetModifiers() const -> Flags<ModifierKeyFlag>;
    auto SetModifiers(Flags<ModifierKeyFlag> const modifiers) -> void;

private:
    PointerActionPhase _phase = PointerActionPhase::None;
    Dp _deltaX = 0.0;
    Dp _deltaY = 0.0;
    PointerScrollPrecision _precision = PointerScrollPrecision::Coarse;
    Flags<ModifierKeyFlag> _modifiers = ModifierKeyFlag::None;
};

///
/// @brief Pointer event forecast.
///
/// @note This event is used to notify surrounding context that a pointer event is about to occur.
///
class PointerEvent::Forecast final : public PointerEvent
{
    using PointerEvent::Copy;

public:
    auto GetPointerEvent() const -> Event<PointerEvent> const&;
    auto SetPointerEvent(Event<PointerEvent> const& event) -> void;

    auto IsIntercepted() const -> Bool;
    auto SetIntercepted(Bool const intercepted) -> void;

private:
    Event<PointerEvent> _event;
    Bool _intercept = false;
};
}
}

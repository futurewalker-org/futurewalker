// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PlatformPointerEventType.hpp"
#include "Futurewalker.Application.PointerEventType.hpp"
#include "Futurewalker.Application.KeyEventType.hpp"

#include "Futurewalker.Event.EventParameter.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Core.MonotonicTime.hpp" 
#include "Futurewalker.Core.Flags.hpp" 

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
class PlatformPointerEvent : public EventParameter
{
public:
    static auto ConvertTiltToSpherical(Degree const& tiltX, Degree const& tiltY) -> std::pair<Degree, Degree>;

public:
    auto GetPointerId() const -> PointerId;
    auto SetPointerId(PointerId const& id) -> void;

    auto GetPointerType() const -> PointerType;
    auto SetPointerType(PointerType const& type) -> void;

    auto GetTimestamp() const -> MonotonicTime;
    auto SetTimestamp(MonotonicTime const& timestamp) -> void;

    auto IsPrimaryPointer() const -> Bool;
    auto SetPrimaryPointer(Bool const isPrimaryPointer) -> void;

    auto GetPosition() const -> Point<Dp>;
    auto SetPosition(Point<Dp> const& position) -> void;

    auto GetButtons() const -> Flags<PointerButtonFlag>;
    auto SetButtons(Flags<PointerButtonFlag> const buttons) -> void;

    auto GetModifiers() const -> Flags<ModifierKeyFlag>;
    auto SetModifiers(Flags<ModifierKeyFlag> const modifiers) -> void;

public:
    class Motion;
    class Action;

private:
    PointerId _id = 0U;
    PointerType _type = PointerType::Other;
    MonotonicTime _timestamp;
    Bool _isPrimaryPointer = false;
    Point<Dp> _position;
    Flags<PointerButtonFlag> _buttonState = PointerButtonFlag::None;
    Flags<ModifierKeyFlag> _modifierState = ModifierKeyFlag::None;
};

class PlatformPointerEvent::Motion : public PlatformPointerEvent
{
public: // Core motion events
    class Down;
    class Up;
    class Move;
    class Enter;
    class Leave;
    class Cancel;

protected:
    /// Common state for motion events.
    struct MotionState
    {
        Degree azimuth = 0;
        Degree altitude = 0;
        Degree twist = 0;
        Degree tiltX = 0;
        Degree tiltY = 0;
        Float64 pressure = 0;
        Float64 tangentialPressure = 0;
        PointerButton button = PointerButton::Unknown;
    };
};

class PlatformPointerEvent::Motion::Down final : public PlatformPointerEvent::Motion
{
public:
    auto GetTiltX() const -> Degree;
    auto SetTiltX(Degree const& tiltX) -> void;

    auto GetTiltY() const -> Degree;
    auto SetTiltY(Degree const& tiltY) -> void;

    auto GetAzimuth() const -> Degree;
    auto SetAzimuth(Degree const& azimuth) -> void;

    auto GetAltitude() const -> Degree;
    auto SetAltitude(Degree const& altitude) -> void;

    auto GetTwist() const -> Degree;
    auto SetTwist(Degree const& twist) -> void;

    auto GetPressure() const -> Float64;
    auto SetPressure(Float64 const pressure) -> void;

    auto GetTangentialPressure() const -> Float64;
    auto SetTangentialPressure(Float64 const pressure) -> void;

    auto GetButton() const -> PointerButton;
    auto SetButton(PointerButton const button) -> void;

private:
    MotionState _state;
};

class PlatformPointerEvent::Motion::Up final : public PlatformPointerEvent::Motion
{
public:
    auto GetTiltX() const -> Degree;
    auto SetTiltX(Degree const& tiltX) -> void;

    auto GetTiltY() const -> Degree;
    auto SetTiltY(Degree const& tiltY) -> void;

    auto GetAzimuth() const -> Degree;
    auto SetAzimuth(Degree const& azimuth) -> void;

    auto GetAltitude() const -> Degree;
    auto SetAltitude(Degree const& altitude) -> void;

    auto GetTwist() const -> Degree;
    auto SetTwist(Degree const& twist) -> void;

    auto GetPressure() const -> Float64;
    auto SetPressure(Float64 const pressure) -> void;

    auto GetTangentialPressure() const -> Float64;
    auto SetTangentialPressure(Float64 const pressure) -> void;

    auto GetButton() const -> PointerButton;
    auto SetButton(PointerButton const button) -> void;

private:
    MotionState _state;
};

class PlatformPointerEvent::Motion::Move final : public PlatformPointerEvent::Motion
{
public:
    auto GetTiltX() const -> Degree;
    auto SetTiltX(Degree const& tiltX) -> void;

    auto GetTiltY() const -> Degree;
    auto SetTiltY(Degree const& tiltY) -> void;

    auto GetAzimuth() const -> Degree;
    auto SetAzimuth(Degree const& azimuth) -> void;

    auto GetAltitude() const -> Degree;
    auto SetAltitude(Degree const& altitude) -> void;

    auto GetTwist() const -> Degree;
    auto SetTwist(Degree const& twist) -> void;

    auto GetPressure() const -> Float64;
    auto SetPressure(Float64 const pressure) -> void;

    auto GetTangentialPressure() const -> Float64;
    auto SetTangentialPressure(Float64 const pressure) -> void;

    auto GetButton() const -> PointerButton;
    auto SetButton(PointerButton const button) -> void;

private:
    MotionState _state;
};

class PlatformPointerEvent::Motion::Enter final : public PlatformPointerEvent::Motion
{
};

class PlatformPointerEvent::Motion::Leave final : public PlatformPointerEvent::Motion
{
};

class PlatformPointerEvent::Motion::Cancel final : public PlatformPointerEvent::Motion
{
};

class PlatformPointerEvent::Action : public PlatformPointerEvent
{
public: // Action events
    class Magnify;
    class Rotate;
    class Swipe;
    class Scroll;
};

class PlatformPointerEvent::Action::Magnify final : public PlatformPointerEvent::Action
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

class PlatformPointerEvent::Action::Rotate final : public PlatformPointerEvent::Action
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

class PlatformPointerEvent::Action::Swipe final : public PlatformPointerEvent::Action
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

class PlatformPointerEvent::Action::Scroll final : public PlatformPointerEvent::Action
{
public:
    auto GetDeltaX() const -> Dp;
    auto SetDeltaX(Dp const deltaX) -> void;

    auto GetDeltaY() const -> Dp;
    auto SetDeltaY(Dp const deltaY) -> void;

    auto GetPrecision() const -> PointerScrollPrecision;
    auto SetPrecision(PointerScrollPrecision const precision) -> void;

    auto GetModifiers() const -> Flags<ModifierKeyFlag>;
    auto SetModifiers(Flags<ModifierKeyFlag> const modifiers) -> void;

private:
    Dp _deltaX = 0.0;
    Dp _deltaY = 0.0;
    PointerScrollPrecision _precision = PointerScrollPrecision::Coarse;
    Flags<ModifierKeyFlag> _modifiers = ModifierKeyFlag::None;
};
}
}

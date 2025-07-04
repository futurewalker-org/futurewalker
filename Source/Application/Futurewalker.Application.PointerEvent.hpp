// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.PointerEventType.hpp"
#include "Futurewalker.Application.KeyEventType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

#include "Futurewalker.Event.Event.hpp"

#include "Futurewalker.Core.MonotonicTime.hpp"

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
    class Wheel;
    class Motion;
    class Gesture;
    class HitTest;

private:
    PointerId _id = 0U;
    PointerType _type = PointerType::Other;
    MonotonicTime _timestamp;
    Bool _isPrimaryPointer = false;
    Point<Dp> _position;
};

class PointerEvent::Motion : public PointerEvent
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

    auto GetButtons() const -> PointerButtonFlags;
    auto SetButtons(PointerButtonFlags const buttons) -> void;

    auto GetModifiers() const -> ModifierKeyFlags;
    auto SetModifiers(ModifierKeyFlags const modifiers) -> void;

public:
    class Down;
    class Up;
    class Move;
    class Cancel;
    class Enter;
    class Leave;
    class Out;
    class Over;

private:
    Point<Dp> _position;
    Degree _azimuth = 0;
    Degree _altitude = 0;
    Degree _twist = 0;
    Degree _tiltX = 0;
    Degree _tiltY = 0;
    Float64 _pressure = 0;
    Float64 _tangentialPressure = 0;
    PointerButton _button = PointerButton::Unknown;
    PointerButtonFlags _buttonState = PointerButtonFlags::None;
    ModifierKeyFlags _modifierState = ModifierKeyFlags::None;
};

class PointerEvent::Wheel final : public PointerEvent
{
public:
    auto GetDeltaX() const -> Float64;
    auto SetDeltaX(Float64 const deltaX) -> void;

    auto GetDeltaY() const -> Float64;
    auto SetDeltaY(Float64 const deltaY) -> void;

    auto GetPrecision() const -> PointerWheelPrecision;
    auto SetPrecision(PointerWheelPrecision const precision) -> void;

private:
    Float64 _deltaX = 0.0;
    Float64 _deltaY = 0.0;
    PointerWheelPrecision _precision = PointerWheelPrecision::Coarse;
};

class PointerEvent::Motion::Down final : public PointerEvent::Motion
{
};

class PointerEvent::Motion::Up final : public PointerEvent::Motion
{
};

class PointerEvent::Motion::Move final : public PointerEvent::Motion
{
};

class PointerEvent::Motion::Cancel final : public PointerEvent::Motion
{
};

class PointerEvent::Motion::Enter final : public PointerEvent::Motion
{
};

class PointerEvent::Motion::Leave final : public PointerEvent::Motion
{
};

class PointerEvent::Motion::Out final : public PointerEvent::Motion
{
};

class PointerEvent::Motion::Over final : public PointerEvent::Motion
{
};

class PointerEvent::Gesture : public PointerEvent
{
public:
    auto GetPhase() const -> PointerGesturePhase;
    auto SetPhase(PointerGesturePhase const phase) -> void;

public:
    class Magnify;
    class Rotate;
    class Swipe;
    class Scroll;

private:
    PointerGesturePhase _phase = PointerGesturePhase::None;
};

class PointerEvent::Gesture::Magnify final : public PointerEvent::Gesture
{
public:
    auto GetScale() const -> Float64;
    auto SetScale(Float64 const scale) -> void;

private:
    Float64 _scale = 1.0;
};

class PointerEvent::Gesture::Rotate final : public PointerEvent::Gesture
{
public:
    auto GetRotation() const -> Degree;
    auto SetRotation(Degree const rotation) -> void;

private:
    Degree _rotation = 0.0;
};

class PointerEvent::Gesture::Swipe final : public PointerEvent::Gesture
{
public:
    auto GetDeltaX() const -> Dp;
    auto SetDeltaX(Dp const deltaX) -> void;

    auto GetDeltaY() const -> Dp;
    auto SetDeltaY(Dp const deltaY) -> void;

private:
    Dp _deltaX = 0.0;
    Dp _deltaY = 0.0;
};

class PointerEvent::Gesture::Scroll final : public PointerEvent::Gesture
{
public:
    auto GetDeltaX() const -> Dp;
    auto SetDeltaX(Dp const deltaX) -> void;

    auto GetDeltaY() const -> Dp;
    auto SetDeltaY(Dp const deltaY) -> void;

private:
    Dp _deltaX = 0.0;
    Dp _deltaY = 0.0;
};

class PointerEvent::HitTest final : public PointerEvent
{
public:
    auto GetPointerEvent() const -> Event<PointerEvent> const&;
    auto SetPointerEvent(Event<PointerEvent> const& event) -> void;

    auto GetHit() const -> Bool;
    auto SetHit(Bool const hit) -> void;

private:
    Event<PointerEvent> _event;
    Bool _hit = false;
};
}
}

// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PointerEvent.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Copy parameter of PointerEvent.
///
/// @note This function will not copy parameters of derived classes.
///
void PointerEvent::Copy(PointerEvent& dst, PointerEvent const& src)
{
    dst._id = src._id;
    dst._type = src._type;
    dst._timestamp = src._timestamp;
    dst._isPrimaryPointer = src._isPrimaryPointer;
    dst._position = src._position;
}

///
/// @brief
///
auto PointerEvent::GetPointerId() const -> PointerId
{
    return _id;
}

///
/// @brief
///
/// @param id
///
auto PointerEvent::SetPointerId(PointerId const id) -> void
{
    _id = id;
}

///
/// @brief 
///
auto PointerEvent::GetPointerType() const -> PointerType
{
    return _type;
}

///
/// @brief
///
/// @param type
///
auto PointerEvent::SetPointerType(PointerType const type) -> void
{
    _type = type;
}

///
/// @brief 
///
auto PointerEvent::GetTimestamp() const -> MonotonicTime
{
    return _timestamp;
}

///
/// @brief
///
/// @param timestamp
///
/// @return
///
auto PointerEvent::SetTimestamp(MonotonicTime const timestamp) -> void
{
    _timestamp = timestamp;
}

///
/// @brief 
///
auto PointerEvent::IsPrimaryPointer() const -> Bool
{
    return _isPrimaryPointer;
}

///
/// @brief
///
/// @param isPrimary
///
auto PointerEvent::SetPrimaryPointer(Bool const isPrimaryPointer) -> void
{
    _isPrimaryPointer = isPrimaryPointer;
}

///
/// @brief 
///
auto PointerEvent::GetPosition() const -> Point<Dp>
{
    return _position;
}

///
/// @brief
///
/// @param pos
///
auto PointerEvent::SetPosition(Point<Dp> const& position) -> void
{
    _position = position;
}

auto PointerEvent::Wheel::GetDeltaX() const -> Float64
{
    return _deltaX;
}

auto PointerEvent::Wheel::SetDeltaX(Float64 const deltaX) -> void
{
    _deltaX = deltaX;
}

auto PointerEvent::Wheel::GetDeltaY() const -> Float64
{
    return _deltaY;
}

auto PointerEvent::Wheel::SetDeltaY(Float64 const deltaY) -> void
{
    _deltaY = deltaY;
}

auto PointerEvent::Wheel::GetPrecision() const -> PointerWheelPrecision
{
    return _precision;
}

auto PointerEvent::Wheel::SetPrecision(PointerWheelPrecision const precision) -> void
{
    _precision = precision;
}

///
/// @brief 
///
auto PointerEvent::Motion::GetTiltX() const -> Degree
{
    return _tiltX;
}

///
/// @brief
///
/// @param tiltX
///
auto PointerEvent::Motion::SetTiltX(Degree const tiltX) -> void
{
    _tiltX = tiltX;
}

///
/// @brief
///
auto PointerEvent::Motion::GetTiltY() const -> Degree
{
    return _tiltY;
}

///
/// @brief
///
/// @param tiltY
///
auto PointerEvent::Motion::SetTiltY(Degree const tiltY) -> void
{
    _tiltY = tiltY;
}

///
/// @brief
///
auto PointerEvent::Motion::GetAzimuth() const -> Degree
{
    return _azimuth;
}

///
/// @brief
///
/// @param azimuth
///
auto PointerEvent::Motion::SetAzimuth(Degree const azimuth) -> void
{
    _azimuth = azimuth;
}

///
/// @brief
///
auto PointerEvent::Motion::GetAltitude() const -> Degree
{
    return _altitude;
}

///
/// @brief
///
/// @param altitude
///
auto PointerEvent::Motion::SetAltitude(Degree const altitude) -> void
{
    _altitude = altitude;
}

///
/// @brief
///
auto PointerEvent::Motion::GetTwist() const -> Degree
{
    return _twist;
}

///
/// @brief
///
/// @param twist
///
auto PointerEvent::Motion::SetTwist(Degree const twist) -> void
{
    _twist = twist;
}

///
/// @brief 
///
auto PointerEvent::Motion::GetPressure() const -> Float64
{
    return _pressure;
}

///
/// @brief
///
/// @param pressure
///
auto PointerEvent::Motion::SetPressure(Float64 const pressure) -> void
{
    _pressure = pressure;
}

auto PointerEvent::Motion::GetTangentialPressure() const -> Float64
{
    return _tangentialPressure;
}

auto PointerEvent::Motion::SetTangentialPressure(Float64 const pressure) -> void
{
    _tangentialPressure = pressure;
}

auto PointerEvent::Motion::GetButton() const -> PointerButton
{
    return _button;
}

auto PointerEvent::Motion::SetButton(PointerButton const button) -> void
{
    _button = button;
}

auto PointerEvent::Motion::GetButtons() const -> PointerButtonFlags
{
    return _buttonState;
}

auto PointerEvent::Motion::SetButtons(PointerButtonFlags const buttons) -> void
{
    _buttonState = buttons;
}

auto PointerEvent::Motion::GetModifiers() const -> ModifierKeyFlags
{
    return _modifierState;
}

auto PointerEvent::Motion::SetModifiers(ModifierKeyFlags const modifiers) -> void
{
    _modifierState = modifiers;
}

auto PointerEvent::Gesture::GetPhase() const -> PointerGesturePhase
{
    return _phase;
}

auto PointerEvent::Gesture::SetPhase(PointerGesturePhase const phase) -> void
{
    _phase = phase;
}

auto PointerEvent::Gesture::Magnify::GetScale() const -> Float64
{
    return _scale;
}

auto PointerEvent::Gesture::Magnify::SetScale(Float64 const scale) -> void
{
    _scale = scale;
}

auto PointerEvent::Gesture::Rotate::GetRotation() const -> Degree
{
    return _rotation;
}

auto PointerEvent::Gesture::Rotate::SetRotation(Degree const rotation) -> void
{
    _rotation = rotation;
}

auto PointerEvent::Gesture::Swipe::GetDeltaX() const -> Dp
{
    return _deltaX;
}

auto PointerEvent::Gesture::Swipe::SetDeltaX(Dp const deltaX) -> void
{
    _deltaX = deltaX;
}

auto PointerEvent::Gesture::Swipe::GetDeltaY() const -> Dp
{
    return _deltaY;
}

auto PointerEvent::Gesture::Swipe::SetDeltaY(Dp const deltaY) -> void
{
    _deltaY = deltaY;
}

auto PointerEvent::Gesture::Scroll::GetDeltaX() const -> Dp
{
    return _deltaX;
}
auto PointerEvent::Gesture::Scroll::SetDeltaX(Dp const deltaX) -> void
{
    _deltaX = deltaX;
}

auto PointerEvent::Gesture::Scroll::GetDeltaY() const -> Dp
{
    return _deltaY;
}
auto PointerEvent::Gesture::Scroll::SetDeltaY(Dp const deltaY) -> void
{
    _deltaY = deltaY;
}

auto PointerEvent::Intercept::GetPointerEvent() const -> Event<PointerEvent> const&
{
    return _event;
}

auto PointerEvent::Intercept::SetPointerEvent(Event<PointerEvent> const& event) -> void
{
    _event = event;
}

auto PointerEvent::Intercept::GetShouldIntercept() const -> Bool
{
    return _intercept;
}

auto PointerEvent::Intercept::SetShouldIntercept(Bool const intercept) -> void
{
    _intercept = intercept;
}
}

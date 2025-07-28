// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PointerEvent.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Copy parameter of PointerEvent.
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

///
/// @brief Get tilt X angle.
///
auto PointerEvent::Motion::Down::GetTiltX() const -> Degree
{
    return _state.tiltX;
}

///
/// @brief Set tilt X angle.
///
/// @param tiltX Tilt X angle.
///
auto PointerEvent::Motion::Down::SetTiltX(Degree const tiltX) -> void
{
    _state.tiltX = tiltX;
}

///
/// @brief Get tilt Y angle.
///
auto PointerEvent::Motion::Down::GetTiltY() const -> Degree
{
    return _state.tiltY;
}

///
/// @brief Set tilt Y angle.
///
/// @param tiltY Tilt Y angle.
///
auto PointerEvent::Motion::Down::SetTiltY(Degree const tiltY) -> void
{
    _state.tiltY = tiltY;
}

///
/// @brief Get azimuth angle.
///
auto PointerEvent::Motion::Down::GetAzimuth() const -> Degree
{
    return _state.azimuth;
}

///
/// @brief Set azimuth angle.
///
/// @param azimuth Azimuth angle.
///
auto PointerEvent::Motion::Down::SetAzimuth(Degree const azimuth) -> void
{
    _state.azimuth = azimuth;
}

///
/// @brief Get altitude angle.
///
auto PointerEvent::Motion::Down::GetAltitude() const -> Degree
{
    return _state.altitude;
}

///
/// @brief Set altitude angle.
///
/// @param altitude Altitude angle.
///
auto PointerEvent::Motion::Down::SetAltitude(Degree const altitude) -> void
{
    _state.altitude = altitude;
}

///
/// @brief Get twist angle.
///
auto PointerEvent::Motion::Down::GetTwist() const -> Degree
{
    return _state.twist;
}

///
/// @brief Set twist angle.
///
/// @param twist Twist angle.
///
auto PointerEvent::Motion::Down::SetTwist(Degree const twist) -> void
{
    _state.twist = twist;
}

///
/// @brief Get pressure.
///
auto PointerEvent::Motion::Down::GetPressure() const -> Float64
{
    return _state.pressure;
}

///
/// @brief Set pressure.
///
/// @param pressure Pressure value.
///
auto PointerEvent::Motion::Down::SetPressure(Float64 const pressure) -> void
{
    _state.pressure = pressure;
}

///
/// @brief Get tangential pressure.
///
auto PointerEvent::Motion::Down::GetTangentialPressure() const -> Float64
{
    return _state.tangentialPressure;
}

///
/// @brief Set tangential pressure.
///
/// @param pressure Tangential pressure value.
///
auto PointerEvent::Motion::Down::SetTangentialPressure(Float64 const pressure) -> void
{
    _state.tangentialPressure = pressure;
}

///
/// @brief Get button.
///
auto PointerEvent::Motion::Down::GetButton() const -> PointerButton
{
    return _state.button;
}

///
/// @brief Set button.
///
/// @param button Button value.
///
auto PointerEvent::Motion::Down::SetButton(PointerButton const button) -> void
{
    _state.button = button;
}

///
/// @brief Get button state.
///
auto PointerEvent::Motion::Down::GetButtons() const -> PointerButtonFlags
{
    return _state.buttonState;
}

///
/// @brief Set button state.
///
/// @param buttons Button state flags.
///
auto PointerEvent::Motion::Down::SetButtons(PointerButtonFlags const buttons) -> void
{
    _state.buttonState = buttons;
}

///
/// @brief Get modifier state.
///
auto PointerEvent::Motion::Down::GetModifiers() const -> ModifierKeyFlags
{
    return _state.modifierState;
}

///
/// @brief Set modifier state.
///
/// @param modifiers Modifier state flags.
///
auto PointerEvent::Motion::Down::SetModifiers(ModifierKeyFlags const modifiers) -> void
{
    _state.modifierState = modifiers;
}

///
/// @brief Get tilt X angle.
///
auto PointerEvent::Motion::Up::GetTiltX() const -> Degree
{
    return _state.tiltX;
}

///
/// @brief Set tilt X angle.
///
/// @param tiltX Tilt X angle.
///
auto PointerEvent::Motion::Up::SetTiltX(Degree const tiltX) -> void
{
    _state.tiltX = tiltX;
}

///
/// @brief Get tilt Y angle.
///
auto PointerEvent::Motion::Up::GetTiltY() const -> Degree
{
    return _state.tiltY;
}

///
/// @brief Set tilt Y angle.
///
/// @param tiltY Tilt Y angle.
///
auto PointerEvent::Motion::Up::SetTiltY(Degree const tiltY) -> void
{
    _state.tiltY = tiltY;
}

///
/// @brief Get azimuth angle.
///
auto PointerEvent::Motion::Up::GetAzimuth() const -> Degree
{
    return _state.azimuth;
}

///
/// @brief Set azimuth angle.
///
/// @param azimuth Azimuth angle.
///
auto PointerEvent::Motion::Up::SetAzimuth(Degree const azimuth) -> void
{
    _state.azimuth = azimuth;
}

///
/// @brief Get altitude angle.
///
auto PointerEvent::Motion::Up::GetAltitude() const -> Degree
{
    return _state.altitude;
}

///
/// @brief Set altitude angle.
///
/// @param altitude Altitude angle.
///
auto PointerEvent::Motion::Up::SetAltitude(Degree const altitude) -> void
{
    _state.altitude = altitude;
}

///
/// @brief Get twist angle.
///
auto PointerEvent::Motion::Up::GetTwist() const -> Degree
{
    return _state.twist;
}

///
/// @brief Set twist angle.
///
/// @param twist Twist angle.
///
auto PointerEvent::Motion::Up::SetTwist(Degree const twist) -> void
{
    _state.twist = twist;
}

///
/// @brief Get pressure.
///
auto PointerEvent::Motion::Up::GetPressure() const -> Float64
{
    return _state.pressure;
}

///
/// @brief Set pressure.
///
/// @param pressure Pressure value.
///
auto PointerEvent::Motion::Up::SetPressure(Float64 const pressure) -> void
{
    _state.pressure = pressure;
}

///
/// @brief Get tangential pressure.
///
auto PointerEvent::Motion::Up::GetTangentialPressure() const -> Float64
{
    return _state.tangentialPressure;
}

///
/// @brief Set tangential pressure.
///
/// @param pressure Tangential pressure value.
///
auto PointerEvent::Motion::Up::SetTangentialPressure(Float64 const pressure) -> void
{
    _state.tangentialPressure = pressure;
}

///
/// @brief Get button.
///
auto PointerEvent::Motion::Up::GetButton() const -> PointerButton
{
    return _state.button;
}

///
/// @brief Set button.
///
/// @param button Button value.
///
auto PointerEvent::Motion::Up::SetButton(PointerButton const button) -> void
{
    _state.button = button;
}

///
/// @brief Get button state.
///
auto PointerEvent::Motion::Up::GetButtons() const -> PointerButtonFlags
{
    return _state.buttonState;
}

///
/// @brief Set button state.
///
/// @param buttons Button state flags.
///
auto PointerEvent::Motion::Up::SetButtons(PointerButtonFlags const buttons) -> void
{
    _state.buttonState = buttons;
}

///
/// @brief Get modifier state.
///
auto PointerEvent::Motion::Up::GetModifiers() const -> ModifierKeyFlags
{
    return _state.modifierState;
}

///
/// @brief Set modifier state.
///
/// @param modifiers Modifier state flags.
///
auto PointerEvent::Motion::Up::SetModifiers(ModifierKeyFlags const modifiers) -> void
{
    _state.modifierState = modifiers;
}

///
/// @brief Get tilt X angle.
///
auto PointerEvent::Motion::Move::GetTiltX() const -> Degree
{
    return _state.tiltX;
}

///
/// @brief Set tilt X angle.
///
/// @param tiltX Tilt X angle.
///
auto PointerEvent::Motion::Move::SetTiltX(Degree const tiltX) -> void
{
    _state.tiltX = tiltX;
}

///
/// @brief Get tilt Y angle.
///
auto PointerEvent::Motion::Move::GetTiltY() const -> Degree
{
    return _state.tiltY;
}

///
/// @brief Set tilt Y angle.
///
/// @param tiltY Tilt Y angle.
///
auto PointerEvent::Motion::Move::SetTiltY(Degree const tiltY) -> void
{
    _state.tiltY = tiltY;
}

///
/// @brief Get azimuth angle.
///
auto PointerEvent::Motion::Move::GetAzimuth() const -> Degree
{
    return _state.azimuth;
}

///
/// @brief Set azimuth angle.
///
/// @param azimuth Azimuth angle.
///
auto PointerEvent::Motion::Move::SetAzimuth(Degree const azimuth) -> void
{
    _state.azimuth = azimuth;
}

///
/// @brief Get altitude angle.
///
auto PointerEvent::Motion::Move::GetAltitude() const -> Degree
{
    return _state.altitude;
}

///
/// @brief Set altitude angle.
///
/// @param altitude Altitude angle.
///
auto PointerEvent::Motion::Move::SetAltitude(Degree const altitude) -> void
{
    _state.altitude = altitude;
}

///
/// @brief Get twist angle.
///
auto PointerEvent::Motion::Move::GetTwist() const -> Degree
{
    return _state.twist;
}

///
/// @brief Set twist angle.
///
/// @param twist Twist angle.
///
auto PointerEvent::Motion::Move::SetTwist(Degree const twist) -> void
{
    _state.twist = twist;
}

///
/// @brief Get pressure.
///
auto PointerEvent::Motion::Move::GetPressure() const -> Float64
{
    return _state.pressure;
}

///
/// @brief Set pressure.
///
/// @param pressure Pressure value.
///
auto PointerEvent::Motion::Move::SetPressure(Float64 const pressure) -> void
{
    _state.pressure = pressure;
}

///
/// @brief Get tangential pressure.
///
auto PointerEvent::Motion::Move::GetTangentialPressure() const -> Float64
{
    return _state.tangentialPressure;
}

///
/// @brief Set tangential pressure.
///
/// @param pressure Tangential pressure value.
///
auto PointerEvent::Motion::Move::SetTangentialPressure(Float64 const pressure) -> void
{
    _state.tangentialPressure = pressure;
}

///
/// @brief Get button.
///
auto PointerEvent::Motion::Move::GetButton() const -> PointerButton
{
    return _state.button;
}

///
/// @brief Set button.
///
/// @param button Button value.
///
auto PointerEvent::Motion::Move::SetButton(PointerButton const button) -> void
{
    _state.button = button;
}

///
/// @brief Get button state.
///
auto PointerEvent::Motion::Move::GetButtons() const -> PointerButtonFlags
{
    return _state.buttonState;
}

///
/// @brief Set button state.
///
/// @param buttons Button state flags.
///
auto PointerEvent::Motion::Move::SetButtons(PointerButtonFlags const buttons) -> void
{
    _state.buttonState = buttons;
}

///
/// @brief Get modifier state.
///
auto PointerEvent::Motion::Move::GetModifiers() const -> ModifierKeyFlags
{
    return _state.modifierState;
}

///
/// @brief Set modifier state.
///
/// @param modifiers Modifier state flags.
///
auto PointerEvent::Motion::Move::SetModifiers(ModifierKeyFlags const modifiers) -> void
{
    _state.modifierState = modifiers;
}

///
/// @brief Get magnification phase.
///
auto PointerEvent::Action::Magnify::GetPhase() const -> PointerActionPhase
{
    return _phase;
}

///
/// @brief Set magnification phase.
///
/// @param phase Magnification phase.
///
auto PointerEvent::Action::Magnify::SetPhase(PointerActionPhase const phase) -> void
{
    _phase = phase;
}

///
/// @brief Get scale factor.
///
auto PointerEvent::Action::Magnify::GetScale() const -> Float64
{
    return _scale;
}

///
/// @brief Set scale factor.
///
/// @param scale Scale factor.
///
auto PointerEvent::Action::Magnify::SetScale(Float64 const scale) -> void
{
    _scale = scale;
}

///
/// @brief Get rotation phase.
///
auto PointerEvent::Action::Rotate::GetPhase() const -> PointerActionPhase
{
    return _phase;
}

///
/// @brief Set rotation phase.
///
/// @param phase Rotation phase.
///
auto PointerEvent::Action::Rotate::SetPhase(PointerActionPhase const phase) -> void
{
    _phase = phase;
}

///
/// @brief Get rotation angle.
///
auto PointerEvent::Action::Rotate::GetRotation() const -> Degree
{
    return _rotation;
}

///
/// @brief Set rotation angle.
///
/// @param rotation Rotation angle.
///
auto PointerEvent::Action::Rotate::SetRotation(Degree const rotation) -> void
{
    _rotation = rotation;
}

///
/// @brief Get swipe phase.
///
auto PointerEvent::Action::Swipe::GetPhase() const -> PointerActionPhase
{
    return _phase;
}

///
/// @brief Set swipe phase.
///
/// @param phase Swipe phase.
///
auto PointerEvent::Action::Swipe::SetPhase(PointerActionPhase const phase) -> void
{
    _phase = phase;
}

///
/// @brief Get delta X.
///
auto PointerEvent::Action::Swipe::GetDeltaX() const -> Dp
{
    return _deltaX;
}

///
/// @brief Set delta X.
///
/// @param deltaX Delta X value.
///
auto PointerEvent::Action::Swipe::SetDeltaX(Dp const deltaX) -> void
{
    _deltaX = deltaX;
}

///
/// @brief Get delta Y.
///
auto PointerEvent::Action::Swipe::GetDeltaY() const -> Dp
{
    return _deltaY;
}

///
/// @brief Set delta Y.
///
/// @param deltaY Delta Y value.
///
auto PointerEvent::Action::Swipe::SetDeltaY(Dp const deltaY) -> void
{
    _deltaY = deltaY;
}

auto PointerEvent::Action::Swipe::IsInertial() const -> Bool
{
    return _inertial;
}

auto PointerEvent::Action::Swipe::SetInertial(Bool const inertial) -> void
{
    _inertial = inertial;
}

///
/// @brief Get scroll phase.
///
auto PointerEvent::Action::Scroll::GetPhase() const -> PointerActionPhase
{
    return _phase;
}

///
/// @brief Set scroll phase.
///
/// @param phase Scroll phase.
///
auto PointerEvent::Action::Scroll::SetPhase(PointerActionPhase const phase) -> void
{
    _phase = phase;
}

///
/// @brief Get delta X.
///
auto PointerEvent::Action::Scroll::GetDeltaX() const -> Dp
{
    return _deltaX;
}

///
/// @brief Set delta X.
///
/// @param deltaX Delta X value.
///
auto PointerEvent::Action::Scroll::SetDeltaX(Dp const deltaX) -> void
{
    _deltaX = deltaX;
}

///
/// @brief Get delta Y.
///
auto PointerEvent::Action::Scroll::GetDeltaY() const -> Dp
{
    return _deltaY;
}

///
/// @brief Set delta Y.
///
/// @param deltaY Delta Y value.
///
auto PointerEvent::Action::Scroll::SetDeltaY(Dp const deltaY) -> void
{
    _deltaY = deltaY;
}

///
/// @brief Get scroll precision.
///
auto PointerEvent::Action::Scroll::GetPrecision() const -> PointerScrollPrecision
{
    return _precision;
}

///
/// @brief Set scroll precision.
///
/// @param precision Scroll precision.
///
auto PointerEvent::Action::Scroll::SetPrecision(PointerScrollPrecision const precision) -> void
{
    _precision = precision;
}

///
/// @brief Get modifier state.
///
auto PointerEvent::Action::Scroll::GetModifiers() const -> ModifierKeyFlags
{
    return _modifiers;
}

///
/// @brief Set modifier state.
///
/// @param modifierState Modifier state flags.
///
auto PointerEvent::Action::Scroll::SetModifiers(ModifierKeyFlags const modifiers) -> void
{
    _modifiers = modifiers;
}

///
/// @brief Get pointer event.
///
auto PointerEvent::Forecast::GetPointerEvent() const -> Event<PointerEvent> const&
{
    return _event;
}

///
/// @brief Set pointer event.
///
/// @param event Pointer event.
///
auto PointerEvent::Forecast::SetPointerEvent(Event<PointerEvent> const& event) -> void
{
    _event = event;
}

///
/// @brief Get interception status.
///
auto PointerEvent::Forecast::IsIntercepted() const -> Bool
{
    return _intercept;
}

///
/// @brief Set interception status.
///
/// @param intercepted Interception status.
///
auto PointerEvent::Forecast::SetIntercepted(Bool const intercepted) -> void
{
    _intercept = intercepted;
}
}

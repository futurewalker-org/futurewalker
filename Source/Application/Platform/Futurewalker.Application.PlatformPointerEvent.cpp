// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.PlatformPointerEvent.hpp"

#include <numbers>

namespace FW_DETAIL_NS
{
///
/// @brief Convert tilt values to spherical values.
///
/// @return Spherical (azimuth, altitude) pair.
///
/// @note Based on W3C PointerEvent spec: https://w3c.github.io/pointerevents/#converting-between-tiltx-tilty-and-altitudeangle-azimuthangle
///
auto PlatformPointerEvent::ConvertTiltToSpherical(Degree const& tiltX, Degree const& tiltY) -> std::pair<Degree, Degree>
{
    auto const tiltXRad = UnitFunction::ConvertDegreeToRadian(tiltX);
    auto const tiltYRad = UnitFunction::ConvertDegreeToRadian(tiltY);

    auto const pi = std::numbers::pi_v<float64_t> / 2;

    Radian azimuth = 0.0;
    if (tiltX == 0.0)
    {
        if (tiltY > 0)
        {
            azimuth = pi / 2;
        }
        else if (tiltY < 0)
        {
            azimuth = 3 * pi / 2;
        }
    }
    else if (tiltY == 0)
    {
        if (tiltX < 0)
        {
            azimuth = pi;
        }
    }
    else if (Degree::Abs(tiltX) == 90 || Degree::Abs(tiltY) == 90)
    {
        azimuth = 0;
    }
    else
    {
        auto const tanX = std::tan(static_cast<float64_t>(tiltXRad));
        auto const tanY = std::tan(static_cast<float64_t>(tiltYRad));
        azimuth = std::atan2(tanY, tanX);
        if (azimuth < 0)
        {
            azimuth += 2 * pi;
        }
    }

    Radian altitude = 0.0;
    if (Degree::Abs(tiltX) == 90 || Degree::Abs(tiltY) == 90)
    {
        altitude = 0;
    }
    else if (tiltX == 0)
    {
        altitude = Radian(pi / 2) - Radian::Abs(tiltYRad);
    }
    else if (tiltY == 0)
    {
        altitude = Radian(pi / 2) - Radian::Abs(tiltXRad);
    }
    else
    {
        auto const txr = static_cast<float64_t>(tiltXRad);
        auto const tyr = static_cast<float64_t>(tiltYRad);
        altitude = std::atan(1.0 / std::sqrt(std::pow(std::tan(txr), 2)) + std::pow(std::tan(tyr), 2));
    }
    return {UnitFunction::ConvertRadianToDegree(azimuth), UnitFunction::ConvertRadianToDegree(altitude)};
}

///
/// @brief
///
/// @return
///
auto PlatformPointerEvent::GetPointerId() const -> PointerId
{
    return _id;
}

///
/// @brief
///
/// @param id
///
/// @return
///
auto PlatformPointerEvent::SetPointerId(PointerId const& id) -> void
{
    _id = id;
}

///
/// @brief
///
/// @return
///
auto PlatformPointerEvent::GetPointerType() const -> PointerType
{
    return _type;
}

///
/// @brief
///
/// @param type
///
/// @return
///
auto PlatformPointerEvent::SetPointerType(PointerType const& type) -> void
{
    _type = type;
}

///
/// @brief
///
/// @return
///
auto PlatformPointerEvent::GetTimestamp() const -> MonotonicTime
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
auto PlatformPointerEvent::SetTimestamp(MonotonicTime const& timestamp) -> void
{
    _timestamp = timestamp;
}

///
/// @brief
///
/// @return
///
auto PlatformPointerEvent::IsPrimaryPointer() const -> Bool
{
    return _isPrimaryPointer;
}

///
/// @brief
///
/// @param isPrimary
///
/// @return
///
auto PlatformPointerEvent::SetPrimaryPointer(Bool const isPrimaryPointer) -> void
{
    _isPrimaryPointer = isPrimaryPointer;
}

///
/// @brief
///
/// @return
///
auto PlatformPointerEvent::GetPosition() const -> Point<Dp>
{
    return _position;
}

///
/// @brief
///
/// @param pos
///
/// @return
///
auto PlatformPointerEvent::SetPosition(Point<Dp> const& position) -> void
{
    _position = position;
}

auto PlatformPointerEvent::GetButtons() const -> PointerButtonFlags
{
    return _buttonState;
}

auto PlatformPointerEvent::SetButtons(PointerButtonFlags const buttons) -> void
{
    _buttonState = buttons;
}

auto PlatformPointerEvent::GetModifiers() const -> ModifierKeyFlags
{
    return _modifierState;
}

auto PlatformPointerEvent::SetModifiers(ModifierKeyFlags const modifiers) -> void
{
    _modifierState = modifiers;
}

auto PlatformPointerEvent::Motion::Down::GetTiltX() const -> Degree
{
    return _state.tiltX;
}

auto PlatformPointerEvent::Motion::Down::SetTiltX(Degree const& tiltX) -> void
{
    _state.tiltX = tiltX;
}

auto PlatformPointerEvent::Motion::Down::GetTiltY() const -> Degree
{
    return _state.tiltY;
}

auto PlatformPointerEvent::Motion::Down::SetTiltY(Degree const& tiltY) -> void
{
    _state.tiltY = tiltY;
}

///
/// @brief Get azimuth of pointer.
///
auto PlatformPointerEvent::Motion::Down::GetAzimuth() const -> Degree
{
    return _state.azimuth;
}

auto PlatformPointerEvent::Motion::Down::SetAzimuth(Degree const& azimuth) -> void
{
    _state.azimuth = azimuth;
}

///
/// @brief Get altitude of pointer.
///
auto PlatformPointerEvent::Motion::Down::GetAltitude() const -> Degree
{
    return _state.altitude;
}

auto PlatformPointerEvent::Motion::Down::SetAltitude(Degree const& altitude) -> void
{
    _state.altitude = altitude;
}

///
/// @brief Get twist of pointer.
///
auto PlatformPointerEvent::Motion::Down::GetTwist() const -> Degree
{
    return _state.twist;
}

///
/// @brief
///
/// @param twist
///
/// @return
///
auto PlatformPointerEvent::Motion::Down::SetTwist(Degree const& twist) -> void
{
    _state.twist = twist;
}

///
/// @brief
///
/// @return
///
auto PlatformPointerEvent::Motion::Down::GetPressure() const -> Float64
{
    return _state.pressure;
}

///
/// @brief
///
/// @param pressure
///
/// @return
///
auto PlatformPointerEvent::Motion::Down::SetPressure(Float64 const pressure) -> void
{
    _state.pressure = pressure;
}

auto PlatformPointerEvent::Motion::Down::GetTangentialPressure() const -> Float64
{
    return _state.tangentialPressure;
}

auto PlatformPointerEvent::Motion::Down::SetTangentialPressure(Float64 const pressure) -> void
{
    _state.tangentialPressure = pressure;
}

auto PlatformPointerEvent::Motion::Down::GetButton() const -> PointerButton
{
    return _state.button;
}

auto PlatformPointerEvent::Motion::Down::SetButton(PointerButton const button) -> void
{
    _state.button = button;
}

auto PlatformPointerEvent::Motion::Up::GetTiltX() const -> Degree
{
    return _state.tiltX;
}

auto PlatformPointerEvent::Motion::Up::SetTiltX(Degree const& tiltX) -> void
{
    _state.tiltX = tiltX;
}

auto PlatformPointerEvent::Motion::Up::GetTiltY() const -> Degree
{
    return _state.tiltY;
}

auto PlatformPointerEvent::Motion::Up::SetTiltY(Degree const& tiltY) -> void
{
    _state.tiltY = tiltY;
}

auto PlatformPointerEvent::Motion::Up::GetAzimuth() const -> Degree
{
    return _state.azimuth;
}

auto PlatformPointerEvent::Motion::Up::SetAzimuth(Degree const& azimuth) -> void
{
    _state.azimuth = azimuth;
}

auto PlatformPointerEvent::Motion::Up::GetAltitude() const -> Degree
{
    return _state.altitude;
}

auto PlatformPointerEvent::Motion::Up::SetAltitude(Degree const& altitude) -> void
{
    _state.altitude = altitude;
}

auto PlatformPointerEvent::Motion::Up::GetTwist() const -> Degree
{
    return _state.twist;
}

auto PlatformPointerEvent::Motion::Up::SetTwist(Degree const& twist) -> void
{
    _state.twist = twist;
}

auto PlatformPointerEvent::Motion::Up::GetPressure() const -> Float64
{
    return _state.pressure;
}

auto PlatformPointerEvent::Motion::Up::SetPressure(Float64 const pressure) -> void
{
    _state.pressure = pressure;
}

auto PlatformPointerEvent::Motion::Up::GetTangentialPressure() const -> Float64
{
    return _state.tangentialPressure;
}

auto PlatformPointerEvent::Motion::Up::SetTangentialPressure(Float64 const pressure) -> void
{
    _state.tangentialPressure = pressure;
}

auto PlatformPointerEvent::Motion::Up::GetButton() const -> PointerButton
{
    return _state.button;
}

auto PlatformPointerEvent::Motion::Up::SetButton(PointerButton const button) -> void
{
    _state.button = button;
}

auto PlatformPointerEvent::Motion::Move::GetTiltX() const -> Degree
{
    return _state.tiltX;
}

auto PlatformPointerEvent::Motion::Move::SetTiltX(Degree const& tiltX) -> void
{
    _state.tiltX = tiltX;
}

auto PlatformPointerEvent::Motion::Move::GetTiltY() const -> Degree
{
    return _state.tiltY;
}

auto PlatformPointerEvent::Motion::Move::SetTiltY(Degree const& tiltY) -> void
{
    _state.tiltY = tiltY;
}

auto PlatformPointerEvent::Motion::Move::GetAzimuth() const -> Degree
{
    return _state.azimuth;
}

auto PlatformPointerEvent::Motion::Move::SetAzimuth(Degree const& azimuth) -> void
{
    _state.azimuth = azimuth;
}

auto PlatformPointerEvent::Motion::Move::GetAltitude() const -> Degree
{
    return _state.altitude;
}

auto PlatformPointerEvent::Motion::Move::SetAltitude(Degree const& altitude) -> void
{
    _state.altitude = altitude;
}

auto PlatformPointerEvent::Motion::Move::GetTwist() const -> Degree
{
    return _state.twist;
}

auto PlatformPointerEvent::Motion::Move::SetTwist(Degree const& twist) -> void
{
    _state.twist = twist;
}

auto PlatformPointerEvent::Motion::Move::GetPressure() const -> Float64
{
    return _state.pressure;
}

auto PlatformPointerEvent::Motion::Move::SetPressure(Float64 const pressure) -> void
{
    _state.pressure = pressure;
}

auto PlatformPointerEvent::Motion::Move::GetTangentialPressure() const -> Float64
{
    return _state.tangentialPressure;
}

auto PlatformPointerEvent::Motion::Move::SetTangentialPressure(Float64 const pressure) -> void
{
    _state.tangentialPressure = pressure;
}

auto PlatformPointerEvent::Motion::Move::GetButton() const -> PointerButton
{
    return _state.button;
}

auto PlatformPointerEvent::Motion::Move::SetButton(PointerButton const button) -> void
{
    _state.button = button;
}

auto PlatformPointerEvent::Action::Magnify::GetPhase() const -> PointerActionPhase
{
    return _phase;
}

auto PlatformPointerEvent::Action::Magnify::SetPhase(PointerActionPhase const phase) -> void
{
    _phase = phase;
}

auto PlatformPointerEvent::Action::Magnify::GetScale() const -> Float64
{
    return _scale;
}

auto PlatformPointerEvent::Action::Magnify::SetScale(Float64 const scale) -> void
{
    _scale = scale;
}

auto PlatformPointerEvent::Action::Rotate::GetPhase() const -> PointerActionPhase
{
    return _phase;
}

auto PlatformPointerEvent::Action::Rotate::SetPhase(PointerActionPhase const phase) -> void
{
    _phase = phase;
}

auto PlatformPointerEvent::Action::Rotate::GetRotation() const -> Degree
{
    return _rotation;
}

auto PlatformPointerEvent::Action::Rotate::SetRotation(Degree const rotation) -> void
{
    _rotation = rotation;
}

auto PlatformPointerEvent::Action::Swipe::GetPhase() const -> PointerActionPhase
{
    return _phase;
}

auto PlatformPointerEvent::Action::Swipe::SetPhase(PointerActionPhase const phase) -> void
{
    _phase = phase;
}

auto PlatformPointerEvent::Action::Swipe::GetDeltaX() const -> Dp
{
    return _deltaX;
}

auto PlatformPointerEvent::Action::Swipe::SetDeltaX(Dp const deltaX) -> void
{
    _deltaX = deltaX;
}

auto PlatformPointerEvent::Action::Swipe::GetDeltaY() const -> Dp
{
    return _deltaY;
}

auto PlatformPointerEvent::Action::Swipe::SetDeltaY(Dp const deltaY) -> void
{
    _deltaY = deltaY;
}

auto PlatformPointerEvent::Action::Swipe::IsInertial() const -> Bool
{
    return _inertial;
}

auto PlatformPointerEvent::Action::Swipe::SetInertial(Bool const inertial) -> void
{
    _inertial = inertial;
}

auto PlatformPointerEvent::Action::Scroll::GetDeltaX() const -> Dp
{
    return _deltaX;
}
auto PlatformPointerEvent::Action::Scroll::SetDeltaX(Dp const deltaX) -> void
{
    _deltaX = deltaX;
}

auto PlatformPointerEvent::Action::Scroll::GetDeltaY() const -> Dp
{
    return _deltaY;
}
auto PlatformPointerEvent::Action::Scroll::SetDeltaY(Dp const deltaY) -> void
{
    _deltaY = deltaY;
}

auto PlatformPointerEvent::Action::Scroll::GetPrecision() const -> PointerScrollPrecision
{
    return _precision;
}

auto PlatformPointerEvent::Action::Scroll::SetPrecision(PointerScrollPrecision const precision) -> void
{
    _precision = precision;
}

auto PlatformPointerEvent::Action::Scroll::GetModifiers() const -> ModifierKeyFlags
{
    return _modifiers;
}

auto PlatformPointerEvent::Action::Scroll::SetModifiers(ModifierKeyFlags const modifiers) -> void
{
    _modifiers = modifiers;
}
}

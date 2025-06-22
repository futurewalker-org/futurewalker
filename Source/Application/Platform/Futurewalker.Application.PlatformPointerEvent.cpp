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

auto PlatformPointerEvent::Wheel::GetDeltaX() const -> Float64
{
    return _deltaX;
}
auto PlatformPointerEvent::Wheel::SetDeltaX(Float64 const deltaX) -> void
{
    _deltaX = deltaX;
}

auto PlatformPointerEvent::Wheel::GetDeltaY() const -> Float64
{
    return _deltaY;
}
auto PlatformPointerEvent::Wheel::SetDeltaY(Float64 const deltaY) -> void
{
    _deltaY = deltaY;
}

auto PlatformPointerEvent::Wheel::GetPrecision() const -> PointerWheelPrecision
{
    return _precision;
}

auto PlatformPointerEvent::Wheel::SetPrecision(PointerWheelPrecision const precision) -> void
{
    _precision = precision;
}

///
/// @brief
///
/// @return
///
auto PlatformPointerEvent::Motion::GetTiltX() const -> Degree
{
    return _tiltX;
}

auto PlatformPointerEvent::Motion::SetTiltX(Degree const& tiltX) -> void
{
    _tiltX = tiltX;
}

auto PlatformPointerEvent::Motion::GetTiltY() const -> Degree
{
    return _tiltY;
}

auto PlatformPointerEvent::Motion::SetTiltY(Degree const& tiltY) -> void
{
    _tiltY = tiltY;
}

///
/// @brief Get azimuth of pointer.
///
auto PlatformPointerEvent::Motion::GetAzimuth() const -> Degree
{
    return _azimuth;
}

auto PlatformPointerEvent::Motion::SetAzimuth(Degree const& azimuth) -> void
{
    _azimuth = azimuth;
}

///
/// @brief Get altitude of pointer.
///
auto PlatformPointerEvent::Motion::GetAltitude() const -> Degree
{
    return _altitude;
}

auto PlatformPointerEvent::Motion::SetAltitude(Degree const& altitude) -> void
{
    _altitude = altitude;
}

///
/// @brief Get twist of pointer.
///
auto PlatformPointerEvent::Motion::GetTwist() const -> Degree
{
    return _twist;
}

///
/// @brief
///
/// @param twist
///
/// @return
///
auto PlatformPointerEvent::Motion::SetTwist(Degree const& twist) -> void
{
    _twist = twist;
}

///
/// @brief
///
/// @return
///
auto PlatformPointerEvent::Motion::GetPressure() const -> Float64
{
    return _pressure;
}

///
/// @brief
///
/// @param pressure
///
/// @return
///
auto PlatformPointerEvent::Motion::SetPressure(Float64 const pressure) -> void
{
    _pressure = pressure;
}

auto PlatformPointerEvent::Motion::GetTangentialPressure() const -> Float64
{
    return _tangentialPressure;
}

auto PlatformPointerEvent::Motion::SetTangentialPressure(Float64 const pressure) -> void
{
    _tangentialPressure = pressure;
}

auto PlatformPointerEvent::Motion::GetButton() const -> PointerButton
{
    return _button;
}

auto PlatformPointerEvent::Motion::SetButton(PointerButton const button) -> void
{
    _button = button;
}

auto PlatformPointerEvent::Gesture::GetPhase() const -> PointerGesturePhase
{
    return _phase;
}

auto PlatformPointerEvent::Gesture::SetPhase(PointerGesturePhase const phase) -> void
{
    _phase = phase;
}

auto PlatformPointerEvent::Gesture::Magnify::GetScale() const -> Float64
{
    return _scale;
}

auto PlatformPointerEvent::Gesture::Magnify::SetScale(Float64 const scale) -> void
{
    _scale = scale;
}

auto PlatformPointerEvent::Gesture::Rotate::GetRotation() const -> Degree
{
    return _rotation;
}

auto PlatformPointerEvent::Gesture::Rotate::SetRotation(Degree const rotation) -> void
{
    _rotation = rotation;
}

auto PlatformPointerEvent::Gesture::Swipe::GetDeltaX() const -> Dp
{
    return _deltaX;
}

auto PlatformPointerEvent::Gesture::Swipe::SetDeltaX(Dp const deltaX) -> void
{
    _deltaX = deltaX;
}

auto PlatformPointerEvent::Gesture::Swipe::GetDeltaY() const -> Dp
{
    return _deltaY;
}

auto PlatformPointerEvent::Gesture::Swipe::SetDeltaY(Dp const deltaY) -> void
{
    _deltaY = deltaY;
}

auto PlatformPointerEvent::Gesture::Scroll::GetDeltaX() const -> Dp
{
    return _deltaX;
}
auto PlatformPointerEvent::Gesture::Scroll::SetDeltaX(Dp const deltaX) -> void
{
    _deltaX = deltaX;
}

auto PlatformPointerEvent::Gesture::Scroll::GetDeltaY() const -> Dp
{
    return _deltaY;
}
auto PlatformPointerEvent::Gesture::Scroll::SetDeltaY(Dp const deltaY) -> void
{
    _deltaY = deltaY;
}

auto PlatformPointerEvent::Gesture::Scroll::IsInertial() const -> Bool
{
    return _inertial; 
}
auto PlatformPointerEvent::Gesture::Scroll::SetInertial(Bool const inertial) -> void
{
    _inertial = inertial;
}
}

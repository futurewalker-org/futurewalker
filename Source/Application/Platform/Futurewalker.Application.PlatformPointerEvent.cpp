// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.PlatformPointerEvent.hpp"

namespace FW_DETAIL_NS
{
PlatformPointerEvent::PlatformPointerEvent(PlatformPointerEventType const eventType)
  : _eventType {eventType}
{
}

auto PlatformPointerEvent::GetEventType() const -> PlatformPointerEventType
{
    return _eventType;
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
auto PlatformPointerEvent::GetTiltX() const -> Degree
{
    return _tiltX;
}

auto PlatformPointerEvent::SetTiltX(Degree const& tiltX) -> void
{
    _tiltX = tiltX;
}

auto PlatformPointerEvent::GetTiltY() const -> Degree
{
    return _tiltY;
}

auto PlatformPointerEvent::SetTiltY(Degree const& tiltY) -> void
{
    _tiltY = tiltY;
}

///
/// @brief Get azimuth of pointer.
///
auto PlatformPointerEvent::GetAzimuth() const -> Degree
{
    return _azimuth;
}

auto PlatformPointerEvent::SetAzimuth(Degree const& azimuth) -> void
{
    _azimuth = azimuth;
}

///
/// @brief Get altitude of pointer.
///
auto PlatformPointerEvent::GetAltitude() const -> Degree
{
    return _altitude;
}

auto PlatformPointerEvent::SetAltitude(Degree const& altitude) -> void
{
    _altitude = altitude;
}

///
/// @brief Get twist of pointer.
///
auto PlatformPointerEvent::GetTwist() const -> Degree
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
auto PlatformPointerEvent::SetTwist(Degree const& twist) -> void
{
    _twist = twist;
}

///
/// @brief 
///
/// @return 
///
auto PlatformPointerEvent::GetPressure() const -> Float64
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
auto PlatformPointerEvent::SetPressure(Float64 const pressure) -> void
{
    _pressure = pressure;
}
}

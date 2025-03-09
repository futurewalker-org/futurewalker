// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.PointerEvent.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
/// @param eventType
///
PointerEvent::PointerEvent(PointerEventType const eventType)
  : _eventType {eventType}
{
}

///
/// @brief
///
auto PointerEvent::GetEventType() const -> PointerEventType
{
    return _eventType;
}

///
/// @brief
///
/// @param eventType
///
auto PointerEvent::SetEventType(PointerEventType const eventType) -> void
{
    _eventType = eventType;
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
auto PointerEvent::GetTiltX() const -> Degree
{
    return _tiltX;
}

///
/// @brief
///
/// @param tiltX
///
auto PointerEvent::SetTiltX(Degree const tiltX) -> void
{
    _tiltX = tiltX;
}

///
/// @brief
///
auto PointerEvent::GetTiltY() const -> Degree
{
    return _tiltY;
}

///
/// @brief
///
/// @param tiltY
///
auto PointerEvent::SetTiltY(Degree const tiltY) -> void
{
    _tiltY = tiltY;
}

///
/// @brief
///
auto PointerEvent::GetAzimuth() const -> Degree
{
    return _azimuth;
}

///
/// @brief
///
/// @param azimuth
///
auto PointerEvent::SetAzimuth(Degree const azimuth) -> void
{
    _azimuth = azimuth;
}

///
/// @brief
///
auto PointerEvent::GetAltitude() const -> Degree
{
    return _altitude;
}

///
/// @brief
///
/// @param altitude
///
auto PointerEvent::SetAltitude(Degree const altitude) -> void
{
    _altitude = altitude;
}

///
/// @brief
///
auto PointerEvent::GetTwist() const -> Degree
{
    return _twist;
}

///
/// @brief
///
/// @param twist
///
auto PointerEvent::SetTwist(Degree const twist) -> void
{
    _twist = twist;
}

///
/// @brief 
///
auto PointerEvent::GetPressure() const -> Float64
{
    return _pressure;
}

///
/// @brief
///
/// @param pressure
///
auto PointerEvent::SetPressure(Float64 const pressure) -> void
{
    _pressure = pressure;
}
}
}

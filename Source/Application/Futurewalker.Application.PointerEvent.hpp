// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.PointerEventType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

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
    explicit PointerEvent(PointerEventType const type);

    auto GetEventType() const -> PointerEventType;
    auto SetEventType(PointerEventType const eventType) -> void;

    auto GetPointerId() const -> PointerId;
    auto SetPointerId(PointerId const id) -> void;

    auto GetPointerType() const -> PointerType;
    auto SetPointerType(PointerType const type) -> void;

    auto GetPosition() const -> Point<Dp>;
    auto SetPosition(Point<Dp> const& position) -> void;

    auto GetTimestamp() const -> MonotonicTime;
    auto SetTimestamp(MonotonicTime const timestamp) -> void;

    auto IsPrimaryPointer() const -> Bool;
    auto SetPrimaryPointer(Bool const isPrimaryPointer) -> void;

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

private:
    PointerEventType _eventType;
    PointerId _id = 0U;
    PointerType _type = PointerType::Other;
    Point<Dp> _position;
    MonotonicTime _timestamp;
    Bool _isPrimaryPointer = false;
    Degree _azimuth = 0;
    Degree _altitude = 0;
    Degree _twist = 0;
    Degree _tiltX = 0;
    Degree _tiltY = 0;
    Float64 _pressure = 0;
};
}
}

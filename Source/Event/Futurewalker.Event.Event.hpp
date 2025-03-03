// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Event.EventType.hpp"
#include "Futurewalker.Event.EventParameter.hpp"
#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.PropertyStore.hpp"
#include "Futurewalker.Core.Exception.hpp"
#include "Futurewalker.Core.DynamicCastFunction.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Event class.
///
class Event
{
public:
    Event() noexcept = default;
    Event(const Event& other);
    Event(Event&& other) noexcept;
    Event& operator=(const Event& other);
    Event& operator=(Event&& other) noexcept;

    template <Concepts::DerivedFrom<EventParameter> EventType>
    explicit Event(const EventType& event);

    template <class EventType>
    auto Is() const noexcept -> Bool;

    template <class EventType>
    auto As() -> EventType&;

    template <class EventType>
    auto As() const -> const EventType&;

    auto GetPropertySore() -> PropertyStore&;
    auto GetPropertySore() const -> const PropertyStore&;

private:
    struct Holder;

    template <class EventType>
    struct EventTypeHolder;

private:
    std::unique_ptr<Holder> _holder;
    PropertyStore _propertyStore;
};

///
/// @brief Internal event holder type.
///
struct Event::Holder
{
    virtual ~Holder() noexcept = default;
    virtual auto GetEvent() -> EventParameter& = 0;
    virtual auto Clone() -> std::unique_ptr<Holder> = 0;
};

///
/// @brief Internal event holder type.
///
template <class EventType>
struct Event::EventTypeHolder final : Holder
{
    EventType event;

    EventTypeHolder()
      : event {}
    {
    }

    EventTypeHolder(const EventType& event)
      : event {event}
    {
    }

    auto GetEvent() -> EventParameter& override
    {
        return event;
    }

    auto Clone() -> std::unique_ptr<Holder> override
    {
        return std::make_unique<EventTypeHolder>(*this);
    }
};

///
/// @brief Construct event from event type.
///
/// @param[in] event Event value.
///
template <Concepts::DerivedFrom<EventParameter> EventType>
Event::Event(const EventType& event)
{
    _holder = std::make_unique<EventTypeHolder<EventType>>(event);
}

///
/// @brief Check type of contained event value.
///
template <class EventType>
auto Event::Is() const noexcept -> Bool
{
    if (_holder)
    {
        return DynamicCastFunction::Is<EventType>(_holder->GetEvent());
    }
    return false;
}

///
/// @brief Access contained event value.
///
/// @throw Exception on type miss match.
///
template <class EventType>
auto Event::As() -> EventType&
{
    if (_holder)
    {
        return DynamicCastFunction::As<EventType>(_holder->GetEvent());
    }
    throw Exception(ErrorCode::Failure, "Bad event cast");
}

///
/// @brief Access contained event value.
///
/// @throw Exception on type miss match.
///
template <class EventType>
auto Event::As() const -> const EventType&
{
    if (_holder)
    {
        return DynamicCastFunction::As<EventType>(_holder->GetEvent());
    }
    throw Exception(ErrorCode::Failure, "Bad event cast");
}
}
}

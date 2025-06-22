// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Event.EventType.hpp"
#include "Futurewalker.Event.EventParameter.hpp"
#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.PropertyStore.hpp"
#include "Futurewalker.Core.Exception.hpp"
#include "Futurewalker.Core.DynamicCastFunction.hpp"

namespace FW_DETAIL_NS
{
struct EventParameterHolder
{
    virtual ~EventParameterHolder() noexcept = 0;
    virtual auto GetEvent() -> EventParameter& = 0;
    virtual auto Clone() -> std::unique_ptr<EventParameterHolder> = 0;
};

namespace FW_EXPORT
{
///
/// @brief Polymorphic value holder for events.
///
template <Concepts::DerivedFrom<EventParameter> Parameter>
class Event
{
    template <Concepts::DerivedFrom<EventParameter> U>
    friend class Event;

public:
    ///
    /// @brief Construct event.
    ///
    /// @param[in] args Parameters for constructor of Parameter.
    ///
    template <class... Args>
    static auto Make(Args&&... args) -> Event<Parameter>
    {
        return Event(std::make_unique<EventTypeHolder<Parameter>>(std::in_place, std::forward<Args>(args)...));
    }

    ///
    /// @brief Default constructor.
    ///
    Event()
      : _holder(std::make_unique<EventTypeHolder<Parameter>>())
    {
    }

    ///
    /// @brief Construct event from another event type.
    ///
    template <Concepts::DerivedFrom<Parameter> T>
    Event(Event<T> const& other)
      : _holder(other._holder->Clone())
      , _propertyStore(other._propertyStore)
    {
    }

    ///
    /// @brief Assign from another event type.
    ///
    template <Concepts::DerivedFrom<Parameter> T>
    Event& operator=(Event<T> const& other)
    {
        _holder = other._holder->Clone();
        _propertyStore = other._propertyStore;
        return *this;
    }

    ///
    /// @brief Copy constructor.
    ///
    Event(Event const& other)
      : _holder(other._holder->Clone())
      , _propertyStore(other._propertyStore)
    {
    }

    ///
    /// @brief Assign from another event type.
    ///
    Event& operator=(Event const& other)
    {
        if (this != &other)
        {
            _holder.reset();
            _holder = other._holder->Clone();
            _propertyStore = other._propertyStore;
        }
        return *this;
    }

    ///
    /// @brief Check type of contained event value.
    ///
    template <Concepts::DerivedFrom<EventParameter> T>
    auto Is() const noexcept -> Bool
    {
        return DynamicCastFunction::Is<T>(_holder->GetEvent());
    }

    ///
    /// @brief Access contained event value.
    ///
    template <Concepts::DerivedFrom<EventParameter> T>
    auto As() const -> Event<T>
    {
        if (Is<T>())
        {
            return Event<T>(_holder->Clone());
        }
        throw Exception(ErrorCode::Failure, "Bad event cast");
    }

    ///
    /// @brief Get property store.
    ///
    auto GetPropertySore() -> PropertyStore&
    {
        return _propertyStore;
    }

    ///
    /// @brief Get property store.
    ///
    auto GetPropertySore() const -> PropertyStore const&
    {
        return _propertyStore;
    }

    ///
    /// @brief Access contained event value.
    ///
    auto operator->() const noexcept -> Parameter const*
    {
        return static_cast<Parameter const*>(&_holder->GetEvent());
    }

    ///
    /// @brief Access contained event value.
    ///
    auto operator->() noexcept -> Parameter*
    {
        return static_cast<Parameter*>(&_holder->GetEvent());
    }

    ///
    /// @brief Access contained event value.
    ///
    auto operator*() const noexcept -> Parameter const&
    {
        return static_cast<Parameter const&>(_holder->GetEvent());
    }

    ///
    /// @brief Access contained event value.
    ///
    auto operator*() noexcept -> Parameter&
    {
        return static_cast<Parameter&>(_holder->GetEvent());
    }

private:
    template <class EventType>
    struct EventTypeHolder final : EventParameterHolder
    {
        EventType event;

        template <class... Args>
        EventTypeHolder(std::in_place_t, Args&&... args)
          : event {std::forward<Args>(args)...}
        {
        }

        EventTypeHolder()
          : event {}
        {
        }

        EventTypeHolder(EventType const& event)
          : event {event}
        {
        }

        auto GetEvent() -> EventParameter& override
        {
            return event;
        }

        auto Clone() -> std::unique_ptr<EventParameterHolder> override
        {
            return std::make_unique<EventTypeHolder>(*this);
        }
    };

    explicit Event(std::unique_ptr<EventParameterHolder> holder)
      : _holder(std::move(holder))
    {
    }

private:
    std::unique_ptr<EventParameterHolder> _holder;
    PropertyStore _propertyStore;
};
}
}

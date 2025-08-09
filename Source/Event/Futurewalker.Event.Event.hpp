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
namespace FW_EXPORT
{
template <>
class Event<EventParameter>
{
public:
    using ParameterType = EventParameter;

    ///
    /// @brief Default constructor.
    ///
    Event()
      : Event(std::make_unique<HolderT<EventParameter>>())
    {
    }

    ///
    /// @brief Copy constructor.
    ///
    Event(Event const& other)
      : Event(other._holder->Clone())
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
        }
        return *this;
    }

    ///
    /// @brief Check type of contained event value.
    ///
    template <Concepts::DerivedFrom<EventParameter> T>
    auto Is() const noexcept -> Bool
    {
        return DynamicCastFunction::Is<T>(_holder->Get());
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
    /// @brief Access contained event value.
    ///
    template <class Parameter>
    auto operator->(this const Event<Parameter>& self) noexcept -> Parameter const*
    {
        return static_cast<Parameter const*>(&self._holder->Get());
    }

    ///
    /// @brief Access contained event value.
    ///
    template <class Parameter>
    auto operator->(this const Event<Parameter>&& self) noexcept -> Parameter const*
    {
        return static_cast<Parameter const*>(&self._holder->Get());
    }

    ///
    /// @brief Access contained event value.
    ///
    template <class Parameter>
    auto operator->(this Event<Parameter>& self) noexcept -> Parameter*
    {
        return static_cast<Parameter*>(&self._holder->Get());
    }

    ///
    /// @brief Access contained event value.
    ///
    template <class Parameter>
    auto operator->(this Event<Parameter>&& self) noexcept -> Parameter*
    {
        return static_cast<Parameter*>(&self._holder->Get());
    }

    ///
    /// @brief Access contained event value.
    ///
    template <class Parameter>
    auto operator*(this const Event<Parameter>& self) noexcept -> Parameter const&
    {
        return static_cast<Parameter const&>(self._holder->Get());
    }

    ///
    /// @brief Access contained event value.
    ///
    template <class Parameter>
    auto operator*(this const Event<Parameter>&& self) noexcept -> Parameter const&
    {
        return static_cast<Parameter const&>(self._holder->Get());
    }

    ///
    /// @brief Access contained event value.
    ///
    template <class Parameter>
    auto operator*(this Event<Parameter>& self) noexcept -> Parameter&
    {
        return static_cast<Parameter&>(self._holder->Get());
    }

    ///
    /// @brief Access contained event value.
    ///
    template <class Parameter>
    auto operator*(this Event<Parameter>&& self) noexcept -> Parameter&
    {
        return static_cast<Parameter&>(self._holder->Get());
    }

protected:
    struct Holder
    {
        virtual ~Holder() noexcept = 0;
        virtual auto Get() -> EventParameter& = 0;
        virtual auto Clone() -> std::unique_ptr<Holder> = 0;
    };

    template <class EventType>
    struct HolderT final : Holder
    {
        EventType event;

        template <class... Args>
        HolderT(std::in_place_t, Args&&... args)
          : event {std::forward<Args>(args)...}
        {
        }

        HolderT()
          : event {}
        {
        }

        HolderT(EventType const& event)
          : event {event}
        {
        }

        auto Get() -> EventParameter& override
        {
            return event;
        }

        auto Clone() -> std::unique_ptr<Holder> override
        {
            return std::make_unique<HolderT>(*this);
        }
    };

    explicit Event(std::unique_ptr<Holder> holder)
      : _holder(std::move(holder))
    {
    }

private:
    std::unique_ptr<Holder> _holder;
};

///
/// @brief Polymorphic value holder for events.
///
template <Concepts::DerivedFrom<EventParameter> Parameter>
class Event final : public Event<EventParameter>
{
    friend class Event<EventParameter>;

public:
    using ParameterType = Parameter;

    ///
    /// @brief Construct event.
    ///
    /// @param[in] args Parameters for constructor of Parameter.
    ///
    template <class... Args>
    static auto Make(Args&&... args) -> Event<Parameter>
    {
        return Event(std::make_unique<HolderT<Parameter>>(std::in_place, std::forward<Args>(args)...));
    }

    ///
    /// @brief Default constructor.
    ///
    Event()
      : Event<>(std::make_unique<HolderT<Parameter>>())
    {
    }

    Event(Event<> const& other)
      : Event<>(other)
    {
    }

    Event& operator=(Event<> const& other)
    {
        return static_cast<Event&>(Event<>::operator=(other));
    }

protected:
    explicit Event(std::unique_ptr<Holder> holder)
      : Event<>(std::move(holder))
    {
    }
};
}
}

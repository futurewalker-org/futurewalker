// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Event.EventType.hpp"
#include "Futurewalker.Event.EventParameter.hpp"
#include "Futurewalker.Event.EventParameterHolder.hpp"

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.PropertyStore.hpp"
#include "Futurewalker.Core.Exception.hpp"
#include "Futurewalker.Core.DynamicCastFunction.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Polymorphic container for event parameters.
///
/// @tparam Parameter Type of event parameter.
///
template <Concepts::DerivedFrom<EventParameter> Parameter>
class Event final
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
        return Event(std::make_unique<EventParameterHolderT<Parameter>>(std::in_place, std::forward<Args>(args)...));
    }

    ///
    /// @brief Copy constructor from another event type.
    ///
    /// @param[in] other Other event.
    ///
    template <Concepts::DerivedFrom<Parameter> U>
    Event(Event<U> const& other)
      : Event(other._holder->Clone())
    {
    }

    ///
    /// @brief Construct event with default-constructed parameter.
    ///
    Event()
      : Event(std::make_unique<EventParameterHolderT<Parameter>>())
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
    auto operator=(Event const& other) -> Event&
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
    /// @brief Get contained event value.
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
    /// @brief Try to get contained event value.
    ///
    template <Concepts::DerivedFrom<EventParameter> T>
    auto TryAs() const -> Optional<Event<T>>
    {
        if (Is<T>())
        {
            return Event<T>(_holder->Clone());
        }
        return {};
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

private:
    explicit Event(std::unique_ptr<EventParameterHolder> holder)
      : _holder(std::move(holder))
    {
    }
    std::unique_ptr<EventParameterHolder> _holder;
};
}
}

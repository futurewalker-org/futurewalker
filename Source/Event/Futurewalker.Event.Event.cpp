// SPDX-License-Identifier: MIT

#include "Futurewalker.Event.Event.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Copy constructor.
///
Event::Event(const Event& other)
  : _propertyStore {other._propertyStore}
{
    if (other._holder)
    {
        _holder = other._holder->Clone();
    }
}

///
/// @brief Move constructor.
///
Event::Event(Event&& other) noexcept
  : Event()
{
    std::swap(_holder, other._holder);
    std::swap(_propertyStore, other._propertyStore);
}

///
/// @brief operator=
///
Event& Event::operator=(const Event& other)
{
    if (other._holder)
    {
        _holder = other._holder->Clone();
    }
    _propertyStore = other._propertyStore;
    return *this;
}

///
/// @brief operator=
///
Event& Event::operator=(Event&& other) noexcept
{
    Event tmp = std::move(other);
    std::swap(_holder, other._holder);
    return *this;
}

///
/// @brief Get property store.
///
auto Event::GetPropertySore() -> PropertyStore&
{
    return _propertyStore;
}

///
/// @brief Get property store.
///
auto Event::GetPropertySore() const -> const PropertyStore&
{
    return _propertyStore;
}
}

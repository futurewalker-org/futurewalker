// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Event.EventParameterType.hpp"

#include "Futurewalker.Core.DynamicCastFunction.hpp"
#include "Futurewalker.Core.Pointer.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Base class of events.
///
class EventParameter
{
public:
    EventParameter() = default;
    EventParameter(EventParameter const&) = default;
    EventParameter& operator=(EventParameter const&) = default;
    virtual ~EventParameter() noexcept;

    template <class T>
    auto Is() const -> Bool
    {
        return DynamicCastFunction::Is<T const>(this);
    }

    template <class T>
    auto As() -> T&
    {
        return DynamicCastFunction::As<T>(*this);
    }

    template <class T>
    auto As() const -> T const&
    {
        return DynamicCastFunction::As<T const>(*this);
    }

    template <class T>
    auto TryAs() -> Pointer<T>
    {
        return DynamicCastFunction::TryAs<T>(this);
    }

    template <class T>
    auto TryAs() const -> Pointer<T const>
    {
        return DynamicCastFunction::TryAs<T const>(this);
    }
};
}
}

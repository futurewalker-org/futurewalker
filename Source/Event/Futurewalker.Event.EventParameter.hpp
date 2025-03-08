// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Event.EventParameterType.hpp"

#include "Futurewalker.Core.DynamicCastFunction.hpp"

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
    virtual ~EventParameter() noexcept = 0;

    template <class T>
    auto Is() const -> Bool
    {
        return DynamicCastFunction::Is<T>(this);
    }

    template <class Self, class T>
    auto As(this Self& self) -> T&
    {
        return DynamicCastFunction::As<T>(*this);
    }
};
}
}

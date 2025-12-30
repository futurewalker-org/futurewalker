// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Event.EventParameterType.hpp"

#include <memory>
#include <utility>

namespace FW_DETAIL_NS
{
struct EventParameterHolder
{
    virtual ~EventParameterHolder() noexcept = 0;
    virtual auto Get() -> EventParameter& = 0;
    virtual auto Clone() -> std::unique_ptr<EventParameterHolder> = 0;
};

template <class EventParameterType>
struct EventParameterHolderT final : EventParameterHolder
{
    EventParameterType event;

    template <class... Args>
    EventParameterHolderT(std::in_place_t, Args&&... args)
      : event {std::forward<Args>(args)...}
    {
    }

    EventParameterHolderT()
      : event {}
    {
    }

    EventParameterHolderT(EventParameterType const& event)
      : event {event}
    {
    }

    auto Get() -> EventParameter& override
    {
        return event;
    }

    auto Clone() -> std::unique_ptr<EventParameterHolder> override
    {
        return std::make_unique<EventParameterHolderT>(*this);
    }
};
}

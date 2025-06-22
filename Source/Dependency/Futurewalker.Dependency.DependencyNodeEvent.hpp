// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Dependency.DependencyNodeEventType.hpp"
#include "Futurewalker.Dependency.DependencyNodeType.hpp"

#include "Futurewalker.Event.EventParameter.hpp"
#include "Futurewalker.Event.Event.hpp"

#include "Futurewalker.Core.PropertyStore.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT 
{
///
/// @brief Dependency node event.
///
class DependencyNodeEvent : public EventParameter
{
public:
    class Notify;
    class ParentChanged;
};

///
/// @brief Notification from dependency node.
///
class DependencyNodeEvent::Notify final : public DependencyNodeEvent
{
public:
    auto GetSender() const -> Shared<DependencyNode>;
    auto SetSender(Shared<DependencyNode> const& sender) -> void;

    auto GetEvent() const -> Event const&;
    auto SetEvent(Event const& event) -> void;

private:
    Weak<DependencyNode> _sender;
    Event _event;
};
}
}

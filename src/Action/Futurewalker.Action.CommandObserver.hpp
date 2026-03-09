// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Action.CommandObserverType.hpp"
#include "Futurewalker.Action.CommandNodeType.hpp"
#include "Futurewalker.Action.CommandDispatcherContextType.hpp"
#include "Futurewalker.Action.CommandId.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

#include "Futurewalker.Core.Primitive.hpp"
#include "Futurewalker.Core.Memory.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Observable object for tracking command state.
///
class CommandObserver : NonCopyable
{
public:
    CommandObserver(PassKey<CommandNode>, Shared<CommandNode> context, CommandId const& commandId);

    auto GetCommandId() const -> CommandId;
    auto IsCommandEnabled() const -> Bool;
    auto IsCommandToggled() const -> Bool;

    auto GetTracker() -> Tracker&;
    auto GetTracker() const -> Tracker const&;

    auto GetEventReceiver() -> EventReceiver&;
    auto GetEventReceiver() const -> EventReceiver const&;

private:
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;

private:
    Weak<CommandNode> _commandNode;
    CommandId _commandId;
    Unique<EventReceiver> _eventReceiver;
};
}
}

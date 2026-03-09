// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Dependency.DependencyNodeEvent.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Get sender node.
///
auto DependencyNodeEvent::Notify::GetSender() const -> Shared<DependencyNode>
{
    return _sender.Lock();
}

///
/// @brief Set sender node.
///
auto DependencyNodeEvent::Notify::SetSender(Shared<DependencyNode> const& sender) -> void
{
    _sender = sender;
}

///
/// @brief Get event.
///
auto DependencyNodeEvent::Notify::GetEvent() const -> Event<> const&
{
    return _event;
}

///
/// @brief Set event.
///
auto DependencyNodeEvent::Notify::SetEvent(Event<> const& event) -> void
{
    _event = event;
}
}

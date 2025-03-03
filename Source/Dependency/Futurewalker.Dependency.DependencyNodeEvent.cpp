// SPDX-License-Identifier: MIT

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
auto DependencyNodeEvent::Notify::SetSender(const Shared<DependencyNode>& sender) -> void
{
    _sender = sender;
}

///
/// @brief Get event.
///
auto DependencyNodeEvent::Notify::GetEvent() const ->  const Event&
{
    return _event;
}

///
/// @brief Set event.
///
auto DependencyNodeEvent::Notify::SetEvent(const Event& event) -> void
{
    _event = event;
}
}

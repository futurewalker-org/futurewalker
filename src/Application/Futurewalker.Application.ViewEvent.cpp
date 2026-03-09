// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ViewEvent.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
auto ViewEvent::ChildRemoved::GetRemovedView() -> Shared<View>
{
    return _removedView.Lock();
}

///
/// @brief
///
auto ViewEvent::ChildRemoved::SetRemovedView(Shared<View> const& view) -> void
{
    _removedView = view;
}

///
/// @brief
///
auto ViewEvent::Notify::GetSender() const -> Shared<View>
{
    return _sender.Lock();
}

///
/// @brief
///
auto ViewEvent::Notify::SetSender(Shared<View> const& sender) -> void
{
    _sender = sender;
}
}

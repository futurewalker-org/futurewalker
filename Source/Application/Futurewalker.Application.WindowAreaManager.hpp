// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.WindowAreaManagerType.hpp"

#include "Futurewalker.Event.EventReceiverType.hpp"
#include "Futurewalker.Event.EventReceiverType.hpp"
#include "Futurewalker.Event.EventParameter.hpp"

#include "Futurewalker.Async.Async.hpp"

#include "Futurewalker.Core.Function.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.PassKey.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Unit.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Window area manager.
///
class WindowAreaManager : NonCopyable
{
public:
    static auto Make() -> Shared<WindowAreaManager>;

    WindowAreaManager(PassKey<WindowAreaManager>);

    auto GetTitleBarAreaRect() const -> Rect<Dp>;
    auto SetTitleBarAreaRect(Rect<Dp> const& rect) -> void;

    auto GetTitleBarAvailableAreaRect() const -> Rect<Dp>;
    auto SetTitleBarAvailableAreaRect(Rect<Dp> const& rect) -> void;

    auto GetContentAreaRect() const -> Rect<Dp>;
    auto SetContentAreaRect(Rect<Dp> const& rect) -> void;

    auto GetEventReceiver() -> EventReceiver&;
    auto GetEventReceiver() const -> EventReceiver const&;

private:
    Shared<EventReceiver> _eventReceiver;
    Rect<Dp> _titleBarAreaRect;
    Rect<Dp> _titleBarAvailableAreaRect;
    Rect<Dp> _contentAreaRect;
};
}
}

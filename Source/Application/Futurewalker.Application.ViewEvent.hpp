// SPDX-License-Identifier: MIT
#pragma once

#include "Futurewalker.Application.ViewEventType.hpp"
#include "Futurewalker.Application.ViewType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Event.Event.hpp"
#include "Futurewalker.Core.PropertyStore.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief View event.
///
class ViewEvent : public EventParameter
{
public:
    ViewEvent() = default;
    ViewEvent(ViewEvent const&) = default;
    ViewEvent& operator=(ViewEvent const&) = default;

public:
    class VisibleChanged;
    class EnabledChanged;
    class ActiveChanged;
    class DisplayScaleChanged;
    class BackingScaleChanged;
    class LayoutDirectionChanged;
    class Attaching;
    class Attached;
    class Detaching;
    class Detached;
    class ChildRemoved;
    class Notify;
};

///
/// @brief
///
class ViewEvent::VisibleChanged : public ViewEvent
{
};

///
/// @brief
///
class ViewEvent::EnabledChanged : public ViewEvent
{
};

///
/// @brief
///
class ViewEvent::ActiveChanged : public ViewEvent
{
};

///
/// @brief
///
class ViewEvent::DisplayScaleChanged : public ViewEvent
{
};

///
/// @brief
///
class ViewEvent::BackingScaleChanged : public ViewEvent
{
};

///
/// @brief
///
class ViewEvent::LayoutDirectionChanged : public ViewEvent
{
};

///
/// @brief
///
class ViewEvent::Attaching : public ViewEvent
{
};

///
/// @brief
///
class ViewEvent::Attached : public ViewEvent
{
};

///
/// @brief
///
class ViewEvent::Detaching : public ViewEvent
{
};

///
/// @brief
///
class ViewEvent::Detached : public ViewEvent
{
};

///
/// @brief
///
class ViewEvent::ChildRemoved : public ViewEvent
{
public:
    auto GetRemovedView() -> Shared<View>;
    auto SetRemovedView(Shared<View> const& view) -> void;

private:
    Weak<View> _removedView;
};

///
/// @brief
///
class ViewEvent::Notify : public ViewEvent
{
public:
    auto GetSenderView() const -> Shared<View>;
    auto SetSenderView(Shared<View> const& senderView) -> void;

private:
    Weak<View> _senderView;
};
}
}

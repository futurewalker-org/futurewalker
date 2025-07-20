// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Attribute.AttributeObserverType.hpp"
#include "Futurewalker.Attribute.AttributeNode.hpp"
#include "Futurewalker.Attribute.StaticAttribute.hpp"
#include "Futurewalker.Attribute.AttributeEvent.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.PassKey.hpp"
#include "Futurewalker.Core.Memory.hpp"
#include "Futurewalker.Core.StaticReference.hpp" 
#include "Futurewalker.Core.Optional.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Attribute observer.
///
/// An interfaces to read/write specific attributes in AttributeNode.
///
/// #### Ownership
///
/// AttributeObserver weakly references AttributeNode.
///
template <class T>
class AttributeObserver : NonCopyable
{
public:
    ///
    /// @brief Construct observer from AttributeNode.
    ///
    /// @param node AttributeNode.
    /// @param description Description of the attribute.
    ///
    AttributeObserver(PassKey<AttributeNode>, Weak<AttributeNode> node, StaticAttributeRef<T> description)
      : _node {node}
      , _description {description}
    {
        _eventReceiver = EventReceiver::Make();

        if (auto const attributeNode = _node.Lock())
        {
            _signalConnection = EventReceiver::Connect(*attributeNode, *this, &AttributeObserver::ReceiveEvent);
        }
    }

    ///
    /// @brief Get description of the attribute being observed.
    ///
    auto GetDescription() const -> StaticAttributeRef<T>
    {
        return _description;
    }

    ///
    /// @brief Get value of the attribute.
    ///
    /// @return Value of the attribute.
    ///
    auto GetValue() const -> Optional<T>
    {
        if (auto const node = _node.Lock())
        {
            return AttributeNode::GetValue<T>(*node, _description);
        }
        return {};
    }

    ///
    /// @brief Set value of the attribute.
    ///
    /// @param value New value of the attribute.
    ///
    auto SetValue(T const& value) -> void
    {
        if (auto const node = _node.Lock())
        {
            AttributeNode::SetValue<T>(*node, _description, value);
        }
    }

    ///
    /// @brief Set reference of the attribute.
    ///
    /// @param reference Reference of other attribute.
    ///
    auto SetReference(StaticAttributeRef<T> reference) -> void
    {
        if (auto const node = _node.Lock())
        {
            AttributeNode::SetReference(*node, _description, reference);
        }
    }

    ///
    /// @brief Get tracker.
    ///
    auto GetTracker() -> Tracker&
    {
        return _eventReceiver->GetTracker();
    }

    ///
    /// @brief Get tracker.
    ///
    auto GetTracker() const -> Tracker const&
    {
        return _eventReceiver->GetTracker();
    }

    ///
    /// @brief Get event receiver.
    ///
    auto GetEventReceiver() -> EventReceiver& 
    {
        return *_eventReceiver;
    }

    ///
    /// @brief Get event receiver.
    ///
    auto GetEventReceiver() const -> EventReceiver const&
    {
        return *_eventReceiver;
    }

private:
    auto ReceiveEvent(Event<>& event) -> Async<Bool>
    {
        if (event.Is<AttributeEvent::ValueChanged>())
        {
            auto const parameter = event.As<AttributeEvent::ValueChanged>();
            if (parameter->GetId() == _description.Get().GetId())
            {
                co_return co_await GetEventReceiver().SendEvent(event);
            }
        }
        co_return false;
    }

private:
    Shared<EventReceiver> _eventReceiver;
    Weak<AttributeNode> _node;
    StaticAttributeRef<T> _description;
    SignalConnection _signalConnection;
};
}
}

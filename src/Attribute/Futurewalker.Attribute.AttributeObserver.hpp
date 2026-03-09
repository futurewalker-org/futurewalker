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
    }

    ///
    /// @brief Destructor.
    ///
    ~AttributeObserver()
    {
        for (auto& connection : _signalConnections)
        {
            connection.Disconnect();
        }
    }

    ///
    /// @brief Connect event handler.
    ///
    /// @param observer Event observer.
    /// @param function Event receiver function.
    ///
    /// @return Signal connection.
    ///
    template <class Observer, class Function>
    auto Connect(Observer&& observer, Function&& function) -> SignalConnection
    {
        if (auto const attributeNode = _node.Lock())
        {
            auto connection = AttributeNode::ConnectAttributeEvent(*attributeNode, _description, std::forward<Observer>(observer), std::forward<Function>(function));
            _signalConnections.push_back(connection);
            return connection;
        }
        return {};
    }

    ///
    /// @brief Get ID of the attribute being observed.
    ///
    auto GetAttributeId() const -> AttributeId
    {
        return _description.Get().GetId();
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

private:
    Weak<AttributeNode> _node;
    StaticAttributeRef<T> _description;
    std::vector<SignalConnection> _signalConnections;
};
}
}

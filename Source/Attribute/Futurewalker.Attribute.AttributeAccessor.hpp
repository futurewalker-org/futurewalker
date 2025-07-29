// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Attribute.AttributeObserver.hpp"
#include "Futurewalker.Attribute.AttributeNode.hpp"
#include "Futurewalker.Attribute.AttributeArg.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Proxy object to access to an attribute.
///
/// @tparam T Value type of attribute
///
template <class T>
class AttributeAccessor : NonCopyable
{
public:
    AttributeAccessor()
    {
        _eventReceiver = EventReceiver::Make();
    }

    ///
    /// @brief Bind accessor to an attribute.
    ///
    /// @param[in] owner Owner of AttributeNode
    /// @param[in] attribute Attribute to bind
    ///
    /// @return `true` on success
    ///
    /// @note This function will not send event.
    ///
    template <class Owner>
    auto BindAttribute(Owner& owner, StaticAttributeRef<T> attribute) -> Bool
    {
        UnbindAttribute();

        _observer = AttributeNode::GetObserver(owner, attribute);
        if (_observer)
        {
            _connection = EventReceiver::Connect(*_observer, *this, &AttributeAccessor::ReceiveEvent);
            return true;
        }
        return false;
    }

    ///
    /// @brief Bind accessor to an attribute with default value assigned.
    ///
    /// @param[in] owner Owner of AttributeNode
    /// @param[in] attribute Attribute to bind
    /// @param[in] value New value of the attribute 
    ///
    /// @return `true` on success
    ///
    /// @note This function will not send event.
    ///
    template <class Owner>
    auto BindAttributeWithDefault(Owner& owner, StaticAttributeRef<T> attribute, const T& value) -> Bool
    {
        if (BindAttribute(owner, attribute))
        {
            SetValue(value);
            return true;
        }
        return false;
    }

    ///
    /// @brief Bind accessor to an attribute with default reference assigned.
    ///
    /// @param[in] owner Owner of AttributeNode
    /// @param[in] attribute Attribute to bind
    /// @param[in] reference New value of the attribute
    ///
    /// @return `true` on success
    ///
    /// @note This function will not send event.
    ///
    template <class Owner>
    auto BindAttributeWithDefault(Owner& owner, StaticAttributeRef<T> attribute, StaticAttributeRef<T> reference) -> Bool
    {
        if (BindAttribute(owner, attribute))
        {
            SetReference(reference);
            return true;
        }
        return false;
    }

    ///
    /// @brief Unbind attribute.
    ///
    auto UnbindAttribute() -> void
    {
        _connection.Disconnect();
        _observer.Reset();
    }

    ///
    /// @brief Set value of argument to the attribute currently binding.
    ///
    /// @param[in] arg AttributeArg
    ///
    auto SetAttributeArg(AttributeArg<T> const& arg) -> void
    {
        if (_observer)
        {
            if (auto const value = arg.GetValue())
            {
                _observer->SetValue(*value);
            }
            else if (auto const reference = arg.GetReference())
            {
                _observer->SetReference(*reference);
            }
        }
    }

    ///
    /// @brief Set value to the attribute currently binding.
    ///
    /// @param[in] value New value of attribute
    ///
    auto SetValue(T const& value) -> void
    {
        if (_observer)
        {
            _observer->SetValue(value);
        }
    }


    ///
    /// @brief Set value to the attribute currently binding.
    ///
    /// @param[in] reference New value of attribute
    ///
    auto SetReference(StaticAttributeRef<T> reference) -> void
    {
        if (_observer)
        {
            _observer->SetReference(reference);
        }
    }

    ///
    /// @brief Get current value of attribute.
    ///
    auto GetValue() const -> Optional<T>
    {
        if (_observer)
        {
            return _observer->GetValue();
        }
        return {};
    }

    ///
    /// @brief Get current value of attribute, or return default value on failure.
    ///
    /// @param[in] defaultValue Default value
    ///
    auto GetValueOr(const T& defaultValue) const -> T
    {
        if (auto const value = GetValue())
        {
            return *value;
        }
        return defaultValue;
    }

    ///
    /// @brief Get current value of attribute, or return default value on failure.
    ///
    auto GetValueOrDefault() const -> T
    {
        if (auto const value = GetValue())
        {
            return *value;
        }
        return T();
    }

    ///
    /// @brief Get id of currently binding attribute.
    ///
    auto GetAttributeId() const -> AttributeId
    {
        if (_observer)
        {
            return _observer->GetAttributeId();
        }
        return 0U;
    }

    auto GetEventReceiver() -> EventReceiver&
    {
        return *_eventReceiver;
    }

    auto GetTracker() -> Tracker&
    {
        return _eventReceiver->GetTracker();
    }

private:
    auto ReceiveEvent(Event<>& event) -> Async<Bool>
    {
        if (event.Is<AttributeEvent::ValueChanged>())
        {
            if (_eventReceiver)
            {
                co_return co_await _eventReceiver->SendEvent(event);
            }
        }
        co_return false;
    }

private:
    Shared<EventReceiver> _eventReceiver;
    Unique<AttributeObserver<T>> _observer;
    ScopedSignalConnection _connection;
};
}
}

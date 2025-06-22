// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Dependency.DependencyNode.hpp"
#include "Futurewalker.Dependency.DependencyNodeEvent.hpp"

#include <algorithm>

#include <cassert>

namespace FW_DETAIL_NS
{
///
/// @brief Make instance.
///
auto DependencyNode::Make(Delegate delegate) -> Shared<DependencyNode>
{
    auto node = Shared<DependencyNode>::Make(PassKey<DependencyNode>(), std::move(delegate));
    node->SetSelf(node);
    return node;
}

///
/// @brief Constructor.
///
DependencyNode::DependencyNode(PassKey<DependencyNode>, Delegate delegate)
  : _delegate {std::move(delegate)}
{
    _propertyStore = PropertyStore::Make();
}

///
/// @brief Broadcast notify event.
///
auto DependencyNode::Notify() -> void
{
    auto event = Event(DependencyNodeEvent::Notify());
    NotifyCore(event);
}

///
/// @brief Broadcast notify event with user-provided event.
///
/// @param event Event object.
///
auto DependencyNode::NotifyWithEvent(Event const& event) -> void
{
    auto parameter = DependencyNodeEvent::Notify();
    parameter.SetEvent(event);
    NotifyCore(Event(parameter));
}

///
/// @brief Add child node.
///
auto DependencyNode::AddChild(Shared<DependencyNode> const& node, Pointer<DependencyNode const> after) -> void
{
    if (node)
    {
        node->RemoveFromParent();
        node->SetParent(GetSelf());
        _children.insert(std::find_if(_children.begin(), _children.end(), [&](auto const& child) { return child.GetPointer() == after; }), node);
    }
}

///
/// @brief Get child node at index.
///
auto DependencyNode::GetChildAt(SInt64 const index) -> Shared<DependencyNode>
{
    if (0 <= index && index < GetChildCount())
    {
        return *std::next(_children.begin(), static_cast<int64_t>(index));
    }
    return nullptr;
}

///
/// @brief Get number of child nodes.
///
auto DependencyNode::GetChildCount() const -> SInt64
{
    return std::ssize(_children);
}

///
/// @brief Remove child node.
///
auto DependencyNode::RemoveChild(Shared<DependencyNode> const& node) -> void
{
    if (!node)
    {
        return;
    }

    for (auto const& child : _children)
    {
        if (node == child)
        {
            node->RemoveFromParent();
            return;
        }
    }
}

///
/// @brief Get parent node.
///
auto DependencyNode::GetParent() -> Shared<DependencyNode>
{
    return _parent.Lock();
}

///
/// @brief Get parent node.
///
auto DependencyNode::GetParent() const -> Shared<DependencyNode const>
{
    return _parent.Lock();
}

///
/// @brief Get property store.
///
auto DependencyNode::GetPropertyStore() -> PropertyStore&
{
    return _propertyStore->GetPropertyStore(); 
}

///
/// @brief Get property store.
///
auto DependencyNode::GetPropertyStore() const -> PropertyStore const&
{
    return _propertyStore->GetPropertyStore();
}

///
/// @brief Get self.
///
auto DependencyNode::GetSelf() -> Shared<DependencyNode>
{
    return _self.Lock();
}

///
/// @brief Set self.
///
auto DependencyNode::SetSelf(Shared<DependencyNode> const& self) -> void
{
    _self = self;
}

///
/// @brief Set parent.
///
auto DependencyNode::SetParent(Shared<DependencyNode> const& parent) -> void
{
    _parent = parent;
}

///
/// @brief Remove from parent node.
///
auto DependencyNode::RemoveFromParent() -> void
{
    if (auto parent = GetParent())
    {
        auto const self = GetSelf();
        auto const it = std::remove(parent->_children.begin(), parent->_children.end(), self);
        parent->_children.erase(it, parent->_children.end());
        SetParent(nullptr);
    }
}

///
/// @brief Send notify event.
///
auto DependencyNode::NotifyCore(Event const& event) -> void
{
    if (!NotifySelf(event))
    {
        NotifyChildren(event);
    }
}

///
/// @brief Send notify event to self.
///
auto DependencyNode::NotifySelf(Event const& event) -> Bool
{
    try
    {
        return SendNotifyEvent(event);
    }
    catch (...)
    {
        assert(false);
    }
    return false;
}

///
/// @brief Notify child nodes.
///
auto DependencyNode::NotifyChildren(Event const& event) -> void
{
    for (auto const& child : _children)
    {
        child->NotifyCore(event);
    }
}

///
/// @brief Send notification event to owner.
///
auto DependencyNode::SendNotifyEvent(Event const& event) -> Bool
{
    if (_delegate.dispatchEvent)
    {
        auto copyEvent = event;
        copyEvent.As<DependencyNodeEvent::Notify>().SetSender(GetSelf());
        return _delegate.dispatchEvent(copyEvent);
    }
    return false;
}
}

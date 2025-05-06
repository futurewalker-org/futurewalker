// SPDX-License-Identifier: MIT

#include "Futurewalker.Application.FocusNode.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Make focus node.
///
auto FocusNode::Make() -> Shared<FocusNode>
{
    return FocusNode::MakeDerived<FocusNode>();
}

FocusNode::FocusNode(PassKey<FocusNode>)
{
    _eventReceiver = EventReceiver::Make();
}

FocusNode::~FocusNode() = default;

auto FocusNode::RequestFocus() -> void
{
    if (auto root = GetRoot())
    {
        root->RootRequestFocus(GetSelf());
    }
}

auto FocusNode::ReleaseFocus() -> void
{
    if (auto root = GetRoot())
    {
        root->RootReleaseFocus(GetSelf());
    }
}

auto FocusNode::AddChild(Shared<FocusNode> const& child, Pointer<FocusNode const> position) -> void
{
    if (child)
    {
        if (auto parent = child->GetParent())
        {
            parent->AbandonChild(child);
        }
        AdoptChild(child, position);
    }
}

auto FocusNode::RemoveChild(Shared<FocusNode> const& child) -> void
{
    if (child)
    {
        if (child->GetParent() == GetSelf())
        {
            AbandonChild(child);
        }
    }
}

auto FocusNode::IsFocused() const -> Bool
{
    // TODO: cache result.
    if (auto root = GetRoot())
    {
        return root->RootGetFocusedNode() == GetSelf();
    }
    return false;
}

auto FocusNode::GetNextFocus() -> Shared<FocusNode>
{
    // TODO
    return GetSelf();
}

auto FocusNode::GetChildren() -> FocusNodeList&
{
    return _children;
}

auto FocusNode::GetChildren() const -> FocusNodeList const&
{
    return _children;
}

auto FocusNode::GetEventReceiver() -> EventReceiver&
{
    return *_eventReceiver;
}

auto FocusNode::GetEventReceiver() const -> EventReceiver const&
{
    return *_eventReceiver;
}

auto FocusNode::SendEvent(Event& event) -> Async<Bool>
{
    co_return co_await GetEventReceiver().SendEvent(event);
}

auto FocusNode::SendEventDetached(Event& event) -> Bool
{
    return GetEventReceiver().SendEventDetached(event);
}

auto FocusNode::GetSelf() -> Shared<FocusNode>
{
    return _self.Lock();
}

auto FocusNode::GetSelf() const -> Shared<FocusNode const>
{
    return _self.Lock();
}

auto FocusNode::SetSelf(Shared<FocusNode> self) -> void
{
    _self = self;
}

auto FocusNode::GetParent() -> Shared<FocusNode>
{
    return _parent.Lock();
}

auto FocusNode::GetParent() const -> Shared<FocusNode const>
{
    return _parent.Lock();
}

auto FocusNode::SetParent(Weak<FocusNode> parent) -> void
{
    _parent = parent;
}

auto FocusNode::GetRoot() -> Shared<FocusNode>
{
    if (auto parent = GetParent())
    {
        return parent->GetRoot();
    }
    return GetSelf();
}

auto FocusNode::GetRoot() const -> Shared<FocusNode const>
{
    if (auto parent = GetParent())
    {
        return parent->GetRoot();
    }
    return GetSelf();
}

auto FocusNode::AdoptChild(Shared<FocusNode> const& child, Pointer<FocusNode const> position) -> void
{
    child->SetParent(GetSelf());
    _children.insert(std::find_if(_children.begin(), _children.end(), [&](auto const& child) { return child.GetPointer() == position; }), child);
}

auto FocusNode::AbandonChild(Shared<FocusNode> const& child) -> void
{
    _children.erase(std::find(_children.begin(), _children.end(), child));
    child->SetParent(nullptr);
}

auto FocusNode::RootGetFocusedNode() const -> Shared<FocusNode>
{
    return {};
}

auto FocusNode::RootRequestFocus(Shared<FocusNode> node) -> void
{
    (void)node;
}

auto FocusNode::RootReleaseFocus(Shared<FocusNode> node) -> void
{
    (void)node;
}
}
}

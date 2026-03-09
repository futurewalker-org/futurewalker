// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.FocusNode.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

#include "Futurewalker.Base.Debug.hpp" 

#include <chrono>

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

auto FocusNode::RequestFocus(FocusReason const reason) -> void
{
    if (auto root = GetRoot())
    {
        root->RootRequestFocus(GetSelf(), reason);
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

auto FocusNode::IsFocusable() const -> Bool
{
    return _focusable;
}

auto FocusNode::SetFocusable(Bool const focusable) -> void
{
    _focusable = focusable;
}

auto FocusNode::GetNextFocus() -> Shared<FocusNode>
{
    return Traverse(true, {}, GetSelf());
}

auto FocusNode::GetPrevFocus() -> Shared<FocusNode>
{
    return Traverse(false, {}, GetSelf());
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

auto FocusNode::SendEvent(Event<>& event) -> Async<Bool>
{
    co_return co_await GetEventReceiver().SendEvent(event);
}

auto FocusNode::SendEventDetached(Event<>& event) -> Bool
{
    return GetEventReceiver().SendEventDetached(event);
}

auto FocusNode::DispatchKeyEventFromRoot(PassKey<RootFocusNode>, Event<>& event, Shared<FocusNode> const& target) -> Bool
{
    return DispatchKeyEvent(event, target);
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

auto FocusNode::DispatchKeyEvent(Event<>& event, Shared<FocusNode> const& target) -> Bool
{
    auto const self = GetSelf();

    auto next = target;
    if (self != target)
    {
        // TODO: Precompute path from root to each node to avoid this.
        while (next)
        {
            if (auto parent = next->GetParent())
            {
                if (parent != self)
                {
                    next = parent;
                    continue;
                }
            }
            break;
        }

        if (!next)
        {
            return false;
        }
    }

    if (self == target)
    {
        if (SendEventDetached(event))
        {
            return true;
        }
        return false;
    }

    for (auto const& child : _children)
    {
        if (child.GetPointer() == next.GetPointer())
        {
            if (child->DispatchKeyEvent(event, target))
            {
                return true;
            }
            break;
        }
    }

    if (SendEventDetached(event))
    {
        return true;
    }
    return false;
}

///
/// @brief Find next or previous focus node.
///
/// @param[in] forward Whether to find next (true) or previous (false) focus node.
/// @param[in] prev Previous node in traversal, or nullopt if this is the first node.
/// @param[in] start The starting node for traversal.
///
/// @return The next or previous focus node, or start if no other focusable node is found.
///
/// TODO: Don't use recursion.
///
auto FocusNode::Traverse(const Bool forward, Shared<FocusNode> const& prev, Shared<FocusNode> const& start) -> Shared<FocusNode>
{
    auto const self = GetSelf();
    auto const parent = GetParent();

    if (prev)
    {
        if (prev == parent)
        {
            if (self == start)
            {
                return self;
            }

            if (IsFocusable())
            {
                return self;
            }

            if (forward)
            {
                if (_children.empty())
                {
                    if (parent)
                    {
                        return parent->Traverse(forward, self, start);
                    }
                }
                else
                {
                    return _children.front()->Traverse(forward, self, start);
                }
            }
            else
            {
                if (_children.empty())
                {
                    if (parent)
                    {
                        return parent->Traverse(forward, self, start);
                    }
                }
                else
                {
                    return _children.back()->Traverse(forward, self, start);
                }
            }
        }
        else
        {
            auto const it = std::find_if(_children.begin(), _children.end(), [&](auto const& child) { return child.GetPointer() == prev.GetPointer(); });
            if (it != _children.end())
            {
                if (forward)
                {
                    auto nextIt = std::next(it);
                    if (nextIt != _children.end())
                    {
                        return (*nextIt)->Traverse(forward, self, start);
                    }

                    if (parent)
                    {
                        return parent->Traverse(forward, self, start);
                    }
                    return _children.front()->Traverse(forward, self, start);
                }
                else
                {
                    if (it != _children.begin())
                    {
                        auto const prevIt = std::prev(it);
                        return (*prevIt)->Traverse(forward, self, start);
                    }

                    if (parent)
                    {
                        return parent->Traverse(forward, self, start);
                    }
                    return _children.back()->Traverse(forward, self, start);
                }
            }
        }
    }
    else
    {
        if (forward)
        {
            if (_children.empty())
            {
                if (parent)
                {
                    return parent->Traverse(forward, self, start);
                }
            }
            else
            {
                return _children.front()->Traverse(forward, self, start);
            }
        }
        else
        {
            if (parent)
            {
                return parent->Traverse(forward, self, start);
            }
            else
            {
                if (!_children.empty())
                {
                    return _children.back()->Traverse(forward, self, start);
                }
            }
        }
    }
    return start;
}

auto FocusNode::RootGetFocusedNode() const -> Shared<FocusNode>
{
    return {};
}

auto FocusNode::RootRequestFocus(Shared<FocusNode> node, FocusReason const reason) -> void
{
    (void)node;
    (void)reason;
}

auto FocusNode::RootReleaseFocus(Shared<FocusNode> node) -> void
{
    (void)node;
}
}
}

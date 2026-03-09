// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.RootFocusNode.hpp"
#include "Futurewalker.Application.FocusEvent.hpp"

#include "Futurewalker.Event.Event.hpp"

#include "Futurewalker.Base.Debug.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Make instance.
///
auto RootFocusNode::Make() -> Shared<RootFocusNode>
{
    return FocusNode::MakeDerived<RootFocusNode>();
}

///
/// @brief Constructor.
///
RootFocusNode::RootFocusNode(PassKey<FocusNode> key)
  : FocusNode(key)
{
}

///
/// @brief Set activate state of focus node.
///
auto RootFocusNode::SetActive(Bool const active) -> void
{
    if (_active != active)
    {
        _active = active;

        if (_active)
        {
            if (auto const lastFocusedNode = _lastFocusedNode.Lock())
            {
                SetFocusedNode(lastFocusedNode, _lastFocusReason);
            }
        }
        else
        {
            if (GetFocusedNode())
            {
                SetFocusedNode({}, FocusReason::Other);
            }
            else
            {
                _lastFocusedNode = {};
                _lastFocusReason = FocusReason::Other;
            }
        }
    }
}

///
/// @brief Get next focus candidate for keyboard navigation.
///
auto RootFocusNode::GetNextFocusCandidate() -> Shared<FocusNode>
{
    if (auto focusedNode = GetFocusedNode())
    {
        return focusedNode->GetNextFocus();
    }

    if (auto lastFocusedNode = _lastFocusedNode.Lock())
    {
        return lastFocusedNode->GetNextFocus();
    }

    if (auto nextFocus = GetNextFocus())
    {
        if (nextFocus.GetPointer() != this)
        {
            return nextFocus;
        }
    }
    return {};
}

///
/// @brief Get previous focus candidate for keyboard navigation.
///
auto RootFocusNode::GetPrevFocusCandidate() -> Shared<FocusNode>
{
    if (auto focusedNode = GetFocusedNode())
    {
        return focusedNode->GetPrevFocus();
    }

    if (auto lastFocusedNode = _lastFocusedNode.Lock())
    {
        return lastFocusedNode->GetPrevFocus();
    }

    if (auto prevFocus = GetPrevFocus())
    {
        if (prevFocus.GetPointer() != this)
        {
            return prevFocus;
        }
    }
    return {};
}

///
/// @brief Get currently focused node.
///
auto RootFocusNode::GetFocusedNode() -> Shared<FocusNode>
{
    return _focusedNode.Lock();
}

///
/// @brief Get currently focused node.
///
auto RootFocusNode::GetFocusedNode() const -> Shared<FocusNode const>
{
    return _focusedNode.Lock();
}

///
/// @brief Dispatch key event to focused node.
///
/// @note If there is not focused node, the event will be dispatched to this node.
///
auto RootFocusNode::DispatchKeyEvent(Event<>& event) -> Bool
{
    if (!DispatchKeyEventFromRoot({}, event, GetFocusedNode()))
    {
        return SendEventDetached(event);
    }
    return false;
}

///
/// @brief Set focused node.
///
auto RootFocusNode::SetFocusedNode(Shared<FocusNode> const& newNode, FocusReason const reason) -> void
{
    auto const oldNode = _focusedNode.Lock();
    auto const oldReason = _focusReason;
    if (oldNode != newNode)
    {
        {
            auto event = Event<>(Event<FocusEvent::FocusWillChange>());
            SendEventDetached(event);
        }

        _focusedNode = newNode;
        _focusReason = reason;
        _lastFocusedNode = oldNode;
        _lastFocusReason = oldReason;

        if (oldNode)
        {
            auto event = Event<>(Event<FocusEvent::FocusOut>());
            oldNode->SendEventDetached(event);
        }

        if (newNode)
        {
            auto focusEvent = Event<FocusEvent::FocusIn>();
            focusEvent->SetReason(reason);
            auto event = Event<>(std::move(focusEvent));
            newNode->SendEventDetached(event);
        }

        {
            auto event = Event<>(Event<FocusEvent::FocusDidChange>());
            SendEventDetached(event);
        }
    }
}

///
/// @brief
///
auto RootFocusNode::RootGetFocusedNode() const -> Shared<FocusNode>
{
    return _focusedNode.Lock();
}

///
/// @brief
///
auto RootFocusNode::RootRequestFocus(Shared<FocusNode> node, FocusReason const reason) -> void
{
    if (node && node->IsFocusable())
    {
        SetFocusedNode(node, reason);
    }
}

///
/// @brief
///
auto RootFocusNode::RootReleaseFocus(Shared<FocusNode> node) -> void
{
    if (node && node == RootGetFocusedNode())
    {
        SetFocusedNode({}, FocusReason::Other);
    }
}
}
}

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
/// @brief Set focused node.
///
auto RootFocusNode::SetFocusedNode(Shared<FocusNode> const& newNode) -> void
{
    auto const oldNode = RootGetFocusedNode();
    if (oldNode != newNode)
    {
        {
            auto event = Event(FocusEvent::FocusWillChange());
            SendEventDetached(event);
        }

        _focusedNode = newNode;

        if (oldNode)
        {
            auto parameter = FocusEvent::FocusOut();
            auto event = Event(parameter);
            oldNode->SendEventDetached(event);
        }

        if (newNode)
        {
            auto parameter = FocusEvent::FocusIn();
            auto event = Event(parameter);
            newNode->SendEventDetached(event);
        }

        {
            auto event = Event(FocusEvent::FocusDidChange());
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
auto RootFocusNode::RootRequestFocus(Shared<FocusNode> node) -> void
{
    if (node)
    {
        SetFocusedNode(node);
    }
}

///
/// @brief
///
auto RootFocusNode::RootReleaseFocus(Shared<FocusNode> node) -> void
{
    if (node && node == RootGetFocusedNode())
    {
        auto next = node->GetNextFocus();
        if (next != node)
        {
            SetFocusedNode(next);
        }
    }
}
}
}

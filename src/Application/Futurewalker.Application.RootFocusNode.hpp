// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.RootFocusNodeType.hpp"

#include "Futurewalker.Application.FocusNode.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Root focus node.
///
class RootFocusNode : public FocusNode
{
public:
    static auto Make() -> Shared<RootFocusNode>;

    RootFocusNode(PassKey<FocusNode> key);

    auto SetActive(Bool const active) -> void;

    auto GetNextFocusCandidate() -> Shared<FocusNode>;
    auto GetPrevFocusCandidate() -> Shared<FocusNode>;

    auto GetFocusedNode() -> Shared<FocusNode>;
    auto GetFocusedNode() const -> Shared<FocusNode const>;

    auto DispatchKeyEvent(Event<>& event) -> Bool;

private:
    auto SetFocusedNode(Shared<FocusNode> const& node, FocusReason const reason) -> void;

private:
    auto RootGetFocusedNode() const -> Shared<FocusNode> override;
    auto RootRequestFocus(Shared<FocusNode> node, FocusReason const reason) -> void override;
    auto RootReleaseFocus(Shared<FocusNode> node) -> void override;

private:
    Bool _active = true;
    Weak<FocusNode> _focusedNode;
    FocusReason _focusReason = FocusReason::Other;
    Weak<FocusNode> _lastFocusedNode;
    FocusReason _lastFocusReason = FocusReason::Other;
};
}
}

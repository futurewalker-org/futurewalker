// SPDX-License-Identifier: MIT
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

    auto GetFocusedNode() -> Shared<FocusNode>;
    auto GetFocusedNode() const -> Shared<FocusNode const>;

private:
    auto SetFocusedNode(Shared<FocusNode> const& node) -> void;

private:
    auto RootGetFocusedNode() const -> Shared<FocusNode> override;
    auto RootRequestFocus(Shared<FocusNode> node) -> void override;
    auto RootReleaseFocus(Shared<FocusNode> node) -> void override;

private:
    Weak<FocusNode> _focusedNode;
};
}
}

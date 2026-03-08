// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ActionView.hpp"

#include "Futurewalker.Action.CommandNode.hpp"

namespace FW_DETAIL_NS
{
auto ActionView::Make() -> Shared<ActionView>
{
    return View::MakeDerived<ActionView>();
}

auto ActionView::MakeWithContent(Shared<View> contentView) -> Shared<ActionView>
{
    auto view = Make();
    view->SetContent(contentView);
    return view;
}

ActionView::ActionView(PassKey<View> key)
  : View(key)
{
}

auto ActionView::GetContent() -> Shared<View>
{
    return GetChildAt(0);
}

auto ActionView::SetContent(Shared<View> content) -> void
{
    auto const child = GetChildAt(0);
    if (child != content)
    {
        if (child)
        {
            child->RemoveFromParent();
        }
        AddChildBack(content);
    }
}

auto ActionView::AddAction(CommandId const& command, Shared<Action> const& action) -> void
{
    CommandNode::AddAction(*this, command, action);
}

auto ActionView::RemoveAction(CommandId const& command) -> void
{
    CommandNode::RemoveAction(*this, command);
}
}

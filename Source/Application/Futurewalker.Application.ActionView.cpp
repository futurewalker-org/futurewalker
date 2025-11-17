// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ActionView.hpp"
#include "Futurewalker.Application.ContainerView.hpp"

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
    if (_container)
    {
        return _container->GetContent();
    }
    return {};
}

auto ActionView::SetContent(Shared<View> content) -> void
{
    if (_container)
    {
        _container->SetContent(content);
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

auto ActionView::Initialize() -> void
{
    _container = ContainerView::Make();
    AddChildBack(_container);
}
}

// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ContainerView.hpp"

#include "Futurewalker.Event.EventReceiver.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Make ContainerView instance.
///
auto ContainerView::Make() -> Shared<ContainerView>
{
    return View::MakeDerived<ContainerView>();
}

///
/// @brief Make ContainerView instance with specified content.
///
/// @param[in] content Content of new instance.
///
auto ContainerView::MakeWithContent(Shared<View> const& content) -> Shared<ContainerView>
{
    auto view = Make();
    view->SetContent(content);
    return view;
}

///
/// @brief Constructor.
///
ContainerView::ContainerView(PassKey<View> key)
  : View(key)
{
}

auto ContainerView::GetContent() -> Shared<View>
{
    return GetChildAt(0);
}

auto ContainerView::GetContent() const -> Shared<View const>
{
    return GetChildAt(0);
}

auto ContainerView::SetContent(Shared<View> const& view) -> void
{
    auto const child = GetChildAt(0);
    if (child != view)
    {
        if (child)
        {
            child->RemoveFromParent();
        }
        AddChildBack(view);
    }
}
}

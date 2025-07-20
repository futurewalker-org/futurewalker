// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.ArrangeScope.hpp"
#include "Futurewalker.Application.View.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Construct arrange scope.
///
/// @param parameter
///
ArrangeScope::ArrangeScope(PassKey<View>, View& view, ArrangeParameter const& parameter)
  : _view {view}
  , _parameter {parameter}
{
}

///
/// @brief Get parameter.
///
auto ArrangeScope::GetParameter() -> ArrangeParameter const&
{
    return _parameter;
}

///
/// @brief Arrange child view.
///
/// @param view Child view
/// @param position Position to be arranged
///
auto ArrangeScope::ArrangeChild(ReferenceArg<View> view, Point<Dp> const& position) -> void
{
    if (view && view->IsChildOf(_view))
    {
        auto parameter = ArrangeParameter();
        parameter.SetOldFrameRect(view->GetFrameRect());
        parameter.SetNewFrameRect(Rect<Dp>(position, GetMeasuredSize(*view)));
        view->EnterArrangeScope(PassKey<ArrangeScope>(), parameter);
        return;
    }
    FW_DEBUG_ASSERT(false);
}

///
/// @brief Get measured size of child view.
///
/// @param view
///
auto ArrangeScope::GetMeasuredSize(ReferenceArg<View> view) -> Size<Dp>
{
    if (view)
    {
        return view->GetMeasuredSize(PassKey<ArrangeScope>());
    }
    return {};
}

///
/// @brief Arrange from RootView.
///
/// @param view
///
auto ArrangeScope::ArrangeRootView(PassKey<RootView>, View& view) -> void
{
    auto parameter = ArrangeParameter();
    parameter.SetOldFrameRect(view.GetFrameRect());
    parameter.SetNewFrameRect(Rect<Dp>(Point<Dp>(), view.GetMeasuredSize(PassKey<ArrangeScope>())));
    view.EnterArrangeScope({}, parameter);
}
}

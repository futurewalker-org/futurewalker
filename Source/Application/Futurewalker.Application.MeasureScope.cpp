// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.MeasureScope.hpp"
#include "Futurewalker.Application.View.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief Construct measure scope.
///
/// @param parameter
///
MeasureScope::MeasureScope(PassKey<View>, View& view, MeasureParameter const& parameter)
  : _view {view}
  , _parameter {parameter}
{
}

///
/// @brief
///
auto MeasureScope::GetParameter() const -> MeasureParameter const&
{
    return _parameter;
}

///
/// @brief Measure child view with same constraints.
///
/// @param child Child view
///
/// @return Measured size of child view
///
auto MeasureScope::MeasureChild(ReferenceArg<View> child) -> Size<Dp>
{
    if (child)
    {
        return MeasureChild(*child, _parameter.GetWidthConstraints(), _parameter.GetHeightConstraints());
    }
    return {};
}

///
/// @brief Measure child with constraints. 
///
/// @param width
/// @param height
///
/// @return Measure size of child view
///
auto MeasureScope::MeasureChild(ReferenceArg<View> child, AxisConstraints const& width, AxisConstraints const& height) -> Size<Dp>
{
    if (child)
    {
        auto parameter = MeasureParameter();
        parameter.SetWidthConstraints(width);
        parameter.SetHeightConstraints(height);
        child->EnterMeasureScope(PassKey<MeasureScope>(), parameter);
        return GetMeasuredSize(*child);
    }
    return {};
}

///
/// @brief Get measured size of child view
///
/// @param view Child view
///
auto MeasureScope::GetMeasuredSize(ReferenceArg<View const> view) -> Size<Dp>
{
    if (view)
    {
        return view->GetMeasuredSize(PassKey<MeasureScope>());
    }
    return {};
}

///
/// @brief Set measured size of current view.
///
/// @param size Measured size of current view
///
/// @note `size` have to be finite
///
auto MeasureScope::SetMeasuredSize(Size<Dp> const& size) -> void
{
    _view.SetMeasuredSize(PassKey<MeasureScope>(), _parameter, size);
}

///
/// @brief Set measured size of current view.
///
/// @param width Measured width of current view 
/// @param height Measured height of current view 
///
/// @note Both `width` and `height` have to be finite
///
auto MeasureScope::SetMeasuredSize(Dp const width, Dp const height) -> void
{
    SetMeasuredSize(Size<Dp>(width, height));
}

///
/// @brief Measure from RootView.
///
/// @param view
/// @param size
///
auto MeasureScope::MeasureRootView(PassKey<RootView>, View& view, Size<Dp> const& size) -> void
{
    auto parameter = MeasureParameter();
    parameter.SetWidthConstraints(AxisConstraints::MakeExact(size.GetWidth()));
    parameter.SetHeightConstraints(AxisConstraints::MakeExact(size.GetHeight()));
    view.EnterMeasureScope({}, parameter);
}
}

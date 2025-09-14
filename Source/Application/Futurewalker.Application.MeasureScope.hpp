// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.MeasureScopeType.hpp"
#include "Futurewalker.Application.MeasureParameter.hpp"
#include "Futurewalker.Application.AxisConstraints.hpp"
#include "Futurewalker.Application.ViewType.hpp"
#include "Futurewalker.Application.RootViewType.hpp"

#include "Futurewalker.Geometry.hpp"

#include "Futurewalker.Core.ReferenceArg.hpp"
#include "Futurewalker.Core.NonCopyable.hpp"
#include "Futurewalker.Core.PassKey.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief
///
class MeasureScope : NonCopyable
{
public:
    MeasureScope(PassKey<View>, View& view, MeasureParameter const& parameter);

    auto GetParameter() const -> MeasureParameter const&;

    auto MeasureChild(ReferenceArg<View> child) -> Size<Dp>;
    auto MeasureChild(ReferenceArg<View> child, BoxConstraints const& constraints) -> Size<Dp>;
    auto MeasureChild(ReferenceArg<View> child, AxisConstraints const& widthConstraints, AxisConstraints const& heightConstraints) -> Size<Dp>;

    auto GetMeasuredSize(ReferenceArg<View const> view) -> Size<Dp>;

    auto SetMeasuredSize(Size<Dp> const& size) -> void;
    auto SetMeasuredSize(Dp const width, Dp const height) -> void;

    static auto MeasureView(View& view, BoxConstraints const& constraints) -> Size<Dp>;

private:
    View& _view;
    MeasureParameter _parameter;
};
}
}

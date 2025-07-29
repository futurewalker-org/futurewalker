// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.MeasureParameter.hpp"

namespace FW_DETAIL_NS
{
auto MeasureParameter::GetConstraints() const -> BoxConstraints const&
{
    return _constraints;
}

auto MeasureParameter::SetConstraints(BoxConstraints const& constraints) -> void
{
    _constraints = constraints;
}

///
/// @brief
///
auto MeasureParameter::GetWidthConstraints() const -> AxisConstraints const&
{
    return _constraints.GetWidthConstraints();
}

///
/// @brief
///
auto MeasureParameter::GetHeightConstraints() const -> AxisConstraints const&
{
    return _constraints.GetHeightConstraints();
}
}

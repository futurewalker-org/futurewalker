// SPDX-License-Identifier: MPL-2.0

#include "Futurewalker.Application.MeasureParameter.hpp"

namespace FW_DETAIL_NS
{
///
/// @brief
///
auto MeasureParameter::GetWidthConstraints() const -> AxisConstraints const&
{
    return _widthConstraints;
}

///
/// @brief
///
auto MeasureParameter::SetWidthConstraints(AxisConstraints const& width) -> void
{
    _widthConstraints = width;
}

///
/// @brief
///
auto MeasureParameter::GetHeightConstraints() const -> AxisConstraints const&
{
    return _heightConstraints;
}

///
/// @brief
///
auto MeasureParameter::SetHeightConstraints(AxisConstraints const& height) -> void
{
    _heightConstraints = height;
}
}

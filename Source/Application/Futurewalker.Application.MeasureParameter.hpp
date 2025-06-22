// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.MeasureParameterType.hpp"
#include "Futurewalker.Application.AxisConstraints.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Measure parameter.
///
class MeasureParameter
{
public:
    auto GetWidthConstraints() const -> AxisConstraints const&;
    auto SetWidthConstraints(AxisConstraints const& width) -> void;

    auto GetHeightConstraints() const -> AxisConstraints const&;
    auto SetHeightConstraints(AxisConstraints const& height) -> void;

private:
    AxisConstraints _widthConstraints;
    AxisConstraints _heightConstraints;
};
}
}

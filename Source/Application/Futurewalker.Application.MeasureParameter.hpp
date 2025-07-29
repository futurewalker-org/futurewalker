// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.MeasureParameterType.hpp"
#include "Futurewalker.Application.BoxConstraints.hpp"

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
    auto GetConstraints() const -> BoxConstraints const&;
    auto SetConstraints(BoxConstraints const& constraints) -> void;

    auto GetWidthConstraints() const -> AxisConstraints const&;
    auto GetHeightConstraints() const -> AxisConstraints const&;

private:
    BoxConstraints _constraints;
};
}
}

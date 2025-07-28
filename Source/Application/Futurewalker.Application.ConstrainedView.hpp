// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.ConstrainedViewType.hpp"
#include "Futurewalker.Application.AxisConstraints.hpp"
#include "Futurewalker.Application.ContainerViewType.hpp"

#include "Futurewalker.Application.View.hpp" 

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
//!
//! @brief
//!
class ConstrainedView final : public View
{
public:
    static auto MakeView() -> Shared<ConstrainedView>;

    ConstrainedView(PassKey<View> key);

    auto GetContent() -> Shared<View>;
    auto SetContent(Shared<View> const& content) -> void;

    auto GetWidthConstraints() const -> AxisConstraints;
    auto SetWidthConstraints(AxisConstraints const& constraints) -> void;

    auto GetHeightConstraints() const -> AxisConstraints;
    auto SetHeightConstraints(AxisConstraints const& constraints) -> void;

protected:
    auto Initialize() -> void override;
    auto Measure(MeasureScope& scope) -> void override;
    auto Arrange(ArrangeScope& scope) -> void override;

private:
    Shared<ContainerView> _container;
    AxisConstraints _widthConstraints;
    AxisConstraints _heightConstraints;
};
}
}

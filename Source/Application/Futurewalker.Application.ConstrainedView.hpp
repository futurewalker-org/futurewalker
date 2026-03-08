// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.ConstrainedViewType.hpp"
#include "Futurewalker.Application.AxisConstraints.hpp"
#include "Futurewalker.Application.View.hpp" 

#include "Futurewalker.Attribute.AttributeArg.hpp"
#include "Futurewalker.Attribute.AttributeAccessor.hpp"

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
    auto GetContent() const -> Shared<View const>;
    auto SetContent(Shared<View> const& content) -> void;

    auto GetWidthConstraints() const -> AxisConstraints;
    auto SetWidthConstraints(AttributeArg<AxisConstraints> const& constraints) -> void;

    auto GetHeightConstraints() const -> AxisConstraints;
    auto SetHeightConstraints(AttributeArg<AxisConstraints> const& constraints) -> void;

protected:
    auto Initialize() -> void override;
    auto Measure(MeasureScope& scope) -> void override;

private:
    auto ReceiveAttributeEvent(Event<>& event) -> Async<Bool>;
    auto IntersectConstraints(AxisConstraints const& source, AxisConstraints const& mask) -> AxisConstraints;

private:
    AttributeAccessor<AxisConstraints> _widthConstraints;
    AttributeAccessor<AxisConstraints> _heightConstraints;
};
}
}

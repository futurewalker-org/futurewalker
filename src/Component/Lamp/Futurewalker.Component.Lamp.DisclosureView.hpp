// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Component.Lamp.DisclosureViewType.hpp"

#include "Futurewalker.Application.View.hpp"

#include "Futurewalker.Attribute.AttributeArg.hpp"
#include "Futurewalker.Attribute.AttributeAccessor.hpp"

namespace FW_LAMP_DETAIL_NS
{
namespace FW_EXPORT
{
class DisclosureView : public View
{
public:
    static auto Make() -> Shared<DisclosureView>;

    auto GetContent() -> Shared<View>;
    auto GetContent() const -> Shared<View const>;
    auto SetContent(Shared<View> content) -> void;

    auto SetOpenRatio(AttributeArg<Float64> const& openRatio) -> void;

    DisclosureView(PassKey<View> key);

private:
    auto Initialize() -> void override;
    auto Measure(MeasureScope& scope) -> void override;
    auto Arrange(ArrangeScope& scope) -> void override;
    auto Draw(DrawScope& scope) -> void override;
    auto ReceiveAttributeEvent(Event<>& event) -> Bool;
    auto UpdateContentVisibility() -> void;

private:
    Float64 _measuredOpenRatio = 0.0;
    AttributeAccessor<Float64> _openRatio;
};
}
}

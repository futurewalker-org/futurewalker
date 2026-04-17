// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.ScrollViewType.hpp"
#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.ContainerView.hpp"

#include "Futurewalker.Attribute.AttributeAccessor.hpp"

#include "Futurewalker.Core.Flags.hpp"

namespace FW_DETAIL_NS
{
namespace FW_EXPORT
{
///
/// @brief Single child scroll view.
///
class ScrollView : public View
{
public:
    static auto Make() -> Shared<ScrollView>;
    static auto MakeWithContent(Shared<View> const& content) -> Shared<ScrollView>;

    ScrollView(PassKey<View> key);

    auto SetContent(Shared<View> const& content) -> void;
    auto GetContent() -> Shared<View>;

    auto GetDirection() const -> Flags<ScrollViewDirection>;
    auto SetDirection(Flags<ScrollViewDirection> const direction) -> void;

protected:
    auto Initialize() -> void override;
    auto Measure(MeasureScope& scope) -> void override;
    auto Arrange(ArrangeScope& scope) -> void override;
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;

private:
    Shared<ContainerView> _containerView;
    Flags<ScrollViewDirection> _direction = ScrollViewDirection::None;
    AttributeAccessor<Float64> _horizontalScrollFactor;
    AttributeAccessor<Float64> _verticalScrollFactor;
    Vector<Dp> _offset;
};
}
}

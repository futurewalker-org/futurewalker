// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "Futurewalker.Application.ScrollViewType.hpp"
#include "Futurewalker.Application.View.hpp"
#include "Futurewalker.Application.ContainerView.hpp"

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

    ScrollView(PassKey<View> key);

    auto SetContent(Shared<View> const& content) -> void;
    auto GetContent() -> Shared<View>;

    auto GetDirection() const -> ScrollViewDirection;
    auto SetDirection(ScrollViewDirection const direction) -> void;

protected:
    auto Initialize() -> void override;
    auto Measure(MeasureScope& scope) -> void override;
    auto Arrange(ArrangeScope& scope) -> void override;
    auto ReceiveEvent(Event<>& event) -> Async<Bool>;

private:
    Shared<ContainerView> _containerView;
    ScrollViewDirection _direction = ScrollViewDirection::None;
    Offset<Dp> _offset;
};
}
}
